#include <esp32-hal-gpio.h>
#include <Arduino.h>
#include "USB.h"
#include "USBHIDMouse.h"
#include "USBHIDKeyboard.h"
#include "ThreadController.h"
//#include "Adafruit_NeoPixel.h"

USBHIDMouse Mouse;
USBHIDKeyboard Keyboard;

ThreadController tc = ThreadController();

//int bootPin = 0;
//bool isDisabled = false;

//Adafruit_NeoPixel pixels(1, 48, NEO_GRB + NEO_KHZ800);

void mouse() {
    // move mouse up
    Mouse.move(0, -1);
    delay(5);
    // move mouse down
    Mouse.move(0, 1);
}

void buy_weapon() {
    // press 'b' key
    Keyboard.write('b');
    // press 'x' key for melee weapon
    Keyboard.write('x');
    // press '3' key for melee weapon
    Keyboard.write('3');
}

void fire() {
    // press 'p' key
    Keyboard.write('p');
}

void move() {
    // press 'w' key
    Keyboard.write('w');

    // press 'a' key
    Keyboard.write('a');
}

//void stop_rgb() {
//    pixels.setPixelColor(0, Adafruit_NeoPixel::Color(0, 0, 0));
//}
//
//void start_rgb() {
//    for (int r = 0; r < 255; r += 10) {
//        for (int g = 0; g < 255; g += 10) {
//            for (int b = 0; b < 255; b += 10) {
//                pixels.setPixelColor(0, Adafruit_NeoPixel::Color(r, g, b));
//                delay(10);
//            }
//        }
//    }
//}

void setup() {
//    pinMode(bootPin, INPUT_PULLUP);
//    pixels.begin();

    Mouse.begin();
    Keyboard.begin();
    USB.begin();

    auto *mouseThread = new Thread();
    mouseThread->onRun(mouse);
    mouseThread->setInterval(8000);

    auto *buyWeaponThread = new Thread();
    buyWeaponThread->onRun(buy_weapon);
    buyWeaponThread->setInterval(5000);

    auto *fireThread = new Thread();
    fireThread->onRun(fire);
    fireThread->setInterval(500);

    auto *moveThread = new Thread();
    moveThread->onRun(move);
    moveThread->setInterval(2000);

//    auto *rgbThread = new Thread();
//    rgbThread->onRun(start_rgb);
//    rgbThread->setInterval(10);

    tc.add(mouseThread);
    tc.add(buyWeaponThread);
    tc.add(fireThread);
    tc.add(moveThread);
//    tc.add(rgbThread);
}

void loop() {
    tc.run();

//    if (digitalRead(bootPin) == LOW) {
//        isDisabled = !isDisabled;
//    }
//
//    if (!isDisabled) {
//        tc.run();
//    } else {
//        stop_rgb();
//    }
}
