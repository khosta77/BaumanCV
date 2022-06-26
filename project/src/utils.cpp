#include "../include/utils.h"

// Был добавлено вычисление коэффициента Спирмена, но работает не корректно. За комитил
void test() {
    vector<string> tst_jpg = {"1" , "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    vector<double> corr;
    vector<double> corr2;
    for (size_t i = 0; i < tst_jpg.size() - 1; i += 2) {
        CV_Array X(imread((PATH + tst_jpg[i] + JPG), 1));
        CV_Array Y(imread((PATH + tst_jpg[i + 1] + JPG), 1));
        correlation::coefficient_Pearson cof(X, Y);
//        correlation::coefficient_Spearman cof2(X, Y);
        corr.push_back(cof.get_coefficient());
//        corr2.push_back(cof2.get_coefficient());
    }
    ofstream fout("test_Pirson_data.txt");
    for (size_t i = 0; i < corr.size(); i++) {
        fout << i << " " << corr[i] << endl;
    }
    fout.close();
//    ofstream fout1("test_Spearman_data.txt");
//    for (size_t i = 0; i < corr2.size(); i++) {
//        fout1 << i << " " << corr2[i] << endl;
//    }
//    fout1.close();
}
