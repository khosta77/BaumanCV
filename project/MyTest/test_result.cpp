#include "./test_result.h"

test_result::~test_result() {
    clear();
}

void test_result::M2a() {
    avgv();
    min_value = *std::min_element(all_value.begin(), all_value.end());
    max_value = *std::max_element(all_value.begin(), all_value.end());
}

void test_result::clear() {
    all_value.clear();
    name.clear();
}

void test_result::avgv() {
    average_value = all_value[0];
    for (size_t i = 1; i < all_value.size(); ++i)
        average_value += all_value[i];
    average_value /= all_value.size();
}


