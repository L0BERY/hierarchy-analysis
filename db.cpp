#include "db.h"

db::db() : QObject{}{

}

db::~db(){
    database.close();
}

bool db::openDB(QString file_path){

    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(file_path);
    if(!database.open()){
        std::cout << "Connection FAILED.", database.lastError().text();
        return false;
    }
    QSqlQuery cursor(database);
    cursor.exec("SELECT * FROM sqlite_master WHERE type='table");
    QSqlRecord rec = cursor.record();
    emit set_table_variant_signal(table_names);

    return true;
}

void db::open_table(QString table_name){
    model = new QSqlTableModel(this, database);
    model->setTable(table_name);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

//    qDebug() << model->record(0);
    emit set_table_signal(model);
}

void db::close_database(){
    model->submitAll();
    database.close();
}
