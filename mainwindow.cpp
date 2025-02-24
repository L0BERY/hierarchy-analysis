#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->hide();

    anim_text = new QPropertyAnimation(ui->text_state);
    grEffect = new QGraphicsOpacityEffect(ui->text_state);
    ui->text_state->setGraphicsEffect(grEffect);
    anim_text->setTargetObject(grEffect);
    anim_text->setPropertyName("opacity");

    anim_drag_table = new QPropertyAnimation(ui->tableView, "pos");
    anim_drag_table->setDuration(400);
    anim_drag_table->setEasingCurve(QEasingCurve::OutQuart);

    anim_opacity_table = new QPropertyAnimation(ui->tableView, "opacity");
    anim_opacity_table->setDuration(400);
    anim_opacity_table->setEasingCurve(QEasingCurve::OutQuart);

    anim_size_table = new QPropertyAnimation(ui->tableView, "size");
    anim_size_table->setDuration(400);
    anim_size_table->setEasingCurve(QEasingCurve::OutQuart);

    start_but_pos = ui->but_show_db->pos();
    pos_tableView = ui->tableView->pos();
    size_table = ui->tableView->size();

    anim_group = new QParallelAnimationGroup(this);
    anim_group->addAnimation(anim_drag_table);
    anim_group->addAnimation(anim_opacity_table);
    anim_group->addAnimation(anim_size_table);

    connect(database, &db::set_table_signal, this, &MainWindow::set_table_slot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_but_file_path_clicked()
{
    auto set_anim = [](QPropertyAnimation* anim, float a, int msec){
        anim->setStartValue(1.0);
        anim->setEndValue(a);
        anim->setDuration(msec);
        anim->start();
    };

    set_anim(anim_text, 1.0, 0);

    QString path = ui->text_file_path->toPlainText();
    QFileInfo file(path);
    if(!(file.exists() && file.isFile())){
        if(is_open) on_but_show_db_clicked();
        ui->but_start->setEnabled(false);
        ui->but_show_db->setEnabled(false);
        ui->text_state->setStyleSheet("color : red");
        ui->text_state->setText("Файл не существует");
        set_anim(anim_text, 0.0, 1000);
    }
    else if(file.suffix() != "db"){
        if(is_open) on_but_show_db_clicked();
        ui->but_start->setEnabled(false);
        ui->but_show_db->setEnabled(false);
        ui->text_state->setStyleSheet("color : red");
        ui->text_state->setText("Неверный формат файла");
        set_anim(anim_text, 0.0, 1000);
    }
    else {
        if(!is_open) ui->but_start->setEnabled(true);
        ui->but_show_db->setEnabled(true);
        ui->text_state->setStyleSheet("color : green");
        ui->text_state->setText("Файл найден");
        file_path = path;
        set_anim(anim_text, 0.0, 1000);

    }
}

void MainWindow::on_but_show_db_clicked()
{
    is_open = !is_open;
    if (is_open){
        ui->but_start->setEnabled(false);
        anim_drag_table->setStartValue(QPoint(start_but_pos.x() + ui->but_show_db->width(), start_but_pos.y()));
        anim_drag_table->setEndValue(pos_tableView);

        opacity_table = new QGraphicsOpacityEffect(this);
//        opacity_table->setOpacity(0.0);
        ui->tableView->setGraphicsEffect(opacity_table);

        anim_opacity_table->setTargetObject(opacity_table);
        anim_opacity_table->setPropertyName("opacity");

        anim_opacity_table->setStartValue(0.0);
        anim_opacity_table->setEndValue(1.0);

        anim_size_table->setStartValue(QSize(1, 1));
        anim_size_table->setEndValue(size_table);
        ui->tableView->show();
        anim_group->start();
        ui->but_show_db->setText("Закрыть базу данных");
        if(!database->openDB(file_path)) exit(1);
    }
    else{
        ui->but_start->setEnabled(true);
        anim_drag_table->setStartValue(pos_tableView);
        anim_drag_table->setEndValue(QPoint(start_but_pos.x() + ui->but_show_db->width(), start_but_pos.y()));
        anim_opacity_table->setStartValue(1.0);
        anim_opacity_table->setEndValue(0.0);
        anim_size_table->setStartValue(size_table);
        anim_size_table->setEndValue(QSize(1, 1));
        anim_group->start();
        ui->but_show_db->setText("Открыть базу данных");
        database->close_database();
    }
}

void MainWindow::on_but_start_clicked()
{

}

void MainWindow::set_table_slot(QSqlTableModel *model){
    ui->tableView->setModel(model);
//    ui->tableView->hideColumn(0);
}






