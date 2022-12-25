#ifndef CVRANGEFINDER_MYTEST_H_
#define CVRANGEFINDER_MYTEST_H_

#include <fstream>
#include <iostream>
#include <ostream>
#include <iomanip>
#include <cmath>
#include <unistd.h>

#include "./STime.h"
#include "./test_obj.h"
#include "./test_result.h"
#include "./CPUTime.h"


class MyTest {
    std::vector<test_result> results;  // Результаты тестов
    std::vector<test_obj> tests_methods;  // Переменная в которой хранится вектор тестов

    const size_t TEST_COUNT = 10;  // 10 - для проверки, при тестировании полноценном 100, или выше
    const int SIZE_COLOMN = 10;  // размер колнки, с числом
    const int ROUND = 10000;  // Округление до десятитысячных

    STime start, end;  // Время начала и конца тестирования

    inline double round(const double& value);  // окргуление значение, std метод работал не так как нужно

    // Метод ниже проводит тесты в колличестве TEST_COUNT
    inline void makeOneTestMethod(const size_t& i, test_result& tr, double&, double&, double&);
    // Метод ниже вызывается в цикле тестирования, и в нем как проводятся тесты, так и считаются велечины
    inline void oneTestMethod(const size_t& i, test_result& tr, double&, double&, double&);
    // Метод ниже проводит все тесты
    inline void makeTest();  

    int size_id;      // Максимальный разряд
    int size_string;  // Максимальный размер строки

    inline void determineTheSizeString();
    inline void determineTheSizeId();
    inline void determineTheSize();

    inline void draw_line(std::ostream& os);
    inline void printTitle(std::ostream& os);
    inline void printResults(std::ostream& os);
    inline void output (std::ostream& os); 

public:
    MyTest() : size_id(0), size_string(0) {};
    ~MyTest();

    void add(const std::string& tn, double (*f)());
    void make();
};

#endif // CVRANGEFINDER_MYTEST_H_
