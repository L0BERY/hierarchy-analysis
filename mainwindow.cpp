#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->hide();
    ui->choice_table->setEnabled(false);

    anim_text = new QPropertyAnimation(ui->text_state);
    grEffect = new QGraphicsOpacityEffect(ui->text_state);
    ui->text_state->setGraphicsEffect(grEffect);
    anim_text->setTargetObject(grEffect);
    anim_text->setPropertyName("opacity");

    anim_drag_table = new QPropertyAnimation(ui->tableView, "pos");
    anim_drag_table->setDuration(400);
    anim_drag_table->setEasingCurve(QEasingCurve::OutQuart);

//    anim_opacity_table = new QPropertyAnimation(ui->tableView);
//    anim_opacity_table->setTargetObject(ui->tableView);
//    anim_opacity_table->setPropertyName("windowOpacity");
//    anim_opacity_table->setEasingCurve(QEasingCurve::OutQuart);
//    opacity_table = new QGraphicsOpacityEffect(ui->tableView);
//    ui->tableView->setGraphicsEffect(opacity_table);
//    anim_opacity_table->setDuration(400);

    anim_size_table = new QPropertyAnimation(ui->tableView, "size");
    anim_size_table->setDuration(400);
    anim_size_table->setEasingCurve(QEasingCurve::OutQuart);

    start_but_pos = ui->but_show_db->pos();
    pos_tableView = ui->tableView->pos();
    size_table = ui->tableView->size();

    anim_group = new QParallelAnimationGroup(this);
    anim_group->addAnimation(anim_drag_table);
//    anim_group->addAnimation(anim_opacity_table);
    anim_group->addAnimation(anim_size_table);

    connect(database, &db::set_table_signal, this, &MainWindow::set_table_slot);
    connect(database, &db::set_table_variant_signal, this, &MainWindow::set_table_variant_slot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_but_file_path_clicked()
{
    database->close_database();

    auto set_anim = [](QPropertyAnimation* anim, float a, int msec){
        anim->setStartValue(1.0);
        anim->setEndValue(a);
        anim->setDuration(msec);
        anim->start();
    };

    auto ui_helper = [this](QString text){
        if(is_open) on_but_show_db_clicked();
        ui->but_start->setEnabled(false);
        ui->but_show_db->setEnabled(false);
        ui->choice_table->setEnabled(false);
        ui->text_state->setStyleSheet("color : red");
        ui->text_state->setText(text);
    };

    ui->choice_table->clear();
    set_anim(anim_text, 1.0, 0);

    QString path = ui->text_file_path->toPlainText();
    QFileInfo file(path);
    if(!(file.exists() && file.isFile())){
        ui_helper("Файл не существует");
        set_anim(anim_text, 0.0, 1000);
    }
    else if(file.suffix() != "db"){
        ui_helper("Неверный формат файла");
        set_anim(anim_text, 0.0, 1000);
    }
    else {
        if(!is_open) {
            ui->but_start->setEnabled(true);
            ui->choice_table->setEnabled(true);
            if(!database->openDB(file_path)) exit(1);
        }
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
        ui->choice_table->setEnabled(false);
        anim_drag_table->setStartValue(QPoint(start_but_pos.x() + ui->but_show_db->width(), start_but_pos.y()));
        anim_drag_table->setEndValue(pos_tableView);
//        opacity_table->setOpacity(0.0);
//        anim_opacity_table->setStartValue(0.0);
//        anim_opacity_table->setEndValue(1.0);
        anim_size_table->setStartValue(QSize(1, 1));
        anim_size_table->setEndValue(size_table);
        ui->tableView->show();
        anim_group->start();
        ui->but_show_db->setText("Закрыть таблицу");
        database->open_table(ui->choice_table->currentText());
    }
    else{
        ui->but_start->setEnabled(true);
        ui->choice_table->setEnabled(true);
        anim_drag_table->setStartValue(pos_tableView);
        anim_drag_table->setEndValue(QPoint(start_but_pos.x() + ui->but_show_db->width(), start_but_pos.y()));
//        anim_opacity_table->setStartValue(1.0);
//        anim_opacity_table->setEndValue(0.0);
        anim_size_table->setStartValue(size_table);
        anim_size_table->setEndValue(QSize(1, 1));
        anim_group->start();
        ui->but_show_db->setText("Открыть таблицу");
        database->save_model();
    }
}

void MainWindow::on_but_start_clicked()
{
    database->get_matrix_db(ui->choice_table->currentText());
}

void MainWindow::set_table_variant_slot(QStringList table_names){
    for(int i = 0; i < table_names.length(); i++){
        ui->choice_table->addItem(table_names[i]);
    }
}

void MainWindow::set_table_slot(QSqlTableModel *model){
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
}

void MainWindow::on_but_add_clicked()
{

}

