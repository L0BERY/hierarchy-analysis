#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once

#include <QMainWindow>
#include <QString>
#include <QFileInfo>
#include <QObject>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>

#include "db.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString getFilePath() {return file_path;};


private slots:
    void on_but_file_path_clicked();

    void on_but_start_clicked();

    void on_but_show_db_clicked();

public slots:
    void set_table_slot(QSqlTableModel *model);
    void set_table_variant_slot(QStringList table_names);


private:
    Ui::MainWindow *ui;
    db *database = new db;
    bool is_open = false;
    QPoint start_but_pos;
    QPoint pos_tableView;
    QPropertyAnimation *anim_text;
    QPropertyAnimation *anim_drag_table;
    QPropertyAnimation *anim_opacity_table;
    QPropertyAnimation *anim_size_table;
    QSize size_table;
    QParallelAnimationGroup *anim_group;
    QGraphicsOpacityEffect *grEffect;
    QGraphicsOpacityEffect *opacity_table;
    QString file_path = "";
};
#endif // MAINWINDOW_H
