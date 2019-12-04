#include "mainwidget.hpp"
#include "ui_mainwidget.h"

#include "timeconvertwidget.hpp"
#include "jsoneditwidget.hpp"

MainWidget::MainWidget(QWidget* parent)
    : QTabWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    // 添加时间转换
    addTab(new TimeConvertWidget(this), QStringLiteral("时间转换"));
    // 添加JSON处理
    addTab(new jsoneditwidget(this), QStringLiteral("JSON"));
}

MainWidget::~MainWidget()
{
    delete ui;
}

