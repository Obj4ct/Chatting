#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QLabel>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_listen_clicked();

    void on_btn_sendMsg_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer * m_server;//服务器的server
    QTcpSocket * m_tcp;//发送的消息指针
    QLabel * m_status;
};
#endif // MAINWINDOW_H
