#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QFontDialog>
#include <QLabel>
#include <QFileDialog>
#include <QStyle>
#include <QApplication>
#include <QMap>
#include <QRandomGenerator>
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; };
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString fileName,QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent* event) override;
    // void keyPressEvent(QKeyEvent* event) override;

private:
    Ui::MainWindowClass* ui;
    void newMainWindow(QString fileName, QWidget* parent = nullptr);
    QString fileName;       // 文件名称
    bool isChanged=false;   // 表示文件是否被改变，需要保存
    //QString coding;         // 文件编码方式
    //void getCoding();
    //void changeCoding(QString newCoding);
    //QMap<QString,QAction*> actionMappercoding;     // 菜单栏上编码QAction对应的编码方式
    //void loadAllCoding();
};
