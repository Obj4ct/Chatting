#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostAddress>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_port->setText("8899");
    ui->lineEdit_ip->setText("127.0.0.1");
    setWindowTitle("客户端");
    ui->btn_disconnect->setDisabled(true);
    //创建监听的服务器对象
    m_tcp=new QTcpSocket;
    connect(m_tcp,&QTcpSocket::readyRead,this,[=](){//检测有没有服务端发送数据
        QByteArray data=m_tcp->readAll();
        ui->textEdit_historyMsg->append("服务器说: "+data);
    });
    connect(m_tcp,&QTcpSocket::disconnected,this,[=](){//客户端断开连接
        m_tcp->close();
        m_tcp->deleteLater();//delete 自我销毁
        m_status->setPixmap(QPixmap(":/ico/failed.png").scaled(20,20));
        ui->textEdit_historyMsg->append("断开连接！");
        ui->btn_connect->setDisabled(false);
        ui->btn_disconnect->setDisabled(false);
    });
    connect(m_tcp,&QTcpSocket::connected,this,[=](){//客户端连接服务器成功
         m_status->setPixmap(QPixmap(":/ico/success.png").scaled(20,20));
         ui->textEdit_historyMsg->append("连接成功！");
         ui->btn_connect->setDisabled(true);
         ui->btn_disconnect->setDisabled(false);
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



//发送消息按钮
void MainWindow::on_btn_sendMsg_clicked()
{
    QString msg=ui->textEdit_sendMsg->toPlainText();
    m_tcp->write(msg.toUtf8());
    ui->textEdit_historyMsg->append("客户端说:"+msg);
}

//连接服务器
void MainWindow::on_btn_connect_clicked()
{
    QString ip=ui->lineEdit_ip->text();
    unsigned short port=ui->lineEdit_port->text().toUShort();
    m_tcp->connectToHost(QHostAddress(ip),port);//第一个参数是ip地址

}


void MainWindow::on_btn_disconnect_clicked()
{
    m_tcp->close();
    ui->btn_connect->setDisabled(false);
    ui->btn_disconnect->setDisabled(false);
}

