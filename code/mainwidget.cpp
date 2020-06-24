#include "mainwidget.hpp"
#include "ui_mainwidget.h"

#include "timeconvertwidget.hpp"
#include "jsoneditwidget.hpp"
#include "xmleditwidget.hpp"
#include "portinfoview.hpp"

MainWidget::MainWidget(QWidget* parent)
    : QTabWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    // 添加时间转换
    addTab(new TimeConvertWidget(this), QStringLiteral("时间转换"));
    // 添加JSON处理
    addTab(new JsonEditWidget(this), QStringLiteral("JSON"));
    // 添加XML处理
    addTab(new XmlEditWidget(this),QStringLiteral("XML"));
    // 添加端口使用查看
    addTab(new PortInfoView(this),QStringLiteral("端口"));
}

MainWidget::~MainWidget()
{
    delete ui;
}

