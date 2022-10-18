#include "../include/test.h"

using namespace std;

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
        case DISTANSE: {
            printDistance();
            break;
        }
        default: {
            cout << "Тест не распознан!" << endl;
        }
    }
}

int getTestCode(const std::string& cmd) {
    if (cmd == "identicalPhotos") {
        return IDENTICAL_PHOTOS;
    }
    if (cmd == "defferentPhotos") {
        return DEFFERENT_PHOTOS;
    }
    if (cmd == "defferentAngels") {
        return DEFFERENT_ANGLES;
    }
    if (cmd == "blur") {
        return BLUR;
    }
    if (cmd == "correlation") {
        return CORRELATION_TIME;
    }
    if (cmd == "RGBtoGRAY") {
        return RGB_TO_GRAY;
    }
#ifdef CAMERA_TEST
    if (cmd == "snapshot_time") {
        return SNAPSHOT_TIME;
    }
    if (cmd == "snapshot_time_to_gray") {
        return SNAPSHOT_TIME_TO_GRAY;
    }
#endif
    if (cmd == "distance") {
        return DISTANSE;
    }
    return 0;
}

static void outKT(const std::string &l, const std::string &r) {
    Mat Z(l);
    cout << "  Размер фото: " << Z.getCols() << "x" << Z.getRows() << endl;
    for (size_t i = 0; i < TEST_PERIOD; ++i) {
        auto start = std::chrono::steady_clock::now();
        Mat X(l);
        Mat Y(r);
        auto end = std::chrono::steady_clock::now();
        auto t1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        start = std::chrono::steady_clock::now();
        correlation::coefficient_Pearson cof;
        float k = cof.Pearson(X, Y);
        end = std::chrono::steady_clock::now();
        auto t2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "    Коэффициент: " << k << ", время rgbToGray " << t1 << " ms, "
                  << "время корреляции: " << t2 << " ms" << std::endl;
    }
}

static void outRGBtoGRAY(const std::string &fn) {
    std::cout << "  " << fn << std::endl;
    for (size_t i = 0; i < TEST_PERIOD; ++i) {
        auto start = std::chrono::steady_clock::now();
        Mat X(fn);
        auto end = std::chrono::steady_clock::now();
        auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "    Фото(" << X.getCols() << "x" << X.getRows() << ") время RGBtoGRAY: " << t << " ms" << std::endl;
    }
}

void identicalPhotosTest() {
    outKT("./Image/test_img/test_1.jpg", "./Image/test_img/test_1.jpg");
}

void defferentPhotosTest() {
    outKT("./Image/test_img/test_1.jpg", "./Image/test_img/test_2.jpg");
}

void defferentAngelsTest() {
    outKT("./Image/test_img/test_3_1_1.jpg", "./Image/test_img/test_3_1_2.jpg");
    outKT("./Image/test_img/test_3_2_1.jpg", "./Image/test_img/test_3_2_2.jpg");
}

void blurTest() {
    outKT("./Image/test_img/test_4_1_1.jpg", "./Image/test_img/test_4_1_2.jpg");
    outKT("./Image/test_img/test_4_2_1.jpg", "./Image/test_img/test_4_2_2.jpg");
    outKT("./Image/test_img/test_4_3_1.jpg", "./Image/test_img/test_4_3_2.jpg");
}

void correlationTimeTest() {
    std::vector<string> tst_jpg = {"1" , "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    std::vector<double> corr;
    for (size_t i = 0; i < tst_jpg.size() - 1; i += 2) {
        std::string l = ("./Image/img/" + tst_jpg[i] + ".jpg");
        std::string r = ("./Image/img/" + tst_jpg[i + 1] + ".jpg");
        std::cout << "  Пара: " << i << "-" << (i + 1) << std::endl;
        for (size_t j = 0; j < TEST_PERIOD; ++j) {
            Mat X(l);
            Mat Y(r);
            auto start = std::chrono::steady_clock::now();
            correlation::coefficient_Pearson cof;
            corr.push_back(cof.Pearson(X, Y));
            auto end = std::chrono::steady_clock::now();
            std::cout << "    Время корреляции(" << X.getCols() <<"x" << X.getRows() << "): "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                      << " ms" << std::endl;
        }
    }
    corr.clear();
}

void rgbToGrayTest() {
    outRGBtoGRAY("./Image/test_img/test_1.jpg");
    outRGBtoGRAY("./Image/test_img/test_2.jpg");
    outRGBtoGRAY("./Image/test_img/test_3_1_1.jpg");
    outRGBtoGRAY("./Image/test_img/test_3_1_2.jpg");
    outRGBtoGRAY("./Image/test_img/test_3_2_1.jpg");
    outRGBtoGRAY("./Image/test_img/test_3_2_2.jpg");
}

#ifdef CAMERA_TEST
void snapshotTimeTest() {
     // TODO: тест снимков разных рамзеров
}

void snapshotTimeToGray() {
    // TODO: ??? , тк снимки будут анализироватся после того как будет получен весь массив, то одновременное время бесполезно
}
#endif

void printDistance() {
    for (size_t i = 0; i < TEST_PERIOD; ++i) {
        auto start = std::chrono::steady_clock::now();
        rangefinder rf;
        auto dist = rf.distance("./Image/DataSet/");
        auto end = std::chrono::steady_clock::now();
        cout << "    Расстояние до объекта: " << dist << " [мм]; ";
        std::cout << "Время расчета: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " ms" << std::endl;
    }
}

