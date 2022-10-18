#include "../include/test.h"

#ifdef CVRANGEFINDER_TEST_H_
#include "../include/rangefinder.h"
#include <chrono>
#endif

using namespace std;

int main( [[maybe_unused]] int argc, [[maybe_unused]] const char** argv) {
#ifdef CVRANGEFINDER_TEST_H_
    runTest(argc, argv);
#endif
    return 0;
}
