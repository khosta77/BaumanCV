#!/bin/bash
#make

# Тест №1
# Две одинаковые картинки
echo "Tест №1 - Две одинаковые фотографии"
./main identicalPhotos

# Тест №2
# Две разные картинки
echo -e "Tест №2 - Две разные фотографии"
./main defferentPhotos


# Тест №3
# Один предмет с разных ракурсов
echo -e "Tест №3 - С разных ракурсов"
./main defferentAngels

# Тест №4
# Резкость
echo -e "Tест №4 - Размытие"
./main blur

# Тест №5
# Время корреляции
echo -e "Tест №5 - Время корреляции"
./main correlation

# Тест №6
# Время RGB to GRAY
echo -e "Tест №6 - Время RGB to GRAY"
./main RGBtoGRAY

## Тест №7
## Время RGB to GRAY, снимки одного формата
#echo -e "Tест №7 - Время RGB to GRAY, снимки одного формата"
#./main


# Тест №8
# Время снимка и конвертиция в GRAY
type raspistill 2> /dev/null
if [ $? -eq 0 ]
then
  echo -e "Tест №8 - Время снимка и конвертиция в GRAY (размер снимка 640х480)"
  ./main 1 2 3
  ./main 1 2 3
  ./main 1 2 3
  ./main 1 2 3
  ./main 1 2 3
  ./main 1 2 3
  ./main 1 2 3
fi

# Тест №9
# Время вычисления расстояния
echo -e "Tест №9 - Время вычисления расстояния"
./main distance
