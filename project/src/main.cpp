#include <iostream>
#include "../include/CV_two_array.h"
#include <fstream>

using namespace std;

int main(int argc, char** argv ) {
    if ( argc != 2 ) {
        cout << "usage: DisplayImage.out <Image_Path>\n";
        return -1;
    }
    CV_Array A(imread(argv[1], 1));
    ofstream fout;
    fout.open("data.txt");
    for (size_t i = 0; i < A.getRows(); i++) {
        for (size_t j = 0; j < A.getCols(); j++) {
            fout << setw(4) << A(i, j);
        }
        fout << endl;
    }
    fout.close();
    return 0;
}
