#ifndef CALCULATE_H
#define CALCULATE_H

#include <QObject>
#include <QVector>

class calculate : public QObject
{
    Q_OBJECT
public:
    calculate(QObject *parent);
    void fill_matrix_db();

private:
    int n = 0;  //строки
    int m = 0;  //столбцы
    QVector<QVector <int>> matrix_db;
};

#endif // CALCULATE_H
