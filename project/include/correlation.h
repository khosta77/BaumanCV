#ifndef CVRANGEFINDER_CORRELATION_H_
#define CVRANGEFINDER_CORRELATION_H_

#include "Mat.h"
#include <cmath>
#include <iostream>


namespace correlation {

    class coefficient_Pearson {
    public:
        coefficient_Pearson() = default;

        /** \get_coefficient - Возращает коэффицент Пирсона
         * */
        inline float Pearson(const Mat &A, const Mat &B) noexcept {
            float Rm = 0, Rn = 0, Rmn = 0, Rm_ = 0, Rn_ = 0;
            arithmetic_mean(Rm_, A);
            arithmetic_mean(Rn_, B);
            mean_square_deviation(A, Rm_, Rm);
            mean_square_deviation(B, Rn_, Rn);
            mean_square_deviation(A, B, Rmn, Rm_, Rn_);
            return Rmn / (Rm * Rn);
        }

    private:
         /** \arithmetic_mean - возращает среднее значение
          * */
         inline void arithmetic_mean(float &m_, const Mat &M) const noexcept {
             for (size_t i = 0; i < M.size() - 10; i+=10) { // Все размеры снимков кратны 10
                 m_ += M[i];
                 m_ += M[i + 1];
                 m_ += M[i + 2];
                 m_ += M[i + 3];
                 m_ += M[i + 4];
                 m_ += M[i + 5];
                 m_ += M[i + 6];
                 m_ += M[i + 7];
                 m_ += M[i + 8];
                 m_ += M[i + 9];
             }
             m_ /= M.size();
         }

        /** \mean_square_deviation - один из знаминателей в формуле Пирсона
         * */
        void mean_square_deviation(const Mat &M, const float &m_, float &Rm) const noexcept {
            for (size_t i = 0; i < M.size() - 10; i+=10) {
                Rm += ((M[i] - m_) * (M[i] - m_));
                Rm += ((M[i + 1] - m_) * (M[i + 1] - m_));
                Rm += ((M[i + 2] - m_) * (M[i + 2] - m_));
                Rm += ((M[i + 3] - m_) * (M[i + 3] - m_));
                Rm += ((M[i + 4] - m_) * (M[i + 4] - m_));
                Rm += ((M[i + 5] - m_) * (M[i + 5] - m_));
                Rm += ((M[i + 6] - m_) * (M[i + 6] - m_));
                Rm += ((M[i + 7] - m_) * (M[i + 7] - m_));
                Rm += ((M[i + 8] - m_) * (M[i + 8] - m_));
                Rm += ((M[i + 9] - m_) * (M[i + 9] - m_));
            }
            Rm = std::sqrt(Rm);
        }

        /** \mean_square_deviation - вычисляет числитель из формулы Пирсона
         * */
        inline void mean_square_deviation(const Mat &M, const Mat &N, float &Rmn, const float &Rm_,  const float &Rn_) noexcept {
            for (size_t i = 0; i < M.size() - 10; i+=10) {
                Rmn += ((M[i] - Rm_) * (N[i] - Rn_));
                Rmn += ((M[i + 1] - Rm_) * (N[i + 1] - Rn_));
                Rmn += ((M[i + 2] - Rm_) * (N[i + 2] - Rn_));
                Rmn += ((M[i + 3] - Rm_) * (N[i + 3] - Rn_));
                Rmn += ((M[i + 4] - Rm_) * (N[i + 4] - Rn_));
                Rmn += ((M[i + 5] - Rm_) * (M[i + 5] - Rn_));
                Rmn += ((M[i + 6] - Rm_) * (M[i + 6] - Rn_));
                Rmn += ((M[i + 7] - Rm_) * (M[i + 7] - Rn_));
                Rmn += ((M[i + 8] - Rm_) * (M[i + 8] - Rn_));
                Rmn += ((M[i + 9] - Rm_) * (M[i + 9] - Rn_));
            }
        }
    };
};


#endif //CVRANGEFINDER_CORRELATION_H_
