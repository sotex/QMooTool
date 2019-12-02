#ifndef TIMECONVERTWIDGET_HPP
#define TIMECONVERTWIDGET_HPP

#include <QWidget>

namespace Ui {
class TimeConvertWidget;
}

class TimeConvertWidget : public QWidget {
    Q_OBJECT

public:
    explicit TimeConvertWidget(QWidget* parent = nullptr);
    ~TimeConvertWidget();

    // 定时器触发事件
    void timerEvent(QTimerEvent* event);
private slots:
    // 复制当前Unix时间戳到剪贴板
    void on_pbtn_UnixTimestampCopy_clicked();
    // 复制转换到的Unix时间戳到剪贴板
    void on_pushButton_InputTimestampCopy_clicked();
    // 复制转换到的时间字符串到剪贴板
    void on_pbtn_NormalTimeCopy_clicked();

    void on_pbtn_Unix2Normal_clicked();

    void on_lineEdit_NormalTime_editingFinished();

    void on_pbtn_Normal2Unix_clicked();

    void on_comboBox_Format_currentIndexChanged(int index);

private:
    Ui::TimeConvertWidget* ui;
};

#endif // TIMECONVERTWIDGET_HPP
