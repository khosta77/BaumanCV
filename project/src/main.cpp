#include <iostream>
#include <chrono>
#include "../include/Mat.h"
#include "../include/correlation.h"
#include "../include/test.h"

using namespace std;

void runTest(int argc, const char **argv);

int main(int argc, const char** argv) {
    runTest(argc, argv);
    return 0;
}

void runTest(int argc, const char **argv) {
    if (argc != 2)
        return;
    switch (getTestCode(string(argv[1]))) {
        case IDENTICAL_PHOTOS: {
            identicalPhotosTest();
            break;
        }
        case DEFFERENT_PHOTOS: {
            defferentPhotosTest();
            break;
        }
        case DEFFERENT_ANGLES: {
            defferentAngelsTest();
            break;
        }
        case BLUR: {
            blurTest();
            break;
        }
        case CORRELATION_TIME: {
            correlationTimeTest();
            break;
        }
        case RGB_TO_GRAY: {
            rgbToGrayTest();
            break;
        }
#ifdef CAMERA_TEST
        case SNAPSHOT_TIME: {
            break;
        }
        case SNAPSHOT_TIME_TO_GRAY: {
            break;
        }
#endif
        default: {
            cout << "Тест не распознан!" << endl;
        }
    }
}