#include "NPSWrap.h"
#include <QUrlQuery>
#include <QUuid>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QThread>
#include <QEventLoop>
#include <QTimer>



NPSWrap::NPSWrap(QObject *parent)
    : QObject{parent}
{
    m_pMessage = new QNetworkAccessManager();
    connect(this,&NPSWrap::signal_getCliendIdByVkey,this,&NPSWrap::slot_getCliendIdByVkey);
    connect(this,&NPSWrap::signal_addPorxy,this,&NPSWrap::slot_addPorxy);
    VerifyKey_ = QUuid::createUuid().toString().remove("{").remove("-").remove("}");
    serverPort = 9100;
    myProcess = new QProcess(this);
}

NPSWrap::~NPSWrap()
{

}

//http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp
QString NPSWrap::getTime()
{
    QString time;
    QString url = "http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp";
    QNetworkRequest req;
    req.setUrl(QUrl(url));
    QNetworkReply* reply = m_pMessage->get(req);
    QEventLoop eventLoop;
    QTimer::singleShot(3000,&eventLoop,&QEventLoop::quit);
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    if(doc.isObject()){
        QJsonObject obj = doc.object();
        QJsonValue value = obj.value("data");
        if(value.isObject()){
            obj = value.toObject();
            time =  obj.value("t").toString();
            time = time.mid(0,10);
        }
    }

    //    QDateTime dataTime = QDateTime::currentDateTime();   //获取当前时间
    //    time =  QString::number(dataTime.toTime_t());


    return time;
}

QString NPSWrap::calcSign(QString timestamp, QString key)
{
    QString str =  key + timestamp  ;
    QString md5Str = QCryptographicHash::hash(str.toLatin1(), QCryptographicHash::Md5).toHex();
    return md5Str;
}

void NPSWrap::slot_addClient(QString reqip, QString reqport,QString key,QString remark)
{
    reqip_ = reqip;
    reqport_ = reqport;
    key_ = key;
    QUrl url;
    QString urlStr = QString("http://%1:%2/client/add").arg(reqip).arg(reqport);
    url.setUrl(urlStr);
    QNetworkRequest req;
    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QString timestamp = getTime();
    QString auth_key = calcSign(timestamp,key);
    QUrlQuery body;
    body.addQueryItem("auth_key",auth_key);
    body.addQueryItem("timestamp",timestamp);
    body.addQueryItem("remark",remark);
    body.addQueryItem("u","");
    body.addQueryItem("p","");
    body.addQueryItem("limit","20");
    body.addQueryItem("vkey",VerifyKey_);
    body.addQueryItem("config_conn_allow","1");
    body.addQueryItem("compress","1");
    body.addQueryItem("crypt","1");
    body.addQueryItem("rate_limit","");
    body.addQueryItem("flow_limit","");
    body.addQueryItem("max_conn","");
    body.addQueryItem("max_tunnel","");
    qDebug() <<  __FILE__ << __LINE__ << body.toString();
    QNetworkReply* reply =  m_pMessage->post(req,body.toString().toUtf8());
    connect(reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
            [=](QNetworkReply::NetworkError code){
        qDebug() <<  __FILE__ << __LINE__ << code;
    });
    connect(reply,&QNetworkReply::finished,this,[&](){
        QNetworkReply* reply_ =  (QNetworkReply*)sender();
        QByteArray response = reply_->readAll();
        qDebug() <<  __FILE__ << __LINE__ << response ;
        QJsonDocument doc = QJsonDocument::fromJson(response);
        if(doc.isObject()){
            emit signal_getCliendIdByVkey(reqip_,reqport_,key_);
            QString program = "npc\\npc.exe";
            QStringList arguments;
            arguments  << QString("-server=%1:%2").arg(reqip_).arg(8024);
            arguments  << QString("-vkey=%1").arg(VerifyKey_);
            arguments  << QString("-type=tcp");
            myProcess->start(program, arguments);
        }
        reply_->deleteLater();
    });
}

void NPSWrap::slot_addPorxy(QString clientId,QString localPort,QString remark,bool isTcp)
{
    clientId_ = clientId;
    QUrl url;
    QString urlStr = QString("http://%1:%2/index/add/").arg(reqip_).arg(reqport_);
    url.setUrl(urlStr);
    QNetworkRequest req;
    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QString timestamp = getTime();
    QString auth_key = calcSign(timestamp,key_);
    QUrlQuery body;
    body.addQueryItem("auth_key",auth_key);
    body.addQueryItem("timestamp",timestamp);
    body.addQueryItem("remark",remark);
    if(isTcp){
        body.addQueryItem("type","tcp");
    }else{
        body.addQueryItem("type","udp");
    }
    body.addQueryItem("port",QString::number(serverPort++));
    body.addQueryItem("target",localPort);
    body.addQueryItem("client_id",clientId);
    QNetworkReply* reply =  m_pMessage->post(req,body.toString().toUtf8());
    QPair<QString,QString> info;
    info.first = localPort;
    info.second = remark;
    proxyInfo.insert(reply,info);
    connect(reply,&QNetworkReply::finished,this,[&](){
        QNetworkReply* reply_ =  (QNetworkReply*)sender();
        QByteArray response = reply_->readAll();
        qDebug() <<  __FILE__ << __LINE__ << response;
        QJsonDocument doc = QJsonDocument::fromJson(response);
        if(doc.isObject()){
            QJsonObject obj = doc.object();
            int status = obj.value("status").toInt();
            QPair<QString,QString> info = proxyInfo.value(reply_);
            if(status){
                emit signal_addSuccess(QString("%1:%2:%3 --> %4").arg(info.second).arg(reqip_).arg(serverPort-1).arg(info.first));
            }else{
                emit signal_addPorxy(clientId_,info.first,info.second);
            }
        }
        proxyInfo.remove(reply_);
        reply_->deleteLater();
    });

}

void NPSWrap::slot_getCliendIdByVkey(QString reqip, QString reqport,QString key)
{
    QUrl url;
    QString urlStr = QString("http://%1:%2/client/list").arg(reqip).arg(reqport);
    url.setUrl(urlStr);
    QNetworkRequest req;
    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QString timestamp = getTime();
    QString auth_key = calcSign(timestamp,key);
    QUrlQuery body;
    body.addQueryItem("auth_key",auth_key);
    body.addQueryItem("timestamp",timestamp);
    body.addQueryItem("limit","1000");
    body.addQueryItem("start","0");
    QNetworkReply* reply =  m_pMessage->post(req,body.toString().toUtf8());
    connect(reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
            [=](QNetworkReply::NetworkError code){
        qDebug() <<  __FILE__ << __LINE__ << code;
    });
    connect(reply,&QNetworkReply::finished,this,[&](){
        QNetworkReply* reply_ =  (QNetworkReply*)sender();
        QByteArray response = reply_->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        if(doc.isObject()){
            QJsonObject obj = doc.object();
            QJsonValue value =  obj.value("rows");
            if(value.isArray()){
                QJsonArray arr =  value.toArray();
                if(arr.size()){
                    for(int i = 0;i<arr.size();i++){
                        value = arr.at(i);
                        obj = value.toObject();
                        QString vk = obj.value("VerifyKey").toString();
                        if(vk == VerifyKey_){
                            int id =  obj.value("Id").toInt();
                            emit signal_addSuccess("注册成功");
                            emit signal_getClientId(QString::number(id));
                            break;
                        }
                    }
                }
            }
        }
        reply_->deleteLater();
    });
}

void NPSWrap::slot_deletePorxy(QString proxyId)
{
    QUrl url;
    QString urlStr = QString("http://%1:%2/index/del").arg(reqip_).arg(reqport_);
    url.setUrl(urlStr);
    QNetworkRequest req;
    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QString timestamp = getTime();
    QString auth_key = calcSign(timestamp,key_);
    QUrlQuery body;
    body.addQueryItem("auth_key",auth_key);
    body.addQueryItem("timestamp",timestamp);
    body.addQueryItem("id",proxyId);
    QNetworkReply* reply =  m_pMessage->post(req,body.toString().toUtf8());
    connect(reply,&QNetworkReply::finished,this,[&](){
        QNetworkReply* reply_ =  (QNetworkReply*)sender();
        reply_->deleteLater();
    });
}

void NPSWrap::slot_deleteClient(QString clientId)
{
    myProcess->close();
    QUrl url;
    QString urlStr = QString("http://%1:%2/client/del").arg(reqip_).arg(reqport_);
    url.setUrl(urlStr);
    QNetworkRequest req;
    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QString timestamp = getTime();
    QString auth_key = calcSign(timestamp,key_);
    QUrlQuery body;
    body.addQueryItem("auth_key",auth_key);
    body.addQueryItem("timestamp",timestamp);
    body.addQueryItem("id",clientId);
    QNetworkReply* reply =  m_pMessage->post(req,body.toString().toUtf8());
    QEventLoop eventLoop;
    QTimer::singleShot(3000,&eventLoop,&QEventLoop::quit);
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    connect(reply,&QNetworkReply::finished,this,[&](){
        QNetworkReply* reply_ =  (QNetworkReply*)sender();
        qDebug() <<  __FILE__ << __LINE__ << reply_->readAll();
        reply_->deleteLater();
    });
    eventLoop.exec();
}

void NPSWrap::slot_getProxyList(QString clientId)
{
    QUrl url;
    QString urlStr = QString("http://%1:%2/index/gettunnel").arg(reqip_).arg(reqport_);
    url.setUrl(urlStr);
    QNetworkRequest req;
    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QString timestamp = getTime();
    QString auth_key = calcSign(timestamp,key_);
    QUrlQuery body;
    body.addQueryItem("auth_key",auth_key);
    body.addQueryItem("timestamp",timestamp);
    body.addQueryItem("client_id",clientId);
    body.addQueryItem("type","tcp");
    body.addQueryItem("search","");
    body.addQueryItem("offset","0");
    body.addQueryItem("limit","1000");
    QNetworkReply* reply =  m_pMessage->post(req,body.toString().toUtf8());
    connect(reply,&QNetworkReply::finished,this,[&](){
        QNetworkReply* reply_ =  (QNetworkReply*)sender();
        QByteArray response = reply_->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        if(doc.isObject()){
            QJsonObject obj = doc.object();
            QJsonValue value =  obj.value("rows");
            if(value.isArray()){
                QJsonArray arr =  value.toArray();
                if(arr.size()){
                    for(int i = 0;i<arr.size();i++){
                        value = arr.at(i);
                        obj = value.toObject();
                        int proxyId = obj.value("Id").toInt();
                        slot_deletePorxy(QString::number(proxyId));
                    }
                }
            }
        }
        reply_->deleteLater();
    });
}


