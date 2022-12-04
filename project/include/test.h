#ifndef CVRANGEFINDER_TEST_H_
#define CVRANGEFINDER_TEST_H_

#include "Mat.h"
#include "correlation.h"
#include "rangefinder.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include "CPUTime.h"


class MyTest {
    typedef void (*func)();

    struct test_obj {
        std::string name;
        func foo_ptr;

        test_obj(const std::string& n, func fPtr) : name(n), foo_ptr(fPtr) {} 
    };

    struct test_result {
        std::string name;
        double average_value;
        double min_value;
        double max_value;
        std::vector<double> all_value;
    };

    static std::vector<test_result> results;  // Результаты тестов
    static std::vector<test_obj> tests_methods;  // Переменная в которой хранится вектор тестов

    static const size_t TEST_COUNT = 10;

    static void draw_line(const int& id, const int& n);
public:
    MyTest() = default;
    ~MyTest() = default;

    static void add(const std::string& tn, func tfPtr);

    // TODO: радикольно уменьшить размер этой функции, вынеся ее куски в отдельные методы 
    static void make();
};

void test_time();

#endif // CVRANGEFINDER_TEST_H_
