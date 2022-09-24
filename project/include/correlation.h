#ifndef CVRANGEFINDER_CORRELATION_H_
#define CVRANGEFINDER_CORRELATION_H_

#include "Mat.h"
#include <cmath>
#include <iostream>


namespace correlation {

    class coefficient_Pearson {
    private:
        float Rm;
        float Rn;
        float Rmn;
        float Rm_;
        float Rn_;

    public:
        coefficient_Pearson() : Rm(0), Rn(0), Rmn(0), Rm_(0), Rn_(0) {}

        /** \get_coefficient - Возращает коэффицент Пирсона
         * */
        inline float get_coefficient(const Mat &A, const Mat &B) noexcept {
            arithmetic_mean(Rm_, A);
            arithmetic_mean(Rn_, B);
            mean_square_deviation(A, Rm_, Rm);
            mean_square_deviation(B, Rn_, Rn);
            mean_square_deviation(A, B);
            return Rmn / (Rm * Rn);
        }

    private:
         /** \arithmetic_mean - возращает среднее значение
          * */
         inline void arithmetic_mean(float &m_, const Mat &M) const noexcept {
             for (size_t i = 0; i < M.size(); ++i)
                 m_ += M[i];
             m_ /= M.size();
         }

        /** \mean_square_deviation - один из знаминателей в формуле Пирсона
         * */
        void mean_square_deviation(const Mat &M, const float &m_, float &Rm) const noexcept {
            for (size_t i = 0; i < M.size(); ++i) {
                Rm += ((M[i] - m_) * (M[i] - m_));
            }
            Rm = std::sqrt(Rm);
        }

        /** \mean_square_deviation - вычисляет числитель из формулы Пирсона
         * */
        inline void mean_square_deviation(const Mat &M, const Mat &N) noexcept {
            for (size_t i = 0; i < M.size(); ++i) {
                Rmn += ((M[i] - Rm_) * (N[i] - Rn_));
            }
        }
    };
};


#endif //CVRANGEFINDER_CORRELATION_H_
