#include <iostream>
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
        case 3: {
            Mat X(argv[1]);
            Mat Y(argv[2]);
            correlation::coefficient_Pearson cof(X, Y);
            cout << cof.get_coefficient() << endl;
            break;
        }
    }
    return 0;
}
