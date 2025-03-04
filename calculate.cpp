#include "calculate.h"

calculate::calculate() : QObject{}
{

}

float calculate::multiply_vector(QVector<float> vect){
    float val = 1;
    for(int i = 0; i < vect.size(); i++){
        val = val * vect[i];
    }
    return val;
}

float calculate::sum_vector(QVector<float> vect){
    float val = 0;
    for(int i = 0; i < vect.size(); i++){
        val += vect[i];
    }
    return val;
}

void calculate::set_matrix(std::tuple<QVector<QVector<QVector<float>>>, int, int> tuple){
    this->matrix_db.clear();
    this->matrix_db = std::get<0>(tuple);
//    qDebug() << this->matrix_db;
    n = std::get<1>(tuple);
    m = std::get<2>(tuple);
}
void calculate::calc(){
    int a = 1;
    //qDebug() << this->matrix_db;
    this->help_matrix.clear();
    this->otvet.clear();
    for(int iter = 1; iter < m+2; iter++){
        this->sum_of_column.clear();
        this->middle_val.clear();
        if(iter == 1) a = m;
        else a = n;

        for(int i = 0; i < a; i++){
            float val = 0;
            for(int j = 0; j < a; j++){
                val += this->matrix_db[iter][j][i];
            }
            this->sum_of_column.append(val);
        }
//        qDebug() << this->matrix_db[iter];
        for(int i = 0; i < a; i++){
            for(int j = 0; j < a; j++){
                this->matrix_db[iter][j][i] = this->matrix_db[iter][j][i] / sum_of_column[i];
            }
        }
//        qDebug() << this->matrix_db[iter];
        for(int i = 0; i < a; i++){
            this->middle_val.append(sum_vector(this->matrix_db[iter][i]) / float(a));
        }
        this->help_matrix.append(middle_val);
//        qDebug() << middle_val;
    }
//    qDebug() << this->help_matrix;

    for(int i = 1; i < n+1; i++){
        float val = 0;
        for(int j = 0; j < m; j++){
            val += this->help_matrix[i][j] * this->help_matrix[0][j];
        }
        this->otvet.append(val);
    }
 //   qDebug() << this->otvet;

    float max = this->otvet[0];
    float index = 0;
    for(int i = 0; i < n; i++){
        if (this->otvet[i] > max){
            max = this->otvet[i];
            index = i;
        }
    }

    emit set_otvet_signal(index, max);
}
