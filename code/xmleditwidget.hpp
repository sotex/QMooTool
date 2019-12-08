#ifndef XMLEDITWIDGET_HPP
#define XMLEDITWIDGET_HPP

#include <QWidget>

namespace Ui {
class XmlEditWidget;
}

class XmlEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit XmlEditWidget(QWidget *parent = nullptr);
    ~XmlEditWidget();

private slots:
    void on_pbtn_Compact_clicked();

    void on_pbtn_Format_clicked();

    void on_pbtn_LoadFile_clicked();

    void on_pbtn_SaveFile_clicked();

    void on_pbtn_ToJson_clicked();

private:
    Ui::XmlEditWidget *ui;
};

#endif // XMLEDITWIDGET_HPP
