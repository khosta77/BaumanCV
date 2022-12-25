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


