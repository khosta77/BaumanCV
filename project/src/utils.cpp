#include "../include/utils.h"

void test() {
    vector<string> tst_jpg = {"1" , "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    vector<double> corr;
    for (size_t i = 0; i < tst_jpg.size() - 1; i += 2) {
        Mat X((PATH + tst_jpg[i] + JPG));
        Mat Y((PATH + tst_jpg[i + 1] + JPG));
        correlation::coefficient_Pearson cof(X, Y);
        corr.push_back(cof.get_coefficient());
    }
    ofstream fout("test_Pirson_data.txt");
    for (size_t i = 0; i < corr.size(); i++) {
        fout << i << " " << abs(corr[i]) << endl;
    }
    fout.close();
}

void test2() {
//    for ()
}
