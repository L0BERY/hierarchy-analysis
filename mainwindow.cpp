#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->choice_table->setEnabled(false);
    ui->tableView->hide();
    ui->widget->hide();

    this->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

    anim_text = new QPropertyAnimation(ui->text_state);
    grEffect = new QGraphicsOpacityEffect(ui->text_state);
    ui->text_state->setGraphicsEffect(grEffect);
    anim_text->setTargetObject(grEffect);
    anim_text->setPropertyName("opacity");

    anim_drag_table = new QPropertyAnimation(ui->tableView, "pos");
    anim_drag_table->setDuration(400);
    anim_drag_table->setEasingCurve(QEasingCurve::OutQuart);

    anim_drag_widget = new QPropertyAnimation(ui->widget, "pos");
    anim_drag_widget->setDuration(400);
    anim_drag_widget->setEasingCurve(QEasingCurve::OutQuart);

    anim_size_table = new QPropertyAnimation(ui->tableView, "size");
    anim_size_table->setDuration(400);
    anim_size_table->setEasingCurve(QEasingCurve::OutQuart);

    anim_size_widget = new QPropertyAnimation(ui->widget, "size");
    anim_size_widget->setDuration(400);
    anim_size_widget->setEasingCurve(QEasingCurve::OutQuart);

    pos_widget = ui->widget->pos();
    start_point_widget = ui->but_file_path->pos();
    size_widget = ui->widget->size();

    start_but_pos = ui->but_show_db->pos();

    anim_group_widget = new QParallelAnimationGroup(this);
    anim_group_widget->addAnimation(anim_drag_widget);
    anim_group_widget->addAnimation(anim_size_widget);

    anim_group = new QParallelAnimationGroup(this);
    anim_group->addAnimation(anim_drag_table);
    anim_group->addAnimation(anim_size_table);

    connect(database, &db::set_table_signal, this, &MainWindow::set_table_slot);
    connect(database, &db::set_table_variant_signal, this, &MainWindow::set_table_variant_slot);
    connect(calc, &calculate::set_otvet_signal, this, &MainWindow::set_otvet_slot);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_but_file_path_clicked(){
    database->close_database();
    ui->label->setText("");
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

    auto anim_widget = [this](){
        anim_drag_widget->setStartValue(pos_widget);
        anim_drag_widget->setEndValue(QPoint(0, 70));
        anim_size_widget->setStartValue(size_table);
        anim_size_widget->setEndValue(QSize(1, 1));
        anim_group_widget->start();
    };

    ui->choice_table->clear();
    set_anim(anim_text, 1.0, 0);

    QString path = ui->text_file_path->toPlainText();
    QFileInfo file(path);
    if(!(file.exists() && file.isFile())){

        ui_helper("Файл не существует");
        set_anim(anim_text, 0.0, 1000);
        if(file_path != "") anim_widget();
        file_path = "";
    }
    else if(file.suffix() != "db"){

        ui_helper("Неверный формат файла");
        set_anim(anim_text, 0.0, 1000);
        if(file_path != "") anim_widget();
        file_path = "";
    }
    else {
        if(!is_open) {
            ui->but_start->setEnabled(true);
            ui->choice_table->setEnabled(true);
        }
        ui->but_show_db->setEnabled(true);
        ui->text_state->setStyleSheet("color : green");
        ui->text_state->setText("Файл найден");
        set_anim(anim_text, 0.0, 1000);
        ui->widget->show();
        if(file_path == ""){

            anim_drag_widget->setStartValue(QPoint(0, 70));
            anim_drag_widget->setEndValue(pos_widget);
            anim_size_widget->setStartValue(QSize(1, 1));
            anim_size_widget->setEndValue(size_widget);
            anim_group_widget->start();
        }
        file_path = path;
        if(!database->openDB(file_path)) exit(1);
    }
}

void MainWindow::on_but_show_db_clicked(){
    ui->label->setText("");
    is_open = !is_open;
    if (is_open){
        ui->but_start->setEnabled(false);
        ui->choice_table->setEnabled(false);
        anim_drag_table->setStartValue(QPoint(start_but_pos.x() + ui->but_show_db->width() + 20, start_but_pos.y() + 90));
        anim_drag_table->setEndValue(pos_tableView);
        anim_size_table->setStartValue(QSize(1, 1));
        anim_size_table->setEndValue(size_table);
        ui->but_show_db->setText("Закрыть таблицу");
        database->reset_table(ui->choice_table->currentText());
        database->open_table();
        ui->tableView->show();
        anim_group->start();
    }
    else{
        ui->but_start->setEnabled(true);
        ui->choice_table->setEnabled(true);
        anim_drag_table->setStartValue(pos_tableView);
        anim_drag_table->setEndValue(QPoint(start_but_pos.x() + ui->but_show_db->width() + 20, start_but_pos.y() + 90));
        anim_size_table->setStartValue(size_table);
        anim_size_table->setEndValue(QSize(1, 1));
        anim_group->start();
        ui->but_show_db->setText("Открыть таблицу");
        database->save_model();
    }
}

void MainWindow::on_but_start_clicked(){
    calc->set_matrix(database->get_matrix_db());
    calc->calc();
}

void MainWindow::set_table_variant_slot(QStringList table_names){
    for(int i = 0; i < table_names.length(); i++){
        ui->choice_table->addItem(table_names[i]);
    }
}

void MainWindow::set_table_slot(QSqlTableModel *model){
    ui->tableView->setModel(model);
    if(ui->choice_table->currentText() == "Варианты"){
        ui->tableView->hideColumn(0);
        ui->tableView->showColumn(model->columnCount() - 1);
    }
    else {
        ui->tableView->hideColumn(model->columnCount() - 1);
        ui->tableView->showColumn(0);
    }
}

void MainWindow::set_otvet_slot(int i, float a){
    ui->label->setText("Лучший вариант: "+database->get_name_var(i));
}

void MainWindow::on_choice_table_currentTextChanged(const QString &arg1){
    database->reset_table(arg1);
}

