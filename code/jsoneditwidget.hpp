#ifndef JSONEDITWIDGET_HPP
#define JSONEDITWIDGET_HPP

#include <QSplitter>

namespace Ui {
class jsoneditwidget;
}

class jsoneditwidget : public QSplitter {
    Q_OBJECT

public:
    explicit jsoneditwidget(QWidget* parent = nullptr);
    ~jsoneditwidget();

private slots:
    void on_pbtn_Format_clicked();
    void on_pbtn_ShowTree_clicked();

    void on_pbtn_SaveFile_clicked();

    void on_pbtn_LoadFile_clicked();

private:
    Ui::jsoneditwidget* ui;
};

#endif // JSONEDITWIDGET_HPP
