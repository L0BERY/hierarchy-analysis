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
#include "calculate.h"

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
    void on_choice_table_currentTextChanged(const QString &arg1);


public slots:
    void set_table_slot(QSqlTableModel *model);
    void set_table_variant_slot(QStringList table_names);
    void set_otvet_slot(int, float, QVector<float>*);

private:
    Ui::MainWindow *ui;
    db *database = new db;
    calculate *calc = new calculate;
    bool is_open = false, otvet_show = false;
    void close_table_otvet();
    QPoint start_but_pos, start_point_widget, start_table_otvet, end_val_table_otvet = QPoint(20, 230);
    QPoint pos_tableView = QPoint(250, 90), pos_widget;
    QPropertyAnimation *anim_text;
    QPropertyAnimation *anim_drag_table, *anim_opacity_table, *anim_size_table;
    QPropertyAnimation *anim_drag_widget, *anim_size_widget;
    QPropertyAnimation *anim_drag_table_otvet, *anim_size_table_otvet;

    QSize size_table = QSize(681, 521), size_widget, size_table_otvet = QSize(231, 261);
    QParallelAnimationGroup *anim_group, *anim_group_widget, *anim_group_table_otvet;
    QGraphicsOpacityEffect *grEffect;
//    QGraphicsOpacityEffect *opacity_table;
    QString file_path = "";
};
#endif // MAINWINDOW_H
