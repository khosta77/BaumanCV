#include "./MyTest.h"

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

//// Методы для определения размеров таблицы
void MyTest::determineTheSizeString() {
    for (size_t i = 0; i < results.size(); ++i)
        if (size_string < int(results[i].name.size()))
            size_string = results[i].name.size();
}

void MyTest::determineTheSizeId() {
    for (int i = results.size(); i > 0; i /= 10 )
        ++size_id;
}

void MyTest::determineTheSize() {
    determineTheSizeString();
    determineTheSizeId();
}

//// Методы вывода таблицы
void MyTest::printTitle(std::ostream& os) {
    os << std::setw(size_id) << std::left << "N" << "|"
       << std::setw(size_string) << std::left << "name" << "|"
       << std::setw(SIZE_COLOMN) << std::left << "avg, [sec]" << "|"
       << std::setw(SIZE_COLOMN) << std::left << "min, [sec]" << "|"
       << std::setw(SIZE_COLOMN) << std::left << "max, [sec]" << "|" << std::endl;
       draw_line(os);
}

void MyTest::printResults(std::ostream& os) {
    for (size_t i = 0; i < results.size(); ++i) {
        os << std::setw(size_id) << std::right << (i + 1) << "|"
           << std::setw(size_string) << std::left << results[i].name << "|"
           << std::setw(SIZE_COLOMN) << std::right << round(results[i].average_value) << "|"
           << std::setw(SIZE_COLOMN) << round(results[i].min_value) << "|"
           << std::setw(SIZE_COLOMN) << round(results[i].max_value) << "|" << std::endl;
#if 0  // кому-то будет удобно читать таблицу с разгроничением на строки
        draw_line(size_id, size_string);
#endif
    }
    draw_line(os);
}

void MyTest::output(std::ostream& os) { 
    determineTheSize();
    printTitle(os);
    printResults(os);
}

// public

MyTest::~MyTest() {
    results.clear();
    tests_methods.clear();
}

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


