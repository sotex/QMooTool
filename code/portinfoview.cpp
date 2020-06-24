#include "portinfoview.hpp"
#include "ui_portinfoview.h"
#include <QStandardItemModel>
#include <QDebug>

#ifdef WIN32
// https://docs.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-gettcptable
// Need to link with Iphlpapi.lib and Ws2_32.lib
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#endif

PortInfoView::PortInfoView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PortInfoView)
{
    ui->setupUi(this);
    // 数据模型
    QStandardItemModel* model = new QStandardItemModel(this);
    /*设置列字段名*/
    model->setColumnCount(6);
    model->setHeaderData(0,Qt::Horizontal, QStringLiteral("状态"));
    model->setHeaderData(1,Qt::Horizontal, QStringLiteral("本地IP:Port"));
    model->setHeaderData(2,Qt::Horizontal, QStringLiteral("远程IP:Port"));
    model->setHeaderData(3,Qt::Horizontal, QStringLiteral("进程名"));
    model->setHeaderData(4,Qt::Horizontal, QStringLiteral("PID"));
    model->setHeaderData(5,Qt::Horizontal, QStringLiteral("进程路径"));
    /*设置行字段名*/
    // model->setRowCount(1);
    //    model->setHeaderData(0,Qt::Vertical, QStringLiteral("记录一"));
    //    model->setHeaderData(1,Qt::Vertical, QStringLiteral("记录二"));
    //    model->setHeaderData(2,Qt::Vertical, QStringLiteral("记录三"));
    /*设置一条数据*/
    // model->setItem(0, 0, new QStandardItem(QStringLiteral("张三")));
    // model->setItem(0, 1, new QStandardItem(QStringLiteral("3")));
    // model->setItem(0, 2, new QStandardItem(QStringLiteral("男")));
    ui->tableView->setModel(model);

}

PortInfoView::~PortInfoView()
{
    delete ui;
}

void PortInfoView::on_pbtn_refresh_clicked()
{
#ifdef WIN32
    // 分配内存，保存 TCP 列表
    QByteArray buffer(sizeof(MIB_TCPTABLE)*65536,'\0');
    PMIB_TCPTABLE pTcpTable = reinterpret_cast<MIB_TCPTABLE*>(buffer.data());

    ULONG dwSize = static_cast<ULONG>(buffer.size());
    DWORD dwRetVal = GetTcpTable(pTcpTable,&dwSize,TRUE);
    if(dwRetVal == ERROR_INSUFFICIENT_BUFFER){
        qDebug()<<"dwSize = "<<dwSize;
        buffer.resize(static_cast<int>(dwSize));
        PMIB_TCPTABLE pTcpTable = reinterpret_cast<MIB_TCPTABLE*>(buffer.data());
        dwRetVal = GetTcpTable(pTcpTable, &dwSize, TRUE);
    }
    if (dwRetVal == NO_ERROR) {
        qDebug()<<"\tNumber of entries: "<< pTcpTable->dwNumEntries;
        QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(ui->tableView->model());
        model->setRowCount(static_cast<int>(pTcpTable->dwNumEntries));
        for (int i = 0; i < (int) pTcpTable->dwNumEntries; i++) {
            struct in_addr IpAddr;
            char szLocalAddr[128];
            char szRemoteAddr[128];
            IpAddr.S_un.S_addr = (u_long) pTcpTable->table[i].dwLocalAddr;
            strcpy_s(szLocalAddr, sizeof (szLocalAddr), inet_ntoa(IpAddr));
            IpAddr.S_un.S_addr = (u_long) pTcpTable->table[i].dwRemoteAddr;
            strcpy_s(szRemoteAddr, sizeof (szRemoteAddr), inet_ntoa(IpAddr));


            // 状态、本地地址、远程地址、进程名、PID、进程路径
            qDebug()<<"TCP["<<i<<"] State:"<<pTcpTable->table[i].dwState;
            QString state;
            switch (pTcpTable->table[i].dwState) {
            case MIB_TCP_STATE_CLOSED:
                state = QStringLiteral("关闭");
                break;
            case MIB_TCP_STATE_LISTEN:
                state = QStringLiteral("监听");
                break;
            case MIB_TCP_STATE_SYN_SENT:
                state = QStringLiteral("请求连接");
                break;
            case MIB_TCP_STATE_SYN_RCVD:
                state = QStringLiteral("SYN-RECEIVED");
                break;
            case MIB_TCP_STATE_ESTAB:
                state = QStringLiteral("连接成功");
                break;
            case MIB_TCP_STATE_FIN_WAIT1:
                state = QStringLiteral("FIN-WAIT-1");
                break;
            case MIB_TCP_STATE_FIN_WAIT2:
                state = QStringLiteral("FIN-WAIT-2");
                break;
            case MIB_TCP_STATE_CLOSE_WAIT:
                state = QStringLiteral("CLOSE-WAIT");
                break;
            case MIB_TCP_STATE_CLOSING:
                state = QStringLiteral("CLOSING");
                break;
            case MIB_TCP_STATE_LAST_ACK:
                state = QStringLiteral("LAST-ACK");
                break;
            case MIB_TCP_STATE_TIME_WAIT:
                state = QStringLiteral("TIME-WAIT");
                break;
            case MIB_TCP_STATE_DELETE_TCB:
                state = QStringLiteral("DELETE-TCB");
                break;
            default:
                state = QStringLiteral("未知状态");
                break;
            }

            model->setItem(i,0,new QStandardItem(state));
            model->setItem(i,1,new QStandardItem(QString("%1:%2").arg(szLocalAddr).arg(pTcpTable->table[i].dwLocalPort)));
            model->setItem(i,2,new QStandardItem(QString("%1:%2").arg(szRemoteAddr).arg(pTcpTable->table[i].dwRemotePort)));
            model->setItem(i,3,new QStandardItem(state));
            model->setItem(i,4,new QStandardItem(state));
            model->setItem(i,5,new QStandardItem(state));
        }
    } else {
        qDebug()<<"\tGetTcpTable failed with "<< dwRetVal;
        return;
    }
#endif
}
