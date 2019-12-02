#include "code/timeconvertwidget.hpp"
#include "ui_timeconvertwidget.h"

#include <QClipboard>
#include <QDateTime>

TimeConvertWidget::TimeConvertWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::TimeConvertWidget)
{
    ui->setupUi(this);
    // 设置常用时间格式
    QStringList formats;
    QDateTime t = QDateTime::currentDateTime();
    ui->comboBox_Format->addItem(t.toString(Qt::TextDate), QVariant(Qt::TextDate));
    ui->comboBox_Format->addItem(t.toString(Qt::ISODate), QVariant(Qt::ISODate));
    ui->comboBox_Format->addItem(t.toString(Qt::ISODateWithMs), QVariant(Qt::ISODateWithMs));
    ui->comboBox_Format->addItem(t.toString(Qt::SystemLocaleShortDate), QVariant(Qt::SystemLocaleShortDate));
    ui->comboBox_Format->addItem(t.toString(Qt::SystemLocaleLongDate), QVariant(Qt::SystemLocaleLongDate));
    ui->comboBox_Format->addItem(t.toString(Qt::DefaultLocaleShortDate), QVariant(Qt::DefaultLocaleShortDate));
    ui->comboBox_Format->addItem(t.toString(Qt::DefaultLocaleLongDate), QVariant(Qt::DefaultLocaleLongDate));
    ui->comboBox_Format->addItem(t.toString(Qt::SystemLocaleDate), QVariant(Qt::SystemLocaleDate));
    ui->comboBox_Format->addItem(t.toString(Qt::LocaleDate), QVariant(Qt::LocaleDate));
    ui->comboBox_Format->addItem(t.toString(Qt::LocalDate), QVariant(Qt::LocalDate));
    ui->comboBox_Format->addItem(t.toString(Qt::RFC2822Date), QVariant(Qt::RFC2822Date));
    // 设置当前为 2019年12月2日 16:33:17
    ui->comboBox_Format->setCurrentIndex(4);

    // 设置定时器，每秒去刷新当前Unix时间戳
    int timerid = startTimer(1000);
    setProperty("timerid", timerid);
}

TimeConvertWidget::~TimeConvertWidget()
{
    QVariant timerid = property("timerid");
    if(timerid.isValid()) {
        killTimer(timerid.toInt());
    }
    delete ui;
}

void TimeConvertWidget::timerEvent(QTimerEvent* event)
{
    Q_UNUSED(event)
    qint64 uts = QDateTime::currentDateTimeUtc().toSecsSinceEpoch();
    ui->lineEdit_UnixTimestamp->setText(QString::number(uts));
}

// 复制当前Unix时间戳到剪贴板
void TimeConvertWidget::on_pbtn_UnixTimestampCopy_clicked()
{
    QString text = ui->lineEdit_UnixTimestamp->text();
    if(text.isEmpty()) {
        return;
    }

    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setText(text);
}

// 复制转换到的Unix时间戳到剪贴板
void TimeConvertWidget::on_pushButton_InputTimestampCopy_clicked()
{
    QString text = ui->lineEdit_InputTimestamp->text();
    if(text.isEmpty()) {
        return;
    }

    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setText(text);
}

// 复制转换到的时间字符串到剪贴板
void TimeConvertWidget::on_pbtn_NormalTimeCopy_clicked()
{
    QString text = ui->lineEdit_NormalTime->text();
    if(text.isEmpty()) {
        return;
    }

    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setText(text);
}

// 从Unix时间戳转到普通时间
void TimeConvertWidget::on_pbtn_Unix2Normal_clicked()
{
    QString inputtext = ui->lineEdit_InputTimestamp->text();
    if(inputtext.isEmpty() || inputtext == QStringLiteral("当前时间")) {
        inputtext = ui->lineEdit_UnixTimestamp->text();
    }
    double msse = inputtext.toDouble();
    QDateTime t = QDateTime::fromMSecsSinceEpoch(qint64(msse * 1000));
    if(t.isValid()) {
        // 保存一下时间
        setProperty("normaltime", t);
    }
    // 获取时间格式
    Qt::DateFormat f = (Qt::DateFormat)(ui->comboBox_Format->currentData().toInt());
    QString outtext = t.toString(f);

    ui->lineEdit_NormalTime->setText(outtext);
    // 输出历史记录
    ui->textEdit_histroy->append(inputtext + QStringLiteral("  -->  ") + outtext);
}

// 从普通时间转到Unix时间戳
void TimeConvertWidget::on_pbtn_Normal2Unix_clicked()
{
    QString inputtext = ui->lineEdit_NormalTime->text();
    if(inputtext.isEmpty()) {
        return;
    }
    // 获取时间格式
    Qt::DateFormat f = (Qt::DateFormat)(ui->comboBox_Format->currentData().toInt());
    QDateTime t = QDateTime::fromString(inputtext, f);
    // 如果日期无效，则逐个测试是否是时间格式问题
    if(!t.isValid()) {
        int itemcount = ui->comboBox_Format->count();
        for(int i = 0; i < itemcount; ++i) {
            f = (Qt::DateFormat)(ui->comboBox_Format->itemData(i).toInt());
            t = QDateTime::fromString(inputtext, f);
            if(t.isValid()) {
                // 设置当前格式
                ui->comboBox_Format->setCurrentIndex(i);
                // 保存一下时间
                setProperty("normaltime", t);
                break;
            }
        }
    }
    // 无效时间,添加到日志显示
    if(!t.isValid()) {
        ui->textEdit_histroy->append(inputtext);
        ui->textEdit_histroy->append(QStringLiteral(" 时间无效\n"));
        return;
    }
    QString outtext;
    qint64 msse = t.toMSecsSinceEpoch();
    if(msse % 1000 == 0) {
        outtext.setNum(msse / 1000);
    } else {
        outtext.setNum(double(msse) * 0.001, 'f', 3);
    }
    ui->lineEdit_InputTimestamp->setText(outtext);
    // 输出历史记录
    ui->textEdit_histroy->append(inputtext + QStringLiteral("  -->  ") + outtext);
}

// 完成编辑的时候，主动去判断时间格式
void TimeConvertWidget::on_lineEdit_NormalTime_editingFinished()
{
    QString inputtext = ui->lineEdit_NormalTime->text();
    if(inputtext.isEmpty()) {
        return;
    }
    // 获取时间格式
    Qt::DateFormat f = (Qt::DateFormat)(ui->comboBox_Format->currentData().toInt());
    QDateTime t = QDateTime::fromString(inputtext, f);
    if(t.isValid()) {
        // 保存一下时间
        setProperty("normaltime", t);
        return;
    }
    // 如果日期无效，则逐个测试是否是时间格式问题
    int itemcount = ui->comboBox_Format->count();
    for(int i = 0; i < itemcount; ++i) {
        f = (Qt::DateFormat)(ui->comboBox_Format->itemData(i).toInt());
        t = QDateTime::fromString(inputtext, f);
        if(t.isValid()) {
            // 设置当前格式
            ui->comboBox_Format->setCurrentIndex(i);
            // 保存一下时间
            setProperty("normaltime", t);
            break;
        }
    }
}

// 格式改变，切换格式
void TimeConvertWidget::on_comboBox_Format_currentIndexChanged(int index)
{
    QVariant nt = property("normaltime");
    if(!nt.isValid()) {
        return;
    }
    QDateTime t = nt.toDateTime();
    if(t.isValid()) {
        Qt::DateFormat f = (Qt::DateFormat)(ui->comboBox_Format->itemData(index).toInt());
        QString outtext = t.toString(f);
        ui->lineEdit_NormalTime->setText(outtext);
    }
}
