#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <QTabWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QTabWidget {
    Q_OBJECT

public:
    MainWidget(QWidget* parent = nullptr);
    ~MainWidget();

private:
    Ui::MainWidget* ui;
};
#endif // MAINWIDGET_HPP
