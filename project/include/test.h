#ifndef CVRANGEFINDER_TEST_H_
#define CVRANGEFINDER_TEST_H_

#include "Mat.h"
#include "correlation.h"
#include "rangefinder.h"
#include "wiringPi.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#define CAMERA_TEST 0

#define IDENTICAL_PHOTOS 1
#define DEFFERENT_PHOTOS 2
#define DEFFERENT_ANGLES 3
#define BLUR 4
#define CORRELATION_TIME 5
#define RGB_TO_GRAY 6

#ifdef CAMERA_TEST
#define SNAPSHOT_TIME 7
#define SNAPSHOT_TIME_TO_GRAY 8
#endif

#define DISTANSE 9

#define TEST_PERIOD 20  /* Колличество тестов в 1 цикле */

void runTest(int argc, const char **argv);
int getTestCode(const std::string&);

void identicalPhotosTest();
void defferentPhotosTest();
void defferentAngelsTest();
void blurTest();
void correlationTimeTest();
void rgbToGrayTest();

#ifdef CAMERA_TEST
void snapshotTimeTest();
void snapshotTimeToGray();
#endif

void printDistance();

#endif // CVRANGEFINDER_TEST_H_
