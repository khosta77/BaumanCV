#ifndef CVRANGEFINDER_CORRELATION_H_
#define CVRANGEFINDER_CORRELATION_H_

#include "CV_two_array.h"
#include <cmath>
#include <iostream>


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


    /* НЕ РАБОТАЕТ КАК НАДО
     *По формуле как понятно, надо выбрать ранк элементов, но правильность его вычесления вызывает вопросы.
     *Не понял как ранк вычислить - если коротко. Получается мы работает с массивом...
     *Не не могу словами сказать.
     * */
    class coefficient_Spearman {
    public:
        /** \coefficient_Spearman - Конструктор, вычисляет коэффицент ранговой корреляции Спирмена
         * \M - матрица с картинкой1
         * \N - матрица с картинкой2
         * */
        coefficient_Spearman(const CV_Array &M, const CV_Array &N) {
            if ((M.getCols() != N.getCols()) && (M.getRows() != N.getRows())) {
                throw std::exception();
            }
            double sd = sum_of_differences(M, N);
            int n = (M.getRows() * M.getCols());
            double zn = pow(n, 3) - n;
            std::cout << sd << " " << n << " " << zn << std::endl;
            this->rs = (1 - ((6*sd)/zn));
        }

        /** \get_coefficient - Возращает коэффицент Спирмена
         * */
        double get_coefficient() {
            return this->rs;
        }
    private:
        double rs = 0;  // Коэффициент Спирмена

        /** \sum_of_differences - числитель в формуле Спримена
         * \M - матрица с картинкой1
         * \N - матрица с картинкой2
         * \return - числитель в формуле Спримена
         * */
        double sum_of_differences(const CV_Array &M, const CV_Array &N) {
            double sd = 0;
            for (size_t i = 0; i < M.getRows(); i++) {
                for (size_t j = 0; j < M.getCols(); j++) {
                    sd += pow((M(i, j) - N(i, j)), 2);
                }
            }
            return sd;
        }

    };

// Не уверен стоит ли их добоовлять но пусть будут. Правда они по логике такие же, вплане ранжирования.
//    Это точно то?
    // Коэффицент ранговой корреляции Кендалла

    // Коэффицент ранговой корреляции Гудмена-Краскела
};

#endif //CVRANGEFINDER_CORRELATION_H_
