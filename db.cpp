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
//    qDebug() << table_names;
    count_tables = database.tables().count();
    emit set_table_variant_signal(table_names);
    return true;
}

void db::reset_table(QVariant table_name){
    model = new QSqlTableModel(this, database);
    model->setTable(table_name.toString());
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    if(table_name.toString() == "Варианты"){
        this->column = model->columnCount();
        this->row = model->rowCount();
//        qDebug() << this->column;
//        qDebug() << this->row;
    }
}

void db::open_table(){
    emit set_table_signal(model);
}

QString db::get_name_var(int i){
    QSqlQuery cursor(database);
    cursor.exec("SELECT * FROM Варианты");
    cursor.first();
    for(int j = 0; j < i; j++) cursor.next();
    return cursor.value(1).toString();
}

QStringList db::get_name_vars(){
    QSqlQuery cursor(database);
    cursor.exec("SELECT * FROM Варианты");
    QStringList var;
    cursor.first();
    var.append(cursor.value(1).toString());
    for(int j = 0; j < this->row - 1; j++) {
        cursor.next();
        var.append(cursor.value(1).toString());
    }
    return var;
}

int db::get_row(){
    return this->row;
}

std::tuple<QVector<QVector<QVector<float>>>, int, int> db::get_matrix_db(){
    QVector<QVector<QVector<float>>> final_matrix;
    QVector<QVector<float>> matrix;
    QVector<float> help_matrix;
    QSqlQuery cursor(database);
    int n = 1, a = 1;
    final_matrix.clear();
    for(int j = 0; j < count_tables; j++){
        matrix.clear();
        help_matrix.clear();
        QString req = "SELECT * FROM ";

        if(j == 0) {
            n = 2;
            a = column;
            req += "Варианты";
        }
        else if(j == 1) {
            n = 1;
            a = column - 1;
            req += "Критерии";
        }
        else{
            n = 1;
            a = row + 1;
            req += table_names[j];
        }
//        qDebug() << row;
        cursor.exec(req);
        cursor.first();
//        qDebug() << cursor.record();
        for(int i = n; i < a; i++){
            help_matrix.push_back(cursor.value(i).toFloat());
        }
        matrix.append(help_matrix);
        while(cursor.next()){
            help_matrix.clear();
            for(int i = n; i < a; i++){
                help_matrix.push_back(cursor.value(i).toFloat());
            }
            matrix.append(help_matrix);
        }
        final_matrix.append(matrix);
    }
//    qDebug() << final_matrix;
//    QSqlRecord rec = cursor.record();
    return std::tuple(final_matrix, row, column - 2);
}

void db::close_database(){
    database.close();

}

void db::save_model(){
    if(model != nullptr) {
        model->submitAll();
    }

}
