#include "code/jsoneditwidget.hpp"
#include "ui_jsoneditwidget.h"
#include "3rd/QCodeEditor/include/QJSONHighlighter"
#include "3rd/QJsonModel/qjsonmodel.h"

#include <QJsonDocument>
#include <QCborValue>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDebug>

Q_DECLARE_METATYPE(QJsonModel*)


// 处理输入json
static bool processJsonText(QWidget* parent,
                            const QByteArray inputjson, QJsonDocument& outdoc)
{
    // 解析为json document
    QJsonParseError error;
    outdoc = QJsonDocument::fromJson(inputjson, &error);
    if(error.error != QJsonParseError::NoError) {
        // 获取错误位置文本
        // 错误位置前面取30个字符，后面取30个字符
        int raw0 = qMax(0, error.offset - 30);
        int raw1 = qMin(inputjson.size(), error.offset + 30);   // 指向下一个有效字符首字节
        // UTF8字符的第一个字节要么是 0xxx xxxx 要么是 111..0xxx 而不会是 10xxxxxx的
        while(raw0 > (0) && ((inputjson[raw0] & 0xC0) == 0x80 )) {
            --raw0;
        }
        while ((raw1 + 1) < inputjson.size() && ((inputjson[raw1] & (0xC0)) == 0x80 )) {
            ++raw1;
        }
        // 获取错误输出文本
        QString errstr = QString::fromUtf8(inputjson.mid(raw0, raw1 - raw0))
                         + QStringLiteral("\r\n") + error.errorString();
        QMessageBox::critical(parent,
                              QStringLiteral("JOSN格式错误"),
                              errstr,
                              QStringLiteral("确定"));
        return false;
    }
    return true;
}

jsoneditwidget::jsoneditwidget(QWidget* parent) :
    QSplitter(parent),
    ui(new Ui::jsoneditwidget)
{
    ui->setupUi(this);
    //ui->textEdit_JsonText->setAcceptRichText(false);
    // ui->textEdit_JsonText->setDocument()
    ui->treeView->hide();

    // 设置文本编辑框语法高亮
    ui->textEdit_JsonText->setHighlighter(new QJSONHighlighter());

    // json模型
    QJsonModel* model = new QJsonModel;
    ui->treeView->setModel(model);
    setProperty("model", QVariant::fromValue(model));

    // 压缩和格式化是一样的
    connect(ui->pbtn_Compact, &QPushButton::clicked,
            this, &jsoneditwidget::on_pbtn_Format_clicked);
}

jsoneditwidget::~jsoneditwidget()
{
    delete ui;
}

void jsoneditwidget::on_pbtn_Format_clicked()
{
    // 获取输入文本
    QString inputtex = ui->textEdit_JsonText->toPlainText();
    if(inputtex.isEmpty()) {
        return;
    }
    QByteArray inputjson = inputtex.toUtf8();

    QJsonDocument doc;
    if(!processJsonText(this, inputjson, doc)) {
        return;
    }

    QJsonDocument::JsonFormat format = QJsonDocument::Indented;
    if(sender() == ui->pbtn_Compact) {
        format = QJsonDocument::Compact;
    }

    QByteArray outjson = doc.toJson(format);

    // 关闭自动更新，避免界面卡死(测试发现没啥效果)
    ui->textEdit_JsonText->setUpdatesEnabled(false);
    ui->textEdit_JsonText->setText(QString::fromUtf8(outjson));
    ui->textEdit_JsonText->setUpdatesEnabled(true);
}

void jsoneditwidget::on_pbtn_ShowTree_clicked()
{
    // 根据当前按钮标题，判断显示还是隐藏
    if(QStringLiteral("关闭可视化") == ui->pbtn_ShowTree->text()) {
        ui->treeView->hide();
        ui->pbtn_ShowTree->setText(QStringLiteral("可视化查看"));
        return;
    }
    // 获取json文本
    QString inputtex = ui->textEdit_JsonText->toPlainText();
    if(inputtex.isEmpty()) {
        return;
    }
    QByteArray inputjson = inputtex.toUtf8();
    // 加载，如果失败了，则使用 processJsonText 弹出错误提示
    QJsonModel* model = property("model").value<QJsonModel*>();
    if(model->loadJson(inputjson)) {
        ui->treeView->show();
        ui->pbtn_ShowTree->setText(QStringLiteral("关闭可视化"));
        return;
    }
    QJsonDocument doc;
    processJsonText(this, inputjson, doc);
}

void jsoneditwidget::on_pbtn_LoadFile_clicked()
{
    QString filename =
        QFileDialog::getOpenFileName(this,
                                     QStringLiteral("加载文件"),
                                     QString(),
                                     QStringLiteral("json (*.json *.geojson);;cbor (*.cbor);;所有文件 (*.*)"));
    if(filename.isEmpty()) {
        return;
    }
    QFile f(filename);
    if(!f.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this,
                              QStringLiteral("读取文件错误"),
                              f.errorString(),
                              QStringLiteral("确定"));
        return;
    }
    // 读取文件，并进行处理
    QByteArray inputtext = f.readAll();
    QString outtext;
    // 是否为cbor文件
    if(filename.size() > 5 &&
            filename.right(5) == QStringLiteral(".cbor")) {
        QCborParserError e;
        QCborValue cbor = QCborValue::fromCbor(inputtext, &e);
        if(e.error == QCborError::NoError) {
            QJsonValue json = cbor.toJsonValue();
            if(json.isObject()) {
                outtext = QString::fromUtf8(QJsonDocument(json.toObject()).toJson());
            } else if(json.isArray()) {
                outtext = QString::fromUtf8(QJsonDocument(json.toArray()).toJson());
            }        }
    }else{
        QJsonDocument doc;
        if(processJsonText(this, inputtext, doc)) {
            outtext = QString::fromUtf8(inputtext);
        }
    }
    if(outtext.isEmpty()) {
        return;
    }

    // 关闭自动更新，避免界面卡死(测试发现没啥效果)
    ui->textEdit_JsonText->setUpdatesEnabled(false);
    ui->textEdit_JsonText->setText(outtext);
    ui->textEdit_JsonText->setUpdatesEnabled(true);
}

void jsoneditwidget::on_pbtn_SaveFile_clicked()
{
    QByteArray outtext = ui->textEdit_JsonText->toPlainText().toUtf8();
    if(outtext.isEmpty()) {
        return;
    }
    QString savefilename =
        QFileDialog::getSaveFileName(this,
                                     QStringLiteral("保存为文件"),
                                     QString(),
                                     QStringLiteral("json (*.json *.geojson);;cbor (*.cbor);;所有文件 (*.*)"));
    if(savefilename.isEmpty()) {
        return;
    }

    // 是否为cbor文件
    if(savefilename.size() > 5 &&
            savefilename.right(5) == QStringLiteral(".cbor")) {
        QJsonDocument doc;
        if(!processJsonText(this, outtext, doc) || doc.isEmpty()) {
            QMessageBox::warning(this,
                                 QStringLiteral("数据无效"),
                                 QStringLiteral("非有效JSON数据，无法保存为CBOR文件"),
                                 QStringLiteral("确定"));
            return;
        }
        if(doc.isObject()) {
            outtext = QCborValue::fromJsonValue(doc.object()).toCbor();
        } else if(doc.isArray()) {
            outtext = QCborValue::fromJsonValue(doc.array()).toCbor();
        }
    }

    // 写入文件
    QFile f(savefilename);
    if(!f.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this,
                              QStringLiteral("打开文件错误"),
                              f.errorString(),
                              QStringLiteral("确定"));
        return;
    }
    f.write(outtext);
}
