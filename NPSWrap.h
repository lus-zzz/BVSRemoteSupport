#ifndef NPSWRAP_H
#define NPSWRAP_H
#include <QNetworkAccessManager>
#include <QObject>
#include <QNetworkReply>
#include <QProcess>

class NPSWrap : public QObject
{
    Q_OBJECT
public:
    explicit NPSWrap(QObject *parent = nullptr);
    ~NPSWrap();
     QString getTime();
     QString calcSign(QString timestamp, QString key);


signals:
    void signal_getClientId(QString id);
    void signal_getCliendIdByVkey(QString reqip, QString reqport, QString key);
    void signal_addPorxy(QString clientId,QString localPort,QString remark);
    void signal_addSuccess(QString text);
    void signal_deleteClient(QString clientId);


public slots:
    void slot_addClient(QString reqip, QString reqport, QString key);
    void slot_addPorxy(QString clientId,QString localPort,QString remark);
    void slot_getCliendIdByVkey(QString reqip, QString reqport, QString key);
    void slot_deletePorxy(QString proxyId);
    void slot_deleteClient(QString clientId);
    void slot_getProxyList(QString clientId);



private:
    QNetworkAccessManager* m_pMessage;
    QString VerifyKey_;
    QString reqip_;
    QString reqport_;
    QString key_;
    int serverPort;
    QString clientId_;
    QProcess *myProcess;
    QMap<QNetworkReply*,QPair<QString,QString>> proxyInfo;

signals:

};

#endif // NPSWRAP_H
