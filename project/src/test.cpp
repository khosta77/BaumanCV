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
    mt.add("correlation_2592x1944", []() {  
            double start = getCPUTime();
            Mat A("./Image/DataSet/ph_10_1.jpg");
            Mat B("./Image/DataSet/ph_10_2.jpg");
            double end = getCPUTime();
            correlation::coefficient_Pearson cor;
            cor.Pearson(A, B);
            return (end - start); });
    mt.make();
}

// private
void MyTest::draw_line(const int& id, const int& n) {
    std::cout << std::setfill('-') << std::setw(id) << "" << "+"
              << std::setw(n) << "" << "+"
              << std::setw(SIZE_COLOMN) << "" << "+"
              << std::setw(SIZE_COLOMN) << "" << "+"
              << std::setw(SIZE_COLOMN) << "" << "+" << std::setfill(' ') << std::endl;
}

double MyTest::round(const double& v) {
    return std::round(v * ROUND) / ROUND;
}

// public
void MyTest::add(const std::string& tn, double (*f)()) {
    tests_methods.push_back(test_obj(tn, f));
}

// TODO: радикольно уменьшить размер этой функции, вынеся ее куски в отдельные методы
void MyTest::make() {
    std::cout << "Test size: " << TEST_COUNT << std::endl;
    time_t now = time(0);
    [[ maybe_unused ]] char* start = ctime(&now);
    for (size_t i = 0; i < tests_methods.size(); ++i) { // вот это сбросить в метод отдельный
        double startTime, endTime, inaccuracy;
        test_result tr;
        for (size_t j = 0; j < TEST_COUNT; ++j) {  // вот это в отдельный метод
            startTime = getCPUTime();
            inaccuracy = tests_methods[i].foo();
            endTime = getCPUTime();
            tr.all_value.push_back((endTime - startTime - inaccuracy));
        }
        tr.average_value = tr.all_value[0];
        for (size_t j = 1; j < tr.all_value.size(); ++j)  // в отдельный методд
            tr.average_value += tr.all_value[j];
        tr.average_value /= tr.all_value.size();
        tr.min_value = *std::min_element(tr.all_value.begin(), tr.all_value.end());
        tr.max_value = *std::max_element(tr.all_value.begin(), tr.all_value.end());
        tr.name = tests_methods[i].name;
        results.push_back(tr);
    }
    [[ maybe_unused ]]char* end = ctime(&now);

    // TODO: написать алгоритм который определит максимальное имя и порядок размеров, для таблицы
    int size_string = 0;
    for (size_t i = 0; i < results.size(); ++i)
        if (size_string < int(results[i].name.size()))
            size_string = results[i].name.size();
    int size_id = 0;
    for (int i = results.size(); i > 0; i /= 10 )
        ++size_id;

    // TODO: написать вывод таблицы
    std::cout << std::setw(size_id) << std::left << "N" << "|"
              << std::setw(size_string) << std::left << "name" << "|"
              << std::setw(SIZE_COLOMN) << std::left << "avg, [sec]" << "|"
              << std::setw(SIZE_COLOMN) << std::left << "min, [sec]" << "|"
              << std::setw(SIZE_COLOMN) << std::left << "max, [sec]" << "|" << std::endl;
    draw_line(size_id, size_string);
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << std::setw(size_id) << std::right << (i + 1) << "|"
                  << std::setw(size_string) << std::left << results[i].name << "|"
                  << std::setw(SIZE_COLOMN) << std::right << round(results[i].average_value) << "|"
                  << std::setw(SIZE_COLOMN) << round(results[i].min_value) << "|"
                  << std::setw(SIZE_COLOMN) << round(results[i].max_value) << "|" << std::endl;
        // draw_line(size_id, size_string);
    }
    draw_line(size_id, size_string);
    std::cout << "S: " << start;
    std::cout << "E: " << end;
    // TODO: написать сохранение тестовых данных в csv файл
}


