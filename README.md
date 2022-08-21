# CVRangefinder

В данной ветке собрано все для запуска на rasberry pi 2. **При запуске на стационарном компьютере работать не должна.**

## Курсовой проект. Computer Vision Rangefinder

Цель проекта с помощью двух камер определять расстояние до наведенного объекта. Сравнение изображений происходит спомощью 
коэффициента корреляции. Формула по которой происходит вычисление:

![](https://github.com/khosta77/CVRangefinder/blob/main/info/%D1%84%D0%BE%D1%80%D0%BC%D1%83%D0%BB%D0%B0.png)

где X и Y это две разные матрицы(снимка).

### Работал над проектом

* Филимонов Степан

### Как собрать и запустить программу?

##### У вас должен быть установлен openCV!

```bash
$ cmake --build .
$
$ ./main # произойдут вычесления с картинками из директории img/
$
$ ./main [путь1] [путь2] # Или ваша картинки (пара), вы консоль выведится вычесленная корреляция
```

Возможно провести тест из скрпита с построение графика в gnuplot

```bash
$ cmake --build .
$
$ bash ./test.sh # Картинки берутся из директории img/
```

### Отступление

Формула выше - это формула Пиросона

![](https://github.com/khosta77/CVRangefinder/blob/main/info/%D0%9A%D0%BE%D1%8D%D1%84%D1%84%D0%B8%D1%86%D0%B8%D0%B5%D0%BD%D1%82%20%D0%BA%D0%BE%D1%80%D1%80%D0%B5%D0%BB%D1%8F%D1%86%D0%B8%D0%B8%20%D0%9F%D0%B8%D1%80%D1%81%D0%BE%D0%BD%D0%B0%202.jpg)

### Полезные ссылки

[Формула для вычисления расстояния до объекта](https://translated.turbopages.org/proxy_u/en-ru.ru.f461a4a3-6301e7f5-29783cd8-74722d776562/https/www.quora.com/How-do-you-calculate-the-object-distance-from-the-focal-length)