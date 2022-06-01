#include <iostream>
#include "../include/CV_two_array.h"
#include "../include/correlation.h"
#include "utils.cpp"

using namespace std;

int main(int argc, const char** argv) {
    switch (argc) {
        case 1: {
            test();
            break;
        }
        case 3: {
            CV_Array X(imread(argv[1], 1));
            CV_Array Y(imread(argv[2], 1));
            correlation::coefficient_Pearson cof(X, Y);
            cout << cof.get_coefficient() << endl;
            break;
        }
    }
    return 0;
}
