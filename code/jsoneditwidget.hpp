#ifndef JSONEDITWIDGET_HPP
#define JSONEDITWIDGET_HPP

#include <QSplitter>

namespace Ui {
class JsonEditWidget;
}

class JsonEditWidget : public QSplitter {
    Q_OBJECT

public:
    explicit JsonEditWidget(QWidget* parent = nullptr);
    ~JsonEditWidget();

private slots:
    void on_pbtn_Format_clicked();
    void on_pbtn_ShowTree_clicked();

    void on_pbtn_SaveFile_clicked();

    void on_pbtn_LoadFile_clicked();

    void on_pbtn_ToXML_clicked();

private:
    Ui::JsonEditWidget* ui;
};

#endif // JSONEDITWIDGET_HPP
