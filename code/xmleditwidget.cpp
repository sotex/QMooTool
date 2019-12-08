#include "code/xmleditwidget.hpp"
#include "ui_xmleditwidget.h"
#include "popcodedialog.hpp"

#include "QXMLHighlighter"
#include "3rd/jsonxmlconvert/xml2json.hpp"

// #include <QDataStream>
// #include <QDomDocument>
#include <QFileDialog>
#include <QMessageBox>

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

XmlEditWidget::XmlEditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XmlEditWidget)
{
    ui->setupUi(this);
    // 设置语法高亮
    ui->textEdit_XmlText->setHighlighter(new QXMLHighlighter());
    ui->textEdit_XmlText->setTabReplace(true);
    ui->textEdit_XmlText->setTabReplaceSize(4);
}

XmlEditWidget::~XmlEditWidget()
{
    delete ui;
}

void XmlEditWidget::on_pbtn_Compact_clicked()
{
    QString inputtext = ui->textEdit_XmlText->toPlainText();

    QByteArray inputxml = inputtext.toUtf8();
    QByteArray outputxml;
    try {
        // 解析xml
        rapidxml::xml_document<char> doc;
        doc.parse<0>(inputxml.data());
        // 输出为字符串
        rapidxml::print(std::back_inserter(outputxml),doc,0);
        ui->textEdit_XmlText->setText(QString::fromUtf8(outputxml));
    } catch (rapidxml::parse_error& e) {
        QMessageBox msgbox(this);
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle(QStringLiteral("XML格式错误"));
        char wherebuffer[256];
        strncpy(wherebuffer,e.where<char>(),255);
        msgbox.setText(QStringLiteral("错误: %1\n位置: %2")
                       .arg(e.what()).arg(wherebuffer));
        msgbox.exec();
        return;
    }


    // QDomDocument doc;
    // QString errmsg;
    // int errline,errcol;
    // if(!doc.setContent(inputtext,true,&errmsg,&errline,&errcol)){
    //     QMessageBox msgbox(this);
    //     msgbox.setIcon(QMessageBox::Critical);
    //     msgbox.setWindowTitle(QStringLiteral("XML格式错误"));
    //     msgbox.setText(QStringLiteral("错误: %1 行 %2 列\n%3")
    //                    .arg(errline).arg(errcol).arg(errmsg));
    //     msgbox.exec();
    //     return;
    // }
    // ui->textEdit_XmlText->setText(doc.toString(4));
}

void XmlEditWidget::on_pbtn_Format_clicked()
{
    QString inputtext = ui->textEdit_XmlText->toPlainText();

    QByteArray inputxml = inputtext.toUtf8();
    QByteArray outputxml;
    try {
        // 解析xml
        rapidxml::xml_document<char> doc;
        doc.parse<0>(inputxml.data());
        // 输出为字符串
        rapidxml::print(std::back_inserter(outputxml),doc,1);
        ui->textEdit_XmlText->setText(QString::fromUtf8(outputxml));
    } catch (rapidxml::parse_error& e) {
        QMessageBox msgbox(this);
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle(QStringLiteral("XML格式错误"));
        char wherebuffer[256];
        strncpy(wherebuffer,e.where<char>(),255);
        msgbox.setText(QStringLiteral("错误: %1\n位置: %2")
                       .arg(e.what()).arg(wherebuffer));
        msgbox.exec();
        return;
    }
}

void XmlEditWidget::on_pbtn_LoadFile_clicked()
{
    QString filename =
            QFileDialog::getOpenFileName(this,
                                         QStringLiteral("加载文件"),
                                         QString(),
                                         QStringLiteral("XML (*.xml);;所有文件 (*.*)"));
    if(filename.isEmpty()) {
        return;
    }
    QFile f(filename);
    if(!f.open(QIODevice::ReadOnly)) {
        QMessageBox msgbox(this);
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle(QStringLiteral("读取文件错误"));
        msgbox.setText(f.errorString());
        msgbox.exec();
        return;
    }
    // 读取文件，并进行处理
    QByteArray inputtext = f.readAll();
    QString outputxml;
    try {
        // 解析xml
        rapidxml::xml_document<char> doc;
        // 解析时候，不修改输入
        const int parse_non_destructive =
                rapidxml::parse_no_string_terminators | rapidxml::parse_no_entity_translation;
        doc.parse<parse_non_destructive>(inputtext.data());
        // 解析没有问题
        outputxml = std::move(inputtext);
    } catch (rapidxml::parse_error& e) {
        QMessageBox msgbox(this);
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle(QStringLiteral("XML格式错误"));
        char wherebuffer[256];
        strncpy(wherebuffer,e.where<char>(),255);
        msgbox.setText(QStringLiteral("错误: %1\n位置: %2")
                       .arg(e.what()).arg(wherebuffer));
        msgbox.exec();
        return;
    }
    if(outputxml.isEmpty()) {
        return;
    }
    ui->textEdit_XmlText->setText(outputxml);
}

void XmlEditWidget::on_pbtn_SaveFile_clicked()
{
    QByteArray outtext = ui->textEdit_XmlText->toPlainText().toUtf8();
    if(outtext.isEmpty()) {
        return;
    }
    QString savefilename =
            QFileDialog::getSaveFileName(this,
                                         QStringLiteral("保存为文件"),
                                         QString(),
                                         QStringLiteral("xml (*.xml);;所有文件 (*.*)"));
    if(savefilename.isEmpty()) {
        return;
    }
    // 写入文件
    QFile f(savefilename);
    if(!f.open(QIODevice::WriteOnly)) {
        QMessageBox msgbox(this);
        msgbox.setIcon(QMessageBox::Critical);
        msgbox.setWindowTitle(QStringLiteral("打开文件错误"));
        msgbox.setText(f.errorString());
        msgbox.exec();
        return;
    }
    f.write(outtext);
}

void XmlEditWidget::on_pbtn_ToJson_clicked()
{
    QByteArray inputxml = ui->textEdit_XmlText->toPlainText().toUtf8();
    if(inputxml.isEmpty()){
        return;
    }
    std::string json = Xml2Json::xml2json(inputxml.data());
    if(json.empty()){
        return;
    }
    QVariant pcdlg = property("PopCodeDialog");
    PopCodeDialog* pDlg = nullptr;
    if(pcdlg.isNull()){
        pDlg =  new PopCodeDialog(this);
        pDlg->setModal(false);
        pDlg->setHighlighter(QStringLiteral("JSON"));
        pcdlg = QVariant::fromValue(pDlg);
        setProperty("PopCodeDialog",pcdlg);
    }else{
        pDlg = pcdlg.value<PopCodeDialog*>();
    }
    pDlg->setText(QString::fromUtf8(json.data(),json.size()));
    pDlg->exec();
}
