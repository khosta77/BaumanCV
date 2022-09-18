#ifndef TWO_DIMENSIONAL_ARRAY_TWO_ARRAY_H_
#define TWO_DIMENSIONAL_ARRAY_TWO_ARRAY_H_

#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <exception>

namespace sstd {

    namespace se {

        /** \class - Ошибка, для массива
         * */
        class _out_of_range : public std::exception {
            const size_t out_range;
            const size_t range;
        public:
            _out_of_range(const size_t &out_r, const size_t &r = 0) : out_range(out_r), range(r) {}

            virtual const size_t& what() noexcept {
                return out_range;
            }

            virtual void print() noexcept {
                std::cout << "The value goes beyond the bounds of the array: "
                          << range << " < " << out_range << std::endl;
            }

        };

        class _out_of_range_matrix : public std::exception {
            const size_t out_range_x;
            const size_t range_x;
            const size_t out_range_y;
            const size_t range_y;
        public:
            _out_of_range_matrix(const size_t &out_x, const size_t &out_y, const size_t &x = 0,
                                   const size_t &y = 0) : out_range_x(out_x), range_x(x),
                                   out_range_y(out_y), range_y(y) {}

            virtual const size_t& what() noexcept {
                if (range_x < out_range_x) {
                    return out_range_x;
                }
                return out_range_y;
            }

            virtual void print() noexcept {
                std::cout << "The value goes beyond the bounds of the matrix:" << std::endl << "[X] ";
                if (range_x < out_range_x) {
                    std::cout << range_x << " < " << out_range_x << std::endl << "[Y] ";
                } else {
                    std::cout << "OK" << std::endl << "[Y] ";
                }
                if (range_y < out_range_y) {
                    std::cout << range_y << " < " << out_range_y << std::endl;
                } else {
                    std::cout << "OK" << std::endl;
                }
            }
        };

        /** \class - Ошибка, при отстуствие имени сохранения
         * */
        class _without_file : public std::exception {
            const std::string error;
        public:
            _without_file() {}

            virtual void print() noexcept {
                std::cout << "The object being saved does not have a name" << std::endl;
            }
        };

    };  // Stepan Exception

    /*
     * SMatrix - Класс двумерного массив через одномерный, чтобы было проще работать с матрицами.
     * */
    template<typename T>
    class SMatrix {
    protected:
        size_t rows;      /* Строки */
        size_t cols;      /* Колонки */
        T *matrix;   /* Основной массив - он же матрица, т.к. двумерный */
    public:
//------------------------------------------------------------------------------------------------------------
        //// Constructor/destructor
        /** \Array конструтор выделяющий память под массив
         * \rows - строки
         * \cols - колонки
         * */
        SMatrix(const size_t cols = 0, const size_t rows = 0) : rows(rows), cols(cols) {
            matrix = new T[rows * cols]{};
        }
        /** \Array - конструтор копирования
         * \arr - дрйгой объект класса Array
         * */
        SMatrix(const SMatrix& arr) {
            rows = arr.getRows();
            cols = arr.getCols();
            matrix = new T[rows * cols]{};
            for (size_t i = 0; i < (rows * cols); ++i) {
                matrix[i] = arr.matrix[i];
            }
        }
        /** \= - перегрузка опрератора =
         * \arr - дрйгой объект класса Array
         * \return - *this
         * */
        SMatrix& operator=(const SMatrix& arr) {
            if (&arr != this) {
                delete[] this->matrix;
                rows = arr.getRows();
                cols = arr.getCols();
                matrix = new T[rows * cols]{};
                for (size_t i = 0; i < (rows * cols); ++i) {
                    matrix[i] = arr.matrix[i];
                }
            }
            return *this;
        }
        /** \Деструктор - очищает память
         * */
        ~SMatrix() {
            delete[] matrix;
        }
//------------------------------------------------------------------------------------------------------------
        // Basic operations
        /** \getRows - получить колличество строк
         * \return - вернет rows
         * */
        inline size_t getRows() const noexcept {
            return rows;
        }

        /** \getCols - получить колличество колонок
         * \return - вернет cols
         * */
        inline size_t getCols() const noexcept {
            return cols;
        }

        /** \() - перегрузка оператора ()
         * \i - строка
         * \j - колонка
         * \return - вернет ячейку
         * */
        T operator()(const size_t &i, const size_t &j) const {
            if (i >= rows || j >= cols) {
                throw se::_out_of_range_matrix(j, i, cols, rows);
            }
            return matrix[j + i * cols];
        }

        T& operator()(const size_t &i, const size_t &j) {
            if (i >= rows || j >= cols) {
                throw se::_out_of_range_matrix(j, i, cols, rows);
            }
            return (T&)matrix[j + i * cols];
        }

        T& operator[](const size_t &k) noexcept { return matrix[k]; }

        T& operator[](const size_t &k) const noexcept { return matrix[k]; }
//------------------------------------------------------------------------------------------------------------
        void resize(size_t rows = 0, size_t cols = 0) {
            delete[] matrix;
            this->rows = rows;
            this->cols = cols;
            matrix = new T[rows * cols]{};
        }
    };

};  // sstd


#endif // TWO_DIMENSIONAL_ARRAY_TWO_ARRAY_H_
