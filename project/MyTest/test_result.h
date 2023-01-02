#ifndef CVRANGEFINDER_TEST_RESULT_H_
#define CVRANGEFINDER_TEST_RESULT_H_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct test_result {
    std::string name;
    double average_value;
    double min_value;
    double max_value;
    std::vector<double> all_value;

    test_result() = default;
 
    ~test_result();

    void M2a();  // метод считат max, min и матожидание
    void clear();

private:

    void avgv();  // метод считает матожидание
};

#endif // CVRANGEFINDER_TEST_RESULT_H_
