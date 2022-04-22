#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <NPSWrap.h>
#include <QLabel>
#include "FileServer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    NPSWrap* m_pNPSWrap;
    QString clientId;
    QLabel statusLabel;
    FileServer* m_pFileServer;

signals:


private slots:
    void on_pushButton_addClient_clicked();
    void on_pushButton_clicked();
    void slot_addSuccess(QString text);
    // QWidget interface
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

protected:
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
