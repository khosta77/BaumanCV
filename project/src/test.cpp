#include "../include/test.h"

void test_time() {
    MyTest::add("RGBtGRAY_1200×800_v1", [](){  Mat m("./Image/test_img/test_1.jpg"); });
    MyTest::add("RGBtGRAY_1200×800_v2", [](){  Mat m("./Image/test_img/test_2.jpg"); });
    MyTest::add("RGBtGRAY_960×1280_v1", [](){  Mat m("./Image/test_img/test_3_1_1.jpg"); });
    MyTest::add("RGBtGRAY_960×1280_v2", [](){  Mat m("./Image/test_img/test_3_1_2.jpg"); });
    MyTest::add("RGBtGRAY_960×1280_v3", [](){  Mat m("./Image/test_img/test_3_2_1.jpg"); });
    MyTest::add("RGBtGRAY_960×1280_v4", [](){  Mat m("./Image/test_img/test_3_2_2.jpg"); });
    MyTest::add("RGBtGRAY_2592×1944_v1.1", [](){  Mat m("./Image/DataSet/ph_1_1.jpg"); });
    MyTest::add("RGBtGRAY_2592×1944_v1.2", [](){  Mat m("./Image/DataSet/ph_1_2.jpg"); });
    MyTest::add("RGBtGRAY_2592×1944_v2.1", [](){  Mat m("./Image/DataSet/ph_2_1.jpg"); });
    MyTest::add("RGBtGRAY_2592×1944_v2.2", [](){  Mat m("./Image/DataSet/ph_2_2.jpg"); });
    MyTest::add("RGBtGRAY_2592×1944_v3.1", [](){  Mat m("./Image/DataSet/ph_3_1.jpg"); });
    MyTest::add("RGBtGRAY_2592×1944_v3.2", [](){  Mat m("./Image/DataSet/ph_3_2.jpg"); });
    MyTest::add("RGBtGRAY_2592×1944_v4.1", [](){  Mat m("./Image/DataSet/ph_4_1.jpg"); });
    MyTest::add("RGBtGRAY_2592×1944_v4.2", [](){  Mat m("./Image/DataSet/ph_4_2.jpg"); });
    MyTest::add("RGBtGRAY_2592×1944_v5.1", [](){  Mat m("./Image/DataSet/ph_5_1.jpg"); });
    MyTest::add("RGBtGRAY_2592×1944_v5.2", [](){  Mat m("./Image/DataSet/ph_5_2.jpg"); });
    MyTest::add("RGBtGRAY_2592×1944_v6.1", [](){  Mat m("./Image/DataSet/ph_6_1.jpg"); });
    MyTest::add("RGBtGRAY_2592×1944_v6.2", [](){  Mat m("./Image/DataSet/ph_6_2.jpg"); });
    MyTest::add("RGBtGRAY_2592×1944_v8.1", [](){  Mat m("./Image/DataSet/ph_8_1.jpg"); });
    MyTest::add("RGBtGRAY_2592×1944_v8.2", [](){  Mat m("./Image/DataSet/ph_8_2.jpg"); });
    MyTest::add("RGBtGRAY_2592×1944_v9.1", [](){  Mat m("./Image/DataSet/ph_9_1.jpg"); });
    MyTest::add("RGBtGRAY_2592×1944_v9.2", [](){  Mat m("./Image/DataSet/ph_9_2.jpg"); });
    MyTest::add("RGBtGRAY_2592×1944_v10.1", [](){  Mat m("./Image/DataSet/ph_10_1.jpg"); });
    MyTest::add("RGBtGRAY_2592×1944_v10.2", [](){  Mat m("./Image/DataSet/ph_10_2.jpg"); });
    MyTest::make();
}

// private
void MyTest::draw_line(const int& id, const int& n) {
    std::cout << std::setfill('-') << std::setw(id) << "" << "+"
              << std::setfill('-') << std::setw(n) << "" << "+"
              << std::setfill('-') << std::setw(9) << "" << "+"
              << std::setfill('-') << std::setw(9) << "" << "+"
              << std::setfill('-') << std::setw(9) << "" << "+" << std::endl;
}

// public
void MyTest::add(const std::string& tn, func tfPtr) {
    tests_methods.push_back(test_obj(tn, tfPtr));
}

// TODO: радикольно уменьшить размер этой функции, вынеся ее куски в отдельные методы
void MyTest::make() {
    time_t now = time(0);
    [[ maybe_unused ]] char* start = ctime(&now);
    for (size_t i = 0; i < tests_methods.size(); ++i) { // вот это сбросить в метод отдельный
        double startTime, endTime;
        test_result tr;
        for (size_t j = 0; j < TEST_COUNT; ++j) {  // вот это в отдельный метод
            startTime = getCPUTime();
            tests_methods[i].foo_ptr();
            endTime = getCPUTime();
            tr.all_value.push_back((endTime - startTime));
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
              << std::setw(9)/* << std::setprecision(3)*/<< std::left << "avg" << "|"
              << std::setw(9) << std::left << "min" << "|"
              << std::setw(9) << std::left << "max" << "|" << std::endl;
    draw_line(size_id, size_string);
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << std::setw(size_id) << i << "|"
                  << std::setw(size_string) << std::left << results[i].name << "|"
                  << std::setw(9) << std::setprecision(3) << results[i].average_value << "|"
                  << std::setw(9) << std::setprecision(3) << results[i].min_value << "|"
                  << std::setw(9) << std::setprecision(3) << results[i].max_value << "|" << std::endl;
        draw_line(size_id, size_string);
    }
    // TODO: написать сохранение тестовых данных в csv файл
}


