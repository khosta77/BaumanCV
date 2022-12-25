#include "../include/test.h"

void test_time() {
    MyTest mt;
    mt.add("getCPUTime()", [](){  getCPUTime(); return double(0);});
    mt.add("RGBtGRAY_1200x800_v1", [](){  Mat m("./Image/test_img/test_1.jpg"); return double(0); });
    mt.add("RGBtGRAY_1200x800_v2", [](){  Mat m("./Image/test_img/test_2.jpg"); return double(0); });
    mt.add("RGBtGRAY_960x1280_v1", [](){  Mat m("./Image/test_img/test_3_1_1.jpg"); return double(0); });
    mt.add("RGBtGRAY_960x1280_v2", [](){  Mat m("./Image/test_img/test_3_1_2.jpg"); return double(0); });
    mt.add("RGBtGRAY_960x1280_v3", [](){  Mat m("./Image/test_img/test_3_2_1.jpg"); return double(0); });
    mt.add("RGBtGRAY_960x1280_v4", [](){  Mat m("./Image/test_img/test_3_2_2.jpg"); return double(0); });
    mt.add("RGBtGRAY_2592x1944_v1.1", [](){  Mat m("./Image/DataSet/ph_1_1.jpg"); return double(0); });
    mt.add("RGBtGRAY_2592x1944_v1.2", [](){  Mat m("./Image/DataSet/ph_1_2.jpg"); return double(0); });
    mt.add("RGBtGRAY_2592x1944_v2.1", [](){  Mat m("./Image/DataSet/ph_2_1.jpg"); return double(0); });
    mt.add("RGBtGRAY_2592x1944_v2.2", [](){  Mat m("./Image/DataSet/ph_2_2.jpg"); return double(0); });
    mt.add("RGBtGRAY_2592x1944_v3.1", [](){  Mat m("./Image/DataSet/ph_3_1.jpg"); return double(0); });
    mt.add("RGBtGRAY_2592x1944_v3.2", [](){  Mat m("./Image/DataSet/ph_3_2.jpg"); return double(0); });
    mt.add("RGBtGRAY_2592x1944_v4.1", [](){  Mat m("./Image/DataSet/ph_4_1.jpg"); return double(0); });
    mt.add("RGBtGRAY_2592x1944_v4.2", [](){  Mat m("./Image/DataSet/ph_4_2.jpg"); return double(0); });
    mt.add("RGBtGRAY_2592x1944_v5.1", [](){  Mat m("./Image/DataSet/ph_5_1.jpg"); return double(0); });
    mt.add("RGBtGRAY_2592x1944_v5.2", [](){  Mat m("./Image/DataSet/ph_5_2.jpg"); return double(0); });
    mt.add("RGBtGRAY_2592x1944_v6.1", [](){  Mat m("./Image/DataSet/ph_6_1.jpg"); return double(0); });
    mt.add("RGBtGRAY_2592x1944_v6.2", [](){  Mat m("./Image/DataSet/ph_6_2.jpg"); return double(0); });
    mt.add("RGBtGRAY_2592x1944_v8.1", [](){  Mat m("./Image/DataSet/ph_8_1.jpg"); return double(0); });
    mt.add("RGBtGRAY_2592x1944_v8.2", [](){  Mat m("./Image/DataSet/ph_8_2.jpg"); return double(0); });
    mt.add("RGBtGRAY_2592x1944_v9.1", [](){  Mat m("./Image/DataSet/ph_9_1.jpg"); return double(0); });
    mt.add("RGBtGRAY_2592x1944_v9.2", [](){  Mat m("./Image/DataSet/ph_9_2.jpg"); return double(0); });
    mt.add("RGBtGRAY_2592x1944_v10.1", [](){  Mat m("./Image/DataSet/ph_10_1.jpg"); return double(0); });
    mt.add("RGBtGRAY_2592x1944_v10.2", [](){  Mat m("./Image/DataSet/ph_10_2.jpg"); return double(0); });
    mt.add("RGBtGRAY_960x1280_v1", [](){  Mat m("./Image/img/1.jpg"); return double(0); });
    mt.add("correlation_2592x1944_v10", []() {  
            double start = getCPUTime();
            Mat A("./Image/DataSet/ph_10_1.jpg");
            Mat B("./Image/DataSet/ph_10_2.jpg");
            double end = getCPUTime();
            correlation::coefficient_Pearson cor;
            cor.Pearson(A, B);
            return (end - start); });
    mt.add("correlation_960x1280_v1", []() {
            double start = getCPUTime();
            Mat A("./Image/img/1.jpg");
            Mat B("./Image/img/2.jpg");
            double end = getCPUTime();
            correlation::coefficient_Pearson cor;
            cor.Pearson(A, B);
            return (end - start); });
    mt.make();
}

// private
void MyTest::draw_line(std::ostream& os) {
    os << std::setfill('-') << std::setw(size_id) << "" << "+"
       << std::setw(size_string) << "" << "+"
       << std::setw(SIZE_COLOMN) << "" << "+"
       << std::setw(SIZE_COLOMN) << "" << "+"
       << std::setw(SIZE_COLOMN) << "" << "+" << std::setfill(' ') << std::endl;
}

double MyTest::round(const double& v) {
    return std::round(v * ROUND) / ROUND;
}

//// Методы для проведения теста
void MyTest::makeOneTestMethod(const size_t& i, test_result& tr, double& startTime,
                               double& endTime, double& inaccuracy) {
        for (size_t j = 0; j < TEST_COUNT; ++j) {
            startTime = getCPUTime();
            inaccuracy = tests_methods[i].foo();
            endTime = getCPUTime();
            tr.all_value.push_back((endTime - startTime - inaccuracy));
        }
}

void MyTest::oneTestMethod(const size_t& i, test_result& tr, double& startTime, double& endTime,
                           double& inaccuracy) {
        makeOneTestMethod(i, tr, startTime, endTime, inaccuracy);
        tr.M2a();
        tr.name = tests_methods[i].name;
        results.push_back(tr);
        tr.clear(); 
} 

void MyTest::makeTest() {
    double startTime, endTime, inaccuracy;
    test_result tr;
    for (size_t i = 0; i < tests_methods.size(); ++i)
        oneTestMethod(i, tr, startTime, endTime, inaccuracy);
}
////

// public
void MyTest::add(const std::string& tn, double (*f)()) {
    tests_methods.push_back(test_obj(tn, f));
}

void MyTest::make() {
    std::cout << "Test size: " << TEST_COUNT << std::endl;
    start.getSystemTime();
    makeTest();
    end.getSystemTime();
    output(std::cout);
    std::cout << "Start program at: " << start << std::endl;
    std::cout << "End program at:   " << end << std::endl;
    // Т.к. проект будет законсервирован, то было решено не делать запись в файл.
}


