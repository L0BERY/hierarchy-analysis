#ifndef CALCULATE_H
#define CALCULATE_H
#pragma once

#include <QObject>
#include <QVector>
#include <QDebug>
#include <cmath>

class calculate : public QObject
{
    Q_OBJECT
public:
    calculate();
    void fill_matrix_db();
    void set_matrix(std::tuple<QVector<QVector<QVector<float>>>, int, int>);
    void calc();

signals:
    void set_otvet_signal(int, float);

private:
    int n = 0;  //строки (варианты)
    int m = 0;  //столбцы (критерии)
    QVector<float> sum_of_column, middle_val, otvet;
    // QVector<QVector<QVector <float>>> all_grade_and_param;
    QVector<QVector<float>> help_matrix;
    QVector<QVector<QVector <float>>> matrix_db;
    float multiply_vector(QVector<float>);
    float sum_vector(QVector<float>);
};

#endif // CALCULATE_H
