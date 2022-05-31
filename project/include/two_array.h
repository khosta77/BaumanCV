#ifndef TWO_DIMENSIONAL_ARRAY_TWO_ARRAY_H_
#define TWO_DIMENSIONAL_ARRAY_TWO_ARRAY_H_

#include <istream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <exception>

namespace TwoDimensionalArray {
    /*
     * Array - Класс двумерного массив через одномерный, чтобы было проще работать с матрицами.
     * */
    class Array {
    public:
        double *matrix;   /* Основной массив - он же матрица, т.к. двумерный */
        size_t cols;      /* Колонки */
        size_t rows;      /* Строки */
    public:
        //// Constructor/destructor

        /** \Array конструтор выделяющий память под массив
         * \rows - строки
         * \cols - колонки
         * */
        explicit Array(size_t rows = 0, size_t cols = 0);
        /** \Array - конструтор копирования
         * \arr - дрйгой объект класса Array
         * */
        Array(const Array& arr);
        /** \= - перегрузка опрератора =
         * \arr - дрйгой объект класса Array
         * \return - *this
         * */
        Array& operator=(const Array& arr);
        /** \Деструктор - очищает память
         * */
        ~Array();

        // Basic operations

        /** \getRows - получить колличество строк
         * \return - вернет rows
         * */
        size_t getRows() const;
        /** \getCols - получить колличество колонок
         * \return - вернет cols
         * */
        size_t getCols() const;

        /** \() - перегрузка оператора ()
         * \i - строка
         * \j - колонка
         * \return - вернет ячейку
         * */
        double operator()(size_t i, size_t j) const;
        double& operator()(size_t i, size_t j);
    };

    // Constructor/destructor
    Array::Array(size_t rows, size_t cols) {
        this->rows = rows;
        this->cols = cols;
        this->matrix = new double[this->rows * this->cols];
        for (size_t i = 0; i < this->rows * this->cols; i++) {
            this->matrix[i] = 0;
        }
    }

    Array::Array(const Array& arr) {
        this->rows = arr.getRows();
        this->cols = arr.getCols();
        this->matrix = new double[this->rows * this->cols];
        for (size_t i = 0; i < this->rows; i++) {
            for (size_t j = 0; j < this->cols; j++) {
                this->matrix[j + i * this->cols] = arr.matrix[j + i * arr.cols];
            }
        }
    }

    Array &Array::operator=(const Array& arr) {
        if (&arr != this) {
            delete[] this->matrix;
            this->rows = arr.getRows();
            this->cols = arr.getCols();
            this->matrix = new double[this->rows * this->cols];
            for (size_t i = 0; i < this->rows; i++) {
                for (size_t j = 0; j < this->cols; j++) {
                    this->matrix[j + i * this->cols] = arr.matrix[j + i * arr.cols];
                }
            }
        }
        return *this;
    }

    Array::~Array() {
        delete[] this->matrix;
    }

    // Basic operations
    size_t Array::getRows() const {
        return this->rows;
    }

    size_t Array::getCols() const {
        return this->cols;
    }

    double Array::operator()(size_t i, size_t j) const {
        if (i >= this->rows || j >= this->cols) {
            throw std::exception();
        }
        return this->matrix[j + i * this->cols];
    }

    double &Array::operator()(size_t i, size_t j) {
        if (i >= this->rows || j >= this->cols) {
            throw std::exception();
        }
        return (double &) this->matrix[j + i * this->cols];
    }
};


#endif // TWO_DIMENSIONAL_ARRAY_TWO_ARRAY_H_
