#include "../include/test.h"

using namespace std;

int main(int argc, const char** argv) {
#ifdef CVRANGEFINDER_TEST_H_
    runTest(argc, argv);
#endif
    return 0;
}
