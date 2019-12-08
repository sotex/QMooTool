#include "code/popcodedialog.hpp"
#include "ui_popcodedialog.h"

#include "QJSONHighlighter"
#include "QCXXHighlighter"
#include "QXMLHighlighter"

PopCodeDialog::PopCodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PopCodeDialog)
{
    ui->setupUi(this);
    // ui->codeEdit->setTabReplace(true);
    // ui->codeEdit->setTabReplaceSize(4);
}

PopCodeDialog::~PopCodeDialog()
{
    delete ui;
}

void PopCodeDialog::setHighlighter(const QString &langauge)
{
    if(0 == langauge.compare(QStringLiteral("JSON"),Qt::CaseInsensitive)){
        ui->codeEdit->setHighlighter(new QJSONHighlighter());
    }
    else if(0 == langauge.compare(QStringLiteral("XML"),Qt::CaseInsensitive)){
        ui->codeEdit->setHighlighter(new QXMLHighlighter());
    }
    else if(0 == langauge.compare(QStringLiteral("CXX"),Qt::CaseInsensitive)){
        ui->codeEdit->setHighlighter(new QCXXHighlighter());
    }
}

void PopCodeDialog::setText(const QString &text)
{
    ui->codeEdit->setText(text);
}
