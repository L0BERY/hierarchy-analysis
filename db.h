#ifndef DB_H
#define DB_H
#pragma once

#include <iostream>
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
    bool openDB(QString);
    void open_table(QString);

signals:
    void set_table_signal(QSqlTableModel *model);
    void set_table_variant_signal(QStringList table_names);

private:
    QSqlTableModel *model;
    QSqlDatabase database;
    QStringList table_names;
};

#endif // DB_H
