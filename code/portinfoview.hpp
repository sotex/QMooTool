#ifndef PORTINFOVIEW_HPP
#define PORTINFOVIEW_HPP

#include <QWidget>

namespace Ui {
class PortInfoView;
}

class PortInfoView : public QWidget
{
    Q_OBJECT

public:
    explicit PortInfoView(QWidget *parent = nullptr);
    ~PortInfoView();

private slots:
    void on_pbtn_refresh_clicked();

private:
    Ui::PortInfoView *ui;
};

#endif // PORTINFOVIEW_HPP
