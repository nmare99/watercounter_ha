# README #

Простая плата на базе микросхемы ESP8266 (Wemos D1 mini). Плата считает кол-во импульсов от счетчиков воды (горячей и холодной) и импульс от системы АкваСторож.
Отображение в web интерфейсе и отправка на сервер умного дома по протоколу MQTT

![IMG_0254.jpg](https://bitbucket.org/repo/qEzRgg4/images/1915490043-IMG_0254.jpg)

![WaterCounter_схема.png](https://bitbucket.org/repo/qEzRgg4/images/1898141265-WaterCounter_%D1%81%D1%85%D0%B5%D0%BC%D0%B0.png)

![WaterCounter_bb.png](https://bitbucket.org/repo/qEzRgg4/images/601045342-WaterCounter_bb.png)

![WaterCounter_печатная плата.png](https://bitbucket.org/repo/qEzRgg4/images/3705732514-WaterCounter_%D0%BF%D0%B5%D1%87%D0%B0%D1%82%D0%BD%D0%B0%D1%8F%20%D0%BF%D0%BB%D0%B0%D1%82%D0%B0.png)


File System install https://github.com/esp8266/arduino-esp8266fs-plugin


# Распиновка #
* Красная лампа - D8 - GPIO15
* Синяя лампа - D7 - GPIO13
* Встроенная лампа - D4 - GPIO2
* Холодная вода - D5 - GPIO14
* Горячая вода - D2 - GPIO4
* ALARM - D6 - GPIO12


## Ссылки ##
* [esptool-gui](https://github.com/Rodmg/esptool-gui)
* [esptool](https://github.com/espressif/esptool)


## Код для заливки прошивки ##
```
#!bash

./esptool.py --port /dev/cu.wchusbserial1420 write_flash -fm=dio -fs=4MB 0x00000 ~/Downloads/0x00000_ESP8266_201705062105.bin

```