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
//    QSqlQuery cursor(database);
//    cursor.exec("SELECT * FROM sqlite_master WHERE type='table'");
//    QSqlRecord rec = cursor.record();

//    qDebug() << database.tables();

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

void db::close_database(){
    model->submitAll();
}
