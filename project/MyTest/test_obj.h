#ifndef CVRANGEFINDER_TEST_OBJ_H_
#define CVRANGEFINDER_TEST_OBJ_H_

#include <iostream>
#include <string>


struct test_obj {
    std::string name;
    double (*foo)();

    test_obj(const std::string& n, double (*f)()) : name(n), foo(f) {} 
};

#endif // CVRANGEFINDER_TEST_OBJ_H_
