#include "mainwindow.h"

MainWindow::MainWindow(QString fileName,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass())
{
    ui->setupUi(this);
    this->resize(1250, 750);
    this->fileName = fileName;
    this->setWindowTitle(this->fileName==""?"lpnotepad":this->fileName);
    //loadAllCoding();
    //getCoding();    // 获取文件编码方式
    ui->actionundo->setEnabled(isChanged);
    ui->actionsetting->setEnabled(false);
    ui->menucoding->setEnabled(false);
    if (this->fileName != "") {
        QFile file(this->fileName);
        if (file.exists()) {
            // 读取文件
            file.open(QIODevice::ReadOnly);
            QByteArray text = file.readAll();
            ui->plainTextEdit->setPlainText(QString(text));
            file.close();
        }
    }

    // 显示与隐藏状态栏
    connect(ui->actionstatusBar, &QAction::changed, [=]() {
        if (ui->actionstatusBar->isChecked()) {
            ui->statusBar->show();
        }
        else {
            ui->statusBar->hide();
        }
        });
    // 自动换行
    connect(ui->actionwrap, &QAction::changed, [=]() {
        if (ui->actionwrap->isChecked()) {
            ui->plainTextEdit->setWordWrapMode(QTextOption::WrapAnywhere);
        }
        else {
            ui->plainTextEdit->setWordWrapMode(QTextOption::NoWrap);
        }
        });
    // 退出
    connect(ui->actionexit, &QAction::triggered, [=]() {close(); });

    // 选择字体
    connect(ui->actionfont, &QAction::triggered, [=]() {
        bool flag;
        QFont font = QFontDialog::getFont(&flag, ui->plainTextEdit->font(), this,"选择字体");
        ui->plainTextEdit->setFont(font);
        });

    // 状态栏显示行号列号
    QLabel *rowColumnLabel = new QLabel(this);
    rowColumnLabel->setText(QString("行 %1, 列 %2").arg(ui->plainTextEdit->textCursor().blockNumber() + 1).arg(ui->plainTextEdit->textCursor().columnNumber() + 1));
    ui->statusBar->addWidget(rowColumnLabel);
    // 实时修改行号列号
    connect(ui->plainTextEdit, &QPlainTextEdit::cursorPositionChanged, [=]() {
        rowColumnLabel->setText(QString("行 %1, 列 %2").arg(ui->plainTextEdit->textCursor().blockNumber() + 1).arg(ui->plainTextEdit->textCursor().columnNumber() + 1));
        });

    // 另存为功能和保存功能
    connect(ui->actionsaveTo, &QAction::triggered, [=]() {
        this->fileName = QFileDialog::getSaveFileName(this, "另存为", this->fileName, "文本文档(*.txt);;所有文件(*.*)");
        if (this->fileName == "") {
            // 用户点击取消
            return;
        }
        this->setWindowTitle(this->fileName == "" ? "lpnotepad" : this->fileName);
        isChanged = false;
        QFile file(this->fileName);
        file.open(QIODevice::WriteOnly);
        QString text = ui->plainTextEdit->toPlainText();
        file.write(text.toUtf8().data());
        file.close();
        });
    connect(ui->actionsave, &QAction::triggered, [=]() {
        QFile file(this->fileName);
        if (!file.exists()) {
        // 文件不存在，调用另存为功能
            ui->actionsaveTo->trigger();
        }
        else {
            file.open(QIODevice::WriteOnly);
            this->setWindowTitle(this->fileName == "" ? "lpnotepad" : this->fileName);
            isChanged = false;
            QString text = ui->plainTextEdit->toPlainText();
            file.write(text.toUtf8().data());
            file.close();
        }
        });

    // 全选功能
    connect(ui->actionall, &QAction::triggered, [=]() {
        ui->plainTextEdit->selectAll();
        });
    
    // 当前时间功能
    connect(ui->actiondatetime, &QAction::triggered, [=]() {
        QDateTime time = QDateTime::currentDateTime();
        ui->plainTextEdit->insertPlainText(time.toString("yyyy-MM-dd hh:mm:ss"));
        });

    // 新建功能
    connect(ui->actionnew, &QAction::triggered, [=]() {
        newMainWindow("");
        });

    // 打开功能
    connect(ui->actionopen, &QAction::triggered, [=]() {
        QString openFileName = QFileDialog::getOpenFileName(this, "打开", this->fileName, "文本文档(*.txt);;所有文件(*.*)");
        if (openFileName != "") {
            if (this->fileName == "" && !isChanged) {
                // 该文档是空白文档，且没有被修改时，不会新建窗口
                this->fileName = openFileName;
                this->setWindowTitle(this->fileName);

                // 读取文件
                QFile file(this->fileName);
                file.open(QIODevice::ReadOnly);
                QByteArray text = file.readAll();
                ui->plainTextEdit->setPlainText(QString(text));
                file.close();
            }
            else {
                newMainWindow(openFileName);
            }
        }
        
        });

    // 撤销功能
    connect(ui->actionundo, &QAction::triggered, [=]() {
        ui->plainTextEdit->undo();
        });
    connect(ui->plainTextEdit, &QPlainTextEdit::undoAvailable, [=](bool available) {
        isChanged = available;
        if (available) {
            this->setWindowTitle(this->fileName == "" ? "lpnotepad*" : this->fileName + "*");
        }
        else {
            this->setWindowTitle(this->fileName == "" ? "lpnotepad" : this->fileName + "");
        }
        ui->actionundo->setEnabled(available);
        });

    // 文字被选中
    connect(ui->plainTextEdit, &QPlainTextEdit::selectionChanged, [=]() {
        bool isSelected = ui->plainTextEdit->textCursor().hasSelection();
        ui->actioncut->setEnabled(isSelected);
        ui->actioncopy->setEnabled(isSelected);
        ui->actiondelete->setEnabled(isSelected);
        });

    // 剪切功能
    connect(ui->actioncut, &QAction::triggered, [=]() {
        ui->plainTextEdit->cut();
        });
    
    // 复制功能
    connect(ui->actioncopy, &QAction::triggered, [=]() {
        ui->plainTextEdit->copy();
        });
    
    // 粘贴功能
    connect(ui->actionpaste, &QAction::triggered, [=]() {
        ui->plainTextEdit->paste();
        });

    // 删除功能
    connect(ui->actiondelete, &QAction::triggered, [=]() {
        ui->plainTextEdit->textCursor().deleteChar();
        });
    
    // 右键功能

    //// 显示编码方式
    //QLabel* codingLabel = new QLabel(coding,this);
    //ui->statusBar->addPermanentWidget(codingLabel);
    

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // 拦截退出
    if (isChanged) {
        QMessageBox* closeBox = new QMessageBox;
        closeBox->setWindowTitle("lpnotepad");
        closeBox->setText("您已修改文档，是否保存？");
        closeBox->setIcon(QMessageBox::Question);
        closeBox->setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        closeBox->setDefaultButton(QMessageBox::Save);
        closeBox->setButtonText(QMessageBox::Save, "保存");
        closeBox->setButtonText(QMessageBox::Discard, "放弃");
        closeBox->setButtonText(QMessageBox::Cancel, "取消");
        int select = closeBox->exec();
        switch (select) {
        case QMessageBox::Save:
            ui->actionsave->trigger();
            if (isChanged) {
                // 点击了取消
                event->ignore();
            }
            break;
        case QMessageBox::Discard:
            event->accept();
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
        }

    }
}

void MainWindow::newMainWindow(QString fileName, QWidget* parent) {
    MainWindow* m=new MainWindow(fileName, parent);
    // 移动窗口
    int moveX = this->x() + 30;
    int moveY = this->y() + 30;
    if (moveX + this->width() > QApplication::primaryScreen()->availableSize().width()) {
        moveX = 30;
    }
    if (moveY + this->height() > QApplication::primaryScreen()->availableSize().height()) {
        moveY = 30;
    }
    m->move(moveX, moveY);
    
    m->show();
}

///// <summary>
///// 获取文档的编码方式，默认为UTF-8，也可以在菜单栏上修改
///// 支持：UTF-8、GBK、ISO-8859-1、UTF-16 LE、UTF-16 BE、GB2312
///// </summary>
//void MainWindow::getCoding()
//{
//    if (this->fileName == "") {
//        coding = "UTF-8";
//    }
//    coding = "UTF-8";
//}
//
///// <summary>
///// 改变文件编码方式
///// </summary>
///// <param name="newCoding"></param>
//void MainWindow::changeCoding(QString newCoding)
//{
//    actionMappercoding[this->coding]->setChecked(false);
//    // 将文本转化为utf-8字节数组
//    QByteArray textUtf_8Array = ui->plainTextEdit->toPlainText().toUtf8();
//    QString newText;
//    // 遍历编码方式
//    if (newCoding == "GBK") {
//        newText = QString::fromUtf8()
//    }
//
//    // actionMappercoding[newCoding]->setChecked(true);
//    //for (QMap<QString, QAction*>::iterator it = actionMappercoding.begin(); it != actionMappercoding.end(); it++) {
//    //    
//    //}
//}
//
//// 加载所有编码方式，并保存至map中
//void MainWindow::loadAllCoding()
//{
//    actionMappercoding.insert("UTF-8", ui->actionUTF_8);
//    actionMappercoding.insert("GBK",ui->actionGBK);
//    actionMappercoding.insert("ISO-8859-1",ui->actionISO_8859_1);
//    actionMappercoding.insert("UTF-16 LE",ui->actionUTF_16_LE);
//    actionMappercoding.insert("UTF-16 BE",ui->actionUTF_16_BE);
//    actionMappercoding.insert("GB2312",ui->actionGB2312);
//}
