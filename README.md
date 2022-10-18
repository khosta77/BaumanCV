# Курсовой проект. Computer Vision Rangefinder

Цель проекта с помощью двух камер определять расстояние до наведенного объекта. Сравнение изображений происходит спомощью
корреляции.

## Сборка

1. **wiringPi** Если у вас нет утилиты git для работы с github-репозиториями, то устанавливаем:
   ```sh
   sudo apt-get install git-core
   ```
   Скачиваем исходники библиотеки из репозитория (Это библиотека сообщества)
   ```sh
   git clone https://github.com/WiringPi/WiringPi.git
   cd ./WiringPi
   ./build
   ```
2. **libjpeg**  *Эта бибилотека обезательна*
   ```sh
   sudo apt-get install libjpeg-dev
   sudo apt-get install libjpeg8-dev
   ```


