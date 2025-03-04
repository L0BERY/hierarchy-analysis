#ifndef DB_H
#define DB_H
#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QWidget>
#include <QDebug>

class db : public QObject
{
    Q_OBJECT

public:
    db();
    ~db();
    void close_database();
    void save_model();
    bool openDB(QString);
    void open_table();
    void reset_table(QVariant table_name);
    std::tuple<QVector<QVector<QVector<float>>>, int, int> get_matrix_db();
    QString get_name_var(int);

    int count_tables = 0;

signals:
    void set_table_signal(QSqlTableModel *model);
    void set_table_variant_signal(QStringList table_names);

private:
    QSqlTableModel *model = nullptr;
    QSqlDatabase database;
    QStringList table_names;
    int column = 1, row = 1;
};

#endif // DB_H
