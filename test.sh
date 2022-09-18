#!/bin/bash
make

# Тест №1
# Две одинаковые картинки
echo "Tест №1 - Две одинаковые фотографии"
res1=$(./main ./test_img/test_1.jpg ./test_img/test_1.jpg)

if [[ "$res1" == "1" ]]; then
    echo "    Коэффицент равен 1. Result = $res1"
elif [[ "$res1" > "0.9" ]]; then
    echo "    Коэффицент близок к 1. Result = $res1"
else
    echo "    Коэффицент не близок к 1. Result = $res1"
fi

# Тест №2
# Две разные картинки
echo -e "Tест №2 - Две разные фотографии"

res2=$(./main ./test_img/test_2.jpg ./test_img/test_1.jpg)

if [[ "$res2" == "0" ]]; then
    echo "    Коэффицент равен 0. Result = $res2"
elif [[ "$res2" > "0" && "$res2" < "0.3" ]]; then
    echo "    Коэффицент близок к 0. Result = $res2"
else
    echo "    Коэффицент не близок к 0. Result = $res2"
fi


# Тест №3
# Один предмет с разных ракурсов
echo -e "Tест №3 - С разных ракурсов"

res31=$(./main ./test_img/test_3_1_1.jpg ./test_img/test_3_1_2.jpg)

if [[ "$res31" > "0.5" ]]; then
    echo "    Коэффицент приближен к реальному значению. Result = $res31"
else
    echo "    Коэффицент далек от правды. Result = $res31"
fi

res32=$(./main ./test_img/test_3_2_1.jpg ./test_img/test_3_2_2.jpg)

if [[ "$res32" > "0.5" ]]; then
    echo "    Коэффицент приближен к реальному значению. Result = $res32"
else
    echo "    Коэффицент далек от правды. Result = $res32"
fi

# Тест №4
# Резкость
echo -e "Tест №4 - Размытие"

res41=$(./main ./test_img/test_4_1_1.jpg ./test_img/test_4_1_2.jpg)

if [[ "$res41" < "0.5" ]]; then
    echo "    Коэффицент приближен к реальному значению. Result = $res41"
else
    echo "    Коэффицент далек от правды. Result = $res41"
fi

res42=$(./main ./test_img/test_4_2_1.jpg ./test_img/test_4_2_2.jpg)

if [[ "$res42" < "0.5" ]]; then
    echo "    Коэффицент приближен к реальному значению. Result = $res42"
else
    echo "    Коэффицент далек от правды. Result = $res42"
fi

res43=$(./main ./test_img/test_4_3_1.jpg ./test_img/test_4_3_2.jpg)

if [[ "$res43" < "0.5" ]]; then
    echo "    Коэффицент приближен к реальному значению. Result = $res43"
else
    echo "    Коэффицент далек от правды. Result = $res43"
fi

# Тест №5
# Время RGB to GRAY
echo -e "Tест №5 - Время RGB to GRAY"
./main ./test_img/test_1.jpg
./main ./test_img/test_2.jpg
./main ./test_img/test_3_1_1.jpg
./main ./test_img/test_3_1_2.jpg
./main ./test_img/test_3_2_1.jpg
./main ./test_img/test_3_2_2.jpg
./main
# Тест №6
# Колличество фотографий за определенное время
echo -e "Tест №6 - Колличество фотографий за определенное время"
./main 1 2 3
