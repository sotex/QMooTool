#ifndef POPCODEDIALOG_HPP
#define POPCODEDIALOG_HPP

#include <QDialog>

namespace Ui {
class PopCodeDialog;
}

class PopCodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PopCodeDialog(QWidget *parent = nullptr);
    ~PopCodeDialog();

    void setHighlighter(const QString& langauge);

    void setText(const QString& text);
private:
    Ui::PopCodeDialog *ui;
};
Q_DECLARE_OPAQUE_POINTER(PopCodeDialog)

#endif // POPCODEDIALOG_HPP
