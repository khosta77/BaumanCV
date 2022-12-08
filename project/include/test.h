#ifndef CVRANGEFINDER_TEST_H_
#define CVRANGEFINDER_TEST_H_

#include "Mat.h"
#include "correlation.h"
#include "rangefinder.h"
#include "CPUTime.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <unistd.h>
#include <ctime>
#include <chrono>


struct test_obj {
    std::string name;
    double (*foo)();

    test_obj(const std::string& n, double (*f)()) : name(n), foo(f) {} 
};

struct test_result {
    std::string name;
    double average_value;
    double min_value;
    double max_value;
    std::vector<double> all_value;
};

class MyTest {
    std::vector<test_result> results;  // Результаты тестов
    std::vector<test_obj> tests_methods;  // Переменная в которой хранится вектор тестов

    const size_t TEST_COUNT = 10;  // 10 - для проверки, при тестировании полноценном 100, или выше
    const int SIZE_COLOMN = 10;
    const int ROUND = 10000;

    void draw_line(const int& id, const int& n);
    double round(const double& value);
public:
    MyTest() = default;
    ~MyTest() = default;

    void add(const std::string& tn, double (*f)());

    // TODO: радикольно уменьшить размер этой функции, вынеся ее куски в отдельные методы 
    void make();
};


void test_time();

#endif // CVRANGEFINDER_TEST_H_
