#include <iostream>
#include <chrono>
#include "../include/Mat.h"
#include "../include/correlation.h"
#include "../include/utils.h"

using namespace std;

int main(int argc, const char** argv) {
    switch (argc) {
        case 1: {
            test();
            break;
        }
        case 2: {
            auto start = std::chrono::steady_clock::now();
            Mat X(argv[1]);
            auto end = std::chrono::steady_clock::now();
            cout << "    Время открытия и конвертации в GRAY: "
                 << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;
            break;
        }
        case 3: {
            Mat X(argv[1]);
            Mat Y(argv[2]);
            correlation::coefficient_Pearson cof(X, Y);
            cout << cof.get_coefficient() << endl;
            break;
        }
        case 4: {
#if 0
            raspicam::RaspiCam Camera;
            auto start = std::chrono::steady_clock::now();
            Mat cam(Camera);
            auto end = std::chrono::steady_clock::now();
            cout << "    Время снимка и конвертации его в GRAY: "
                 << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;
            break;
#endif
            auto start = std::chrono::steady_clock::now();
            system("raspistill -o img.jpg -n -w 640 -h 480 -t 3");
            Mat cam("./img.jpg");
            auto end = std::chrono::steady_clock::now();
            cout << "    Время снимка и конвертации его в GRAY: "
                 << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;
            break;
        }
    }
    return 0;
}
