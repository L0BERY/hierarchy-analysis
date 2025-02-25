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

void db::open_table(QVariant table_name){
    model = new QSqlTableModel(this, database);
    model->setTable(table_name.toString());
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

//    qDebug() << model->record();
    emit set_table_signal(model);
}

void db::get_matrix_db(QVariant name){
    QVector<QVector<int>> matrix;
    QSqlQuery cursor(database);
    QString req = "SELECT * FROM ";
    req += name.toString();
    cursor.exec(req);
    cursor.first();
//    QString test = "";
//    test = cursor.value(1).toString();
    while(cursor.next()){
//        qDebug() << cursor.value(1);
    }

//    QSqlRecord rec = cursor.record();
//    qDebug() << test;
}

void db::close_database(){
    database.close();

}

void db::save_model(){
    if(model != nullptr) {
        model->submitAll();
        delete model;
    }
}
