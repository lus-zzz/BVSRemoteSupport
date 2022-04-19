#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusbar->addWidget(&statusLabel);
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
    statusLabel.setText(text);
    QMessageBox::information(this,"提示",text);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    m_pNPSWrap->slot_deleteClient(clientId);
    QMainWindow::closeEvent(event);
}


void MainWindow::on_pushButton_2_clicked()
{
    if(!clientId.isEmpty()){
        m_pFileServer = new FileServer("8080",this);
        emit m_pNPSWrap->signal_addPorxy(clientId,"8080","FileServer");
    }
}

