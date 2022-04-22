#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pNPSWrap = new NPSWrap(this);
    clientId = "";
    m_pFileServer = nullptr;
    connect(m_pNPSWrap,&NPSWrap::signal_getClientId,this,[&](QString id){
        clientId = id;
    });
    connect(m_pNPSWrap,&NPSWrap::signal_addSuccess,this,&MainWindow::slot_addSuccess);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_addClient_clicked()
{
    m_pNPSWrap->slot_addClient(ui->lineEdit_ip->text(),ui->lineEdit_port->text(),ui->lineEdit_key->text());
}


void MainWindow::on_pushButton_clicked()
{
    if(!clientId.isEmpty()){
        emit m_pNPSWrap->signal_addPorxy(clientId,"3389","RemoteDesktop");
    }
}

void MainWindow::slot_addSuccess(QString text)
{

    ui->textEdit->append(text);
//    QMessageBox::information(this,"提示",text);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    m_pNPSWrap->slot_deleteClient(clientId);
    QMainWindow::closeEvent(event);
}


void MainWindow::on_pushButton_2_clicked()
{
    if(!clientId.isEmpty()){
        QString filename = QFileDialog::getExistingDirectory(this,"选择目录","D:\\");
        if(!filename.isEmpty()){
            m_pFileServer = new FileServer(filename,this);
            if(m_pFileServer->port)
                emit m_pNPSWrap->signal_addPorxy(clientId,QString::number(m_pFileServer->port),"FileServer");
        }
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    static int count = 0;
    QDialog dialog(this);
    QFormLayout form(&dialog);
    // Value1
    QString value1 = QString("主机和端口");
    QLineEdit* lineEdit1 = new QLineEdit(&dialog);
    lineEdit1->setText("127.0.0.1:8899");
    form.addRow(value1, lineEdit1);
    // Value2
    QString value2 = QString("服务描述");
    QLineEdit * lineEdit2 = new QLineEdit(&dialog);
    lineEdit2->setText(QString("自定义服务%1").arg(++count));
    form.addRow(value2, lineEdit2);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
        QString text = lineEdit1->text();
        QStringList strList =  text.split(":");
        if (strList.size() == 2){
            emit m_pNPSWrap->signal_addPorxy(clientId,text,lineEdit2->text());
        }

    }

}

