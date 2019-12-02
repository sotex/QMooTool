#include "mainwidget.hpp"
#include "ui_mainwidget.h"

#include "timeconvertwidget.hpp"

MainWidget::MainWidget(QWidget* parent)
    : QTabWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    // 添加时间转换
    addTab(new TimeConvertWidget(this), QStringLiteral("时间转换"));
}

MainWidget::~MainWidget()
{
    delete ui;
}

