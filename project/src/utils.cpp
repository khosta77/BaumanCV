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

void test3() {
    vector<string> tst_jpg = {"1" , "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    for (size_t i = 0; i < tst_jpg.size(); ++i) {
        tst_jpg[i] = PATH + tst_jpg[i] + JPG;
    }
    vector<long> time;
    Mat X(tst_jpg[0]);
    for (size_t i = 0; i < tst_jpg.size(); ++i) {
        auto start = std::chrono::steady_clock::now();
        Mat A(tst_jpg[i]);
        auto end = std::chrono::steady_clock::now();
        auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        time.push_back(t);
        std::cout << "    Время открытия и конвертации в GRAY, сохранения: " << t << " ms" << std::endl;
    }
    ofstream fout("test_time_data.txt", ios::out | ios::app);
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    fout << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << std::endl;
    for (size_t i = 0; i < time.size(); i++) {
        fout << (i + 1)<< " " << time[i] <<  std::endl;
    }
    long a = 0;
    for (size_t i = 0; i < time.size(); ++i) {
        a += time[i];
    }
    a = (a/time.size());
    fout << "Среднее значение: " << a << std::endl;
    fout.close();
}

static Mat push_button() {

    pinMode(LEDPIN_SREEN_COMPLIT, OUTPUT);
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

        corr() = default;
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

    corr df_max = df[0];
    for (size_t i = 0; i < df.size(); i++) {
        if (df[i].coef > df_max.coef) {
            df_max = df[i];
        }
    }

    cout << "---> LENGTH: = " << ((1/F) - (1/df_max.l)) << endl;
    cout << "---> MAX COEFFICIENT: = " << df_max.coef << endl;
}
