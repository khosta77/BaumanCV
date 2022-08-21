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

static Mat push_button() {

    pinMode(LedPin, OUTPUT);
    while(1) {
        if (digitalRead(INPUT_PIN) == HIGH) {
            system("raspistill -o image.jpg -t 1");
            Mat mat("./image.jpg");
            system("rm -rf ./image.jpg");
            digitalWrite(LEDPIN_SREEN_COMPLIT, LOW);
            delay(1000);
            digitalWrite(LEDPIN_SREEN_COMPLIT, HIGH);
            delay(1000);
            digitalWrite(LEDPIN_SREEN_COMPLIT, LOW);
            return mat;
        }
    }
}

void test2() {
    if(wiringPiSetup() == -1) {
        cout << "setup wiringPi failed !\n" << endl;
        return;
    }

    struct corr {
        double coef;
        double l;

        corr(double c, double mm) : coef(c), l(mm) {}
    };

    vector<corr> df;
    double t;
//    pinMode(LEDPIN_END, OUTPUT);
    pinMode(LEDPIN_SREEN_COMPLIT, OUTPUT);

    for (size_t i = 0; i < 6; i++) {
        Mat A = push_button();
        Mat B = push_button();
        correlation::coefficient_Pearson cof(A, B);
        cin >> t;
        df.push_back(corr(cof.get_coefficient(), t));
    }

    for (size_t i = 0; i < df.size(); i++) {
        cout << "coefficient: " << df[i].coef << " length: " << df[i].l << endl;
    }

    corr df_max = std::max(std::begin(df), std::end(df), [](corr df1, corr df2){
        return (df1.coef < df2.coef);
    });

    cout << "---> LENGTH: = " << ((1/F) - (1/df_max.l)) << endl;
    cout << "---> MAX COEFFICIENT: = " << df_max.coef << endl;
}
