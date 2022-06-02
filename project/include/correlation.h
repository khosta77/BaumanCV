#ifndef CVRANGEFINDER_CORRELATION_H_
#define CVRANGEFINDER_CORRELATION_H_

#include "CV_two_array.h"
#include <cmath>


namespace correlation {
    class coefficient_Pearson {
    public:
        /** \coefficient_Pearson - Конструктор, вычисляет коэффицент Пирсона
         * \M - матрица с картинкой1
         * \N - матрица с картинкой2
         * */
        coefficient_Pearson(const CV_Array &M, const CV_Array &N) {
            if ((M.getCols() != N.getCols()) && (M.getRows() != N.getRows())) {
                throw std::exception();
            }
            double Rm = mean_square_deviation(M, arithmetic_mean(M));
            double Rn = mean_square_deviation(N, arithmetic_mean(N));
            double Rmn = mean_square_deviation(M, arithmetic_mean(M), N, arithmetic_mean(N));
            this->r = Rmn / (Rm * Rn);
        }

        /** \get_coefficient - Возращает коэффицент Пирсона
         * */
        double get_coefficient() {
            return this->r;
        }

    private:
        double r = 0;  // Коэффициент Пирсона

         /** \arithmetic_mean - возращает среднее значение
          * \M - матрица с картинкой
          * \return - среднее значение суммы всех ячеейк деленное на общее их колличество
          * */
        double arithmetic_mean(const CV_Array &M) {
            double m_ = 0;
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
        double mean_square_deviation(const CV_Array &M, const double m_) {
            double Rm = 0;
            for (size_t i = 0; i < M.getRows(); i++) {
                for (size_t j = 0; j < M.getCols(); j++) {
                    Rm += pow((M(i, j) - m_), 2);
                }
            }
            return sqrt(Rm);
        }

        /** \mean_square_deviation - вычисляет числитель из формулы Пирсона
         * \M - матрица с картинкой1
         * \m_ - Среднее арифметическое1
         * \N - матрица с картинкой2
         * \n_ - Среднее арифметическое2
         * \return - числитель из формулы Пирсона
         * */
        double mean_square_deviation(const CV_Array &M, const double m_, const CV_Array &N, const double n_) {
            double Rmn = 0;
            for (size_t i = 0; i < M.getRows(); i++) {
                for (size_t j = 0; j < M.getCols(); j++) {
                    Rmn += ((M(i, j) - m_) * (N(i, j) - n_));
                }
            }
            return Rmn;
        }
    };

    class coefficient_Spearman {
    public:
        /** \coefficient_Spearman - Конструктор, вычисляет коэффицент Спирмена
         * \M - матрица с картинкой1
         * \N - матрица с картинкой2
         * */
        coefficient_Spearman() {
            if ((M.getCols() != N.getCols()) && (M.getRows() != N.getRows())) {
                throw std::exception();
            }

        }

        /** \get_coefficient - Возращает коэффицент Спирмена
         * */
        double get_coefficient() {
            return this->r;
        }
    private:
        double r = 0;  // Коэффициент Спирмена


    };
};

#endif //CVRANGEFINDER_CORRELATION_H_
