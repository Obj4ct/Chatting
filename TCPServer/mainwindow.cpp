#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_port->setText("8899");
    setWindowTitle("服务器");
    //创建监听的服务器对象
    m_server=new QTcpServer(this);
    connect(m_server,&QTcpServer::newConnection,this,[=](){//判断有无客户端连接
        m_tcp=m_server->nextPendingConnection();//创建用于通信的套接字对象,获取连接上的套接字对象
        m_status->setPixmap(QPixmap(":/ico/success.png").scaled(20,20));
        //检测是否可以接收数据
        connect(m_tcp,&QTcpSocket::readyRead,this,[=](){//检测有没有客户端发送数据
            QByteArray data=m_tcp->readAll();
            ui->textEdit_historyMsg->append("客户端说: "+data);
        });
        connect(m_tcp,&QTcpSocket::disconnected,this,[=](){//检测客户端是否断开连接
            m_tcp->close();
            m_tcp->deleteLater();//delete 自我销毁
            m_status->setPixmap(QPixmap(":/ico/failed.png").scaled(20,20));
        });
    });
    //状态栏
    m_status=new QLabel();
    m_status->setPixmap(QPixmap(":/ico/failed.png").scaled(20,20));
    ui->statusbar->addWidget(new QLabel("连接状态: "));
    ui->statusbar->addWidget(m_status);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_listen_clicked()
{
    unsigned short port=ui->lineEdit_port->text().toUShort();
    m_server->listen(QHostAddress::Any,port);
    ui->lineEdit_port->setDisabled(true);
}


void MainWindow::on_btn_sendMsg_clicked()
{
    QString msg=ui->textEdit_sendMsg->toPlainText();
    m_tcp->write(msg.toUtf8());
    ui->textEdit_historyMsg->append("服务端说:"+msg);
}

