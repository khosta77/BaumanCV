#ifndef CVRANGEFINDER_TEST_H_
#define CVRANGEFINDER_TEST_H_

#include "Mat.h"
#include "correlation.h"
#include "rangefinder.h"
#include "CPUTime.h"
#include <fstream>
#include <iostream>
#include <ostream>
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

    test_result() = default;
 
    ~test_result() {
        clear();
    }

    inline void M2a() {  // поссчитать max, min, avg
        avgv();
        min_value = *std::min_element(all_value.begin(), all_value.end());
        max_value = *std::max_element(all_value.begin(), all_value.end());
    }

    inline void clear() {
        all_value.clear();
        name.clear();
    }

private:

    inline void avgv() {
        average_value = all_value[0];
        for (size_t i = 1; i < all_value.size(); ++i)  // в отдельный методд
            average_value += all_value[i];
        average_value /= all_value.size();
    }
};

struct STime {
    int h;
    int m;
    int s;

    STime() = default;
    STime(const int&H, const int&M, const int&S) : h(H), m(M), s(S) {}

    inline void getSystemTime() {
        time_t t;
        struct tm *tm_p;
        t = time(NULL);
        tm_p = localtime(&t);
        h  = tm_p->tm_hour;
        m = tm_p->tm_min;
        s = tm_p->tm_sec;
        delete []tm_p;  // вот тут возможноа ошибка
    }

    friend std::ostream &operator<< (std::ostream& os, const STime& st) {
        (st.h <= 9) ? os << "0" << st.h << ":" : os << st.h << ":";
        (st.m <= 9) ? os << "0" << st.m << ":" : os << st.m << ":";
        (st.s <= 9) ? os << "0" << st.s : os << st.s;
        return os;
    }
};


class MyTest {
    std::vector<test_result> results;  // Результаты тестов
    std::vector<test_obj> tests_methods;  // Переменная в которой хранится вектор тестов

    const size_t TEST_COUNT = 10;  // 10 - для проверки, при тестировании полноценном 100, или выше
    const int SIZE_COLOMN = 10;
    const int ROUND = 10000;

    STime start, end;  // Время начала и конца тестирования

    void draw_line(std::ostream& os);
    double round(const double& value);

    // Метод ниже проводит тесты в колличестве TEST_COUNT
    inline void makeOneTestMethod(const size_t& i, test_result& tr, double&, double&, double&);
    // Метод ниже вызывается в цикле тестирования, и в нем как проводятся тесты, так и считаются велечины
    inline void oneTestMethod(const size_t& i, test_result& tr, double&, double&, double&);
    inline void makeTest();

    int size_id;
    int size_string;

    inline void determineTheSizeString() {
        for (size_t i = 0; i < results.size(); ++i)
            if (size_string < int(results[i].name.size()))
                size_string = results[i].name.size();
    }

    inline void determineTheSizeId() {
        for (int i = results.size(); i > 0; i /= 10 )
            ++size_id;
    }

    inline void determineTheSize() const {
        determineTheSizeString();
        determineTheSizeId();
    }

    inline void printTitle(std::ostream& os) const {
        os << std::setw(size_id) << std::left << "N" << "|"
           << std::setw(size_string) << std::left << "name" << "|"
           << std::setw(SIZE_COLOMN) << std::left << "avg, [sec]" << "|"
           << std::setw(SIZE_COLOMN) << std::left << "min, [sec]" << "|"
           << std::setw(SIZE_COLOMN) << std::left << "max, [sec]" << "|" << std::endl;
        draw_line(os);
    }

    inline void printResults(std::ostream& os) const {
        for (size_t i = 0; i < results.size(); ++i) {
            os << std::setw(size_id) << std::right << (i + 1) << "|"
               << std::setw(size_string) << std::left << results[i].name << "|"
               << std::setw(SIZE_COLOMN) << std::right << round(results[i].average_value) << "|"
               << std::setw(SIZE_COLOMN) << round(results[i].min_value) << "|"
               << std::setw(SIZE_COLOMN) << round(results[i].max_value) << "|" << std::endl;
#if 0  // кому-то будет удобно читать таблицу с разгроничением на строки
            draw_line(size_id, size_string);
#endif
        }
        draw_line(os);
    }

    inline void output (std::ostream& os) const { 
        determineTheSize();
        printTitle(os);
        printResults(os);
    }

public:
    MyTest() : size_id(0), size_string(0) {};

    ~MyTest() {
        results.clear();
        tests_methods.clear();
    }

    inline void add(const std::string& tn, double (*f)());

    // TODO: радикольно уменьшить размер этой функции, вынеся ее куски в отдельные методы 
    void make();
};


void test_time();

#endif // CVRANGEFINDER_TEST_H_
