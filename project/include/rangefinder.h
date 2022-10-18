#ifndef CVRANGEFINDER_RANGEFINDER_H_
#define CVRANGEFINDER_RANGEFINDER_H_

#include "Mat.h"
#include "correlation.h"
#include "algorithm"
#include <iterator>

class rangefinder {
#define BLOCK_SIZE 36  // Размер ячейки корреляции
#define ANGLE_CUTOFF 655  // Подобрано эксперементально, для того чтобы правильно считать угол
#define BC 70.0  // Расстояние между камерами
    correlation::coefficient_Pearson K;
    std::vector<double> correlation;
    size_t cols;
    size_t rows;

    std::vector<std::string> tst_jpg = {"1" , "2", "3", "4", "5", "6", "7", "8", "9", "10"};
public:
    rangefinder() = default;
    float distance(const std::string &directory_ds) {
        calculateCorrelation(directory_ds);
        size_t id = getId(correlation, *min_element(std::begin(correlation), std::end(correlation)));
        auto image_correlation = find_all_correlation(directory_ds, id);
        size_t position = getId(image_correlation, *max_element(std::begin(image_correlation), std::end(image_correlation)));
        double  a, b, c, beta, x;
        b = (position * BLOCK_SIZE) - cols / 2;
        a = rows / 2 - ANGLE_CUTOFF;
        c = std::sqrt(std::pow(b, 2) + std::pow(a, 2));
        beta = asin(b / c) * (180.0 / M_PI);
        x = BC * tan(beta * M_PI / 180.0);
        return x;
    }

    ~rangefinder() = default;
private:
    void calculateCorrelation(const std::string &directory_ds) {
        for (size_t i = 0; i < tst_jpg.size(); ++i) {
            Mat X((directory_ds + "ph_" + tst_jpg[i] + "_1.jpg"));
            Mat Y((directory_ds + "ph_" + tst_jpg[i] + "_2.jpg"));
            correlation.push_back(K.Pearson(X, Y));
        }
    }

    size_t getId(const std::vector<double> &vctr, const double &it) {
        for (size_t i = 0; i < vctr.size(); ++i)
            if (vctr[i] == it)
                return i;
        return -1;
    }

    std::vector<double> find_all_correlation(const std::string &directory_ds, const size_t &id) {
        Mat r((directory_ds + "ph_" + tst_jpg[id] + "_1.jpg"));
        Mat l((directory_ds + "ph_" + tst_jpg[id] + "_2.jpg"));
        rows = l.getRows(); cols = l.getCols();
        Mat r_control_block = r.submatrix((r.getRows() / 2 - BLOCK_SIZE / 2),
                                          (r.getRows() / 2 + BLOCK_SIZE / 2),
                                          (r.getCols() / 2 - BLOCK_SIZE / 2),
                                          (r.getCols() / 2 + BLOCK_SIZE / 2));
        std::vector<double> image_correlation;
        for (size_t i = l.getCols() / BLOCK_SIZE; i > 0; --i) {
            Mat l_control_block = l.submatrix((l.getRows() / 2 - BLOCK_SIZE / 2),
                                              (l.getRows() / 2 + BLOCK_SIZE / 2),
                                              ((l.getCols() - (i * BLOCK_SIZE))),
                                              ((l.getCols() - (i * BLOCK_SIZE)) + BLOCK_SIZE));
            image_correlation.push_back(K.Pearson(l_control_block, r_control_block));
        }
        return image_correlation;
    }
};


#endif // CVRANGEFINDER_RANGEFINDER_H_
