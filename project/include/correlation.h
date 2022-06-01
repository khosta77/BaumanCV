#ifndef CVRANGEFINDER_CORRELATION_H_
#define CVRANGEFINDER_CORRELATION_H_

#include "CV_two_array.h"

namespace correlation {
    class coefficient_Pearson {
    public:
        coefficient_Pearson(const CV_Array& M, const CV_Array& N);
        double get_coefficient();
    private:
        double r = 0;
        /** Три метода ниже явлются частью этой формулы:
https://yandex.ru/images/search?pos=4&img_url=https%3A%2F%2Fpresent5.com%2Fpresentation%2F3%2F146909755_397380501.pdf-img%2F146909755_397380501.pdf-18.jpg&text=%D0%BA%D0%BE%D1%8D%D1%84%D1%84%D0%B8%D1%86%D0%B8%D0%B5%D0%BD%D1%82%20%D0%BF%D0%B8%D1%80%D1%81%D0%BE%D0%BD%D0%B0&lr=213&rpt=simage&source=serp
         * */
        double arithmetic_mean(const CV_Array& M);
        double mean_square_deviation(const CV_Array& M, const double m_);
        double mean_square_deviation(const CV_Array& M, const double m_, const CV_Array& N, const double n_);
    };

    coefficient_Pearson::coefficient_Pearson(const CV_Array& M, const CV_Array& N) {
        double Rm = mean_square_deviation(M, arithmetic_mean(M));
        double Rn = mean_square_deviation(N, arithmetic_mean(N));
        double Rmn = mean_square_deviation(M, arithmetic_mean(M), N, arithmetic_mean(N));
        this->r = Rmn/(Rm * Rn);
    }

    double  coefficient_Pearson::get_coefficient() {
        return this->r;
    }

    double coefficient_Pearson::arithmetic_mean(const CV_Array& M) {
        double m_ = 0;
        for (size_t i = 0; i < M.getRows(); i++) {
            for (size_t j = 0; j < M.getCols(); j++) {
                m_ += M(i, j);
            }
        }
        return (m_ / (M.getRows() * M.getCols()));
    }

    double coefficient_Pearson::mean_square_deviation(const CV_Array& M, const double m_) {
        double Rm = 0;
        for (size_t i = 0; i < M.getRows(); i++) {
            for (size_t j = 0; j < M.getCols(); j++) {
                Rm += pow((M(i, j) - m_), 2);
            }
        }
        return sqrt(Rm);
    }

    double coefficient_Pearson::mean_square_deviation(const CV_Array& M, const double m_, const CV_Array& N, const double n_) {
        if ((M.getCols() != N.getCols()) && (M.getRows() != N.getRows())) {
            throw std::exception();
        }
        double Rmn = 0;
        for (size_t i = 0; i < M.getRows(); i++) {
            for (size_t j = 0; j < M.getCols(); j++) {
                Rmn += ((M(i, j) - m_) * (N(i, j) - n_));
            }
        }
        return Rmn;
    }
};

#endif //CVRANGEFINDER_CORRELATION_H_
