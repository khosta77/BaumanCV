#define CHECK 0
#if CHECK
#include "../include/test.h"

#ifdef CVRANGEFINDER_TEST_H_
#include "../include/rangefinder.h"
#include <chrono>

using namespace std;
#endif
#else
#include "iostream"
#include "string"

using namespace std;

class father {
public:
    std::string i = "100";

    [[nodiscard]] virtual inline std::string i_print() const {
        return this->i;
    }

    void print() const {
        cout << this->i_print() << endl;
    }
};

class child1 : public father {
public:
    std::string i;
    [[nodiscard]] inline  std::string i_print() const override {
        return this->i;
    }
    child1(std::string s) : i(s) {}
};
#endif

int main( [[maybe_unused]] int argc, [[maybe_unused]] const char** argv) {
#if CHECK
#ifdef CVRANGEFINDER_TEST_H_
    runTest(argc, argv);
#endif
#else
    child1 ch;
//    child1 *myClass = nullptr;
//    myClass->print();
    ch.print();
#endif
    return 0;
}
