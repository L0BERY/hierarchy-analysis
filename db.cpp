#include "db.h"

db::db() : QObject{}{

}

db::~db(){
    database.close();
}

bool db::openDB(QString file_path){

    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(file_path);
    if(!database.open()) return false;
    table_names = database.tables();

    emit set_table_variant_signal(table_names);

    return true;
}

void db::reset_table(QVariant table_name){
    model = new QSqlTableModel(this, database);
    model->setTable(table_name.toString());
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
}

void db::open_table(){
//    qDebug() << model->record();
    emit set_table_signal(model);
}

void db::get_matrix_db(QVariant name){
    QVector<QVector<float>> matrix;
    QVector<float> help_matrix;
    QSqlQuery cursor(database);
    QString req = "SELECT * FROM ";
    req += name.toString();
    cursor.exec(req);
    cursor.first();

    for(int i = 2; i < model->columnCount(); i++){
        help_matrix.push_back(cursor.value(i).toFloat());
    }
    matrix.append(help_matrix);
    int n = 0;
    while(cursor.next()){
        n++;
        help_matrix.clear();
        for(int i = 2; i < model->columnCount(); i++){
            help_matrix.push_back(cursor.value(i).toFloat());
        }
        matrix.append(help_matrix);
    }

    qDebug() << matrix;
//    QSqlRecord rec = cursor.record();
//    qDebug() << test;
}

void db::close_database(){
    database.close();

}

void db::save_model(){
    if(model != nullptr) {
        model->submitAll();
    }
}
