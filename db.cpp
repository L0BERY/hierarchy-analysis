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
//    cursor.exec("SELECT COUNT(*) FROM test");
//    QSqlRecord rec = cursor.record();

    model = new QSqlTableModel(this, database);
    model->setTable("User");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

//    qDebug() << model->record(0);
    emit set_table_signal(model);

    return true;
}

void db::close_database(){
    model->submitAll();
    database.close();
}
