#ifndef CVRANGEFINDER_CORRELATION_H_
#define CVRANGEFINDER_CORRELATION_H_

#include "Mat.h"
#include <cmath>
#include <iostream>


namespace correlation {
    class coefficient_Pearson {
    public:
        /** \coefficient_Pearson - Конструктор, вычисляет коэффицент Пирсона
         * \M - матрица с картинкой1
         * \N - матрица с картинкой2
         * */
        coefficient_Pearson(const Mat &M, const Mat &N) {
            if ((M.getCols() != N.getCols()) && (M.getRows() != N.getRows())) {
                throw std::exception();
            }
            float Rm = mean_square_deviation(M, arithmetic_mean(M));
            float Rn = mean_square_deviation(N, arithmetic_mean(N));
            float Rmn = mean_square_deviation(M, arithmetic_mean(M), N, arithmetic_mean(N));
            r = (Rmn / (Rm * Rn));
        }

        /** \get_coefficient - Возращает коэффицент Пирсона
         * */
        float get_coefficient() {
            return r;
        }

    private:
        float r = 0;  // Коэффициент Пирсона

         /** \arithmetic_mean - возращает среднее значение
          * \M - матрица с картинкой
          * \return - среднее значение суммы всех ячеейк деленное на общее их колличество
          * */
         float arithmetic_mean(const Mat &M) {
             float m_ = 0;
             for (size_t i = 0; i < M.getRows(); i++) {
                 for (size_t j = 0; j < M.getCols(); j++) {
                     m_ += M(i, j);
                 }
             }
             return (m_ / (M.getRows() * M.getCols()));
         }

        /** \mean_square_deviation - один из знаминателей в формуле Пирсона
         * \M - матрица с картинкой
         * \m_ - Среднее арифметическое
         * \return - один из знаминателей в формуле Пирсона
         * */
        float mean_square_deviation(const Mat &M, cfloat m_) {
            float Rm = 0;
            for (size_t i = 0; i < M.getRows(); i++) {
                for (size_t j = 0; j < M.getCols(); j++) {
                    Rm += std::pow((M(i, j) - m_), 2);
                }
            }
            return std::sqrt(Rm);
        }

        /** \mean_square_deviation - вычисляет числитель из формулы Пирсона
         * \M - матрица с картинкой1
         * \m_ - Среднее арифметическое1
         * \N - матрица с картинкой2
         * \n_ - Среднее арифметическое2
         * \return - числитель из формулы Пирсона
         * */
        float mean_square_deviation(const Mat &M, cfloat m_, const Mat &N, cfloat n_) {
            float Rmn = 0;
            for (size_t i = 0; i < M.getRows(); i++) {
                for (size_t j = 0; j < M.getCols(); j++) {
                    Rmn += ((M(i, j) - m_) * (N(i, j) - n_));
                }
            }
            return Rmn;
        }
    };
};


#endif //CVRANGEFINDER_CORRELATION_H_
