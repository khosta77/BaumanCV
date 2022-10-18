#include "../include/test.h"
#include "../include/rangefinder.h"

#ifdef CVRANGEFINDER_TEST_H_
#include <chrono>
#endif

using namespace std;

int main( [[maybe_unused]] int argc, [[maybe_unused]] const char** argv) {
    auto start = std::chrono::steady_clock::now();
    rangefinder rf;
    auto dist = rf.distance("./Image/DataSet/");
    cout << "Расстояние до объекта: " << dist << " [мм]" << endl;
    auto end = std::chrono::steady_clock::now();
    std::cout << "Время расчета: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms" << std::endl;
#ifdef CVRANGEFINDER_TEST_H_
    runTest(argc, argv);
#endif
    return 0;
}
