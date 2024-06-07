#include <Arduino.h>
#include <USBHIDMouse.h>
#include <USBHIDKeyboard.h>
#include <USB.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <Bounce2.h>


#define LED_PIN 48
#define BOOT_PIN 0

Bounce2::Button bootButton = Bounce2::Button(); // INSTANTIATE A Bounce2::Button OBJECT

USBHIDMouse Mouse;
USBHIDKeyboard Keyboard;

TaskHandle_t mouseTaskHandle;
TaskHandle_t buyTaskHandle;
TaskHandle_t fireTaskHandle;
TaskHandle_t moveTaskHandle;

bool running = false;

[[noreturn]] void mouseTask(__attribute__((unused)) void *pvParameters) {
    while (true) {
        Mouse.move(0, -10);
        vTaskDelay(pdMS_TO_TICKS(10));
        Mouse.move(0, 10);

        vTaskDelay(pdMS_TO_TICKS(8000));
    }
}

[[noreturn]] void buyTask(__attribute__((unused)) void *pvParameters) {
    while (true) {
        // press 'b' key
        Keyboard.write('b');
        vTaskDelay(pdMS_TO_TICKS(50));
        // press 's' key for melee weapon
        Keyboard.write('s');
        vTaskDelay(pdMS_TO_TICKS(50));
        // press '3' key for melee weapon
        Keyboard.write('3');
        // press 'f2' key for auto buying
        Keyboard.write(KEY_F2);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

[[noreturn]] void fireTask(__attribute__((unused)) void *pvParameters) {
    while (true) {
        // press 'p' key
        Keyboard.press('p');
        // press 'w' key
        Keyboard.press('w');

        vTaskDelay(pdMS_TO_TICKS(2000));

        Keyboard.releaseAll();

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

[[noreturn]] void moveTask(__attribute__((unused)) void *pvParameters) {
    while (true) {
        // press 'KEY_RIGHT_ARROW' key
        Keyboard.press(KEY_RIGHT_ARROW);
        vTaskDelay(pdMS_TO_TICKS(500));
        Keyboard.releaseAll();

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

__attribute__((unused)) void setup() {
    Mouse.begin();
    Keyboard.begin();
    USB.begin();

    pinMode(LED_PIN, OUTPUT);
    pinMode(BOOT_PIN, INPUT_PULLUP);
    digitalWrite(LED_PIN, LOW);

    bootButton.attach(BOOT_PIN, INPUT_PULLUP);
    bootButton.interval(5);
    bootButton.setPressedState(LOW);

    xTaskCreate(mouseTask, "Mouse Task", 2048, nullptr, 1, &mouseTaskHandle);
    xTaskCreate(buyTask, "Buy Task", 2048, nullptr, 1, &buyTaskHandle);
    xTaskCreate(fireTask, "Fire Task", 2048, nullptr, 1, &fireTaskHandle);
    xTaskCreate(moveTask, "Move Task", 2048, nullptr, 1, &moveTaskHandle);

    // fixme: this is so ugly...
    vTaskSuspend(mouseTaskHandle);
    vTaskSuspend(buyTaskHandle);
    vTaskSuspend(fireTaskHandle);
    vTaskSuspend(moveTaskHandle);
}

void loop() {
    // UPDATE THE BUTTON BY CALLING .update() AT THE BEGINNING OF THE LOOP:
    bootButton.update();

    if (bootButton.pressed()) {
        running = !running;
    }

    if (running) {
        digitalWrite(LED_PIN, HIGH);
        vTaskResume(mouseTaskHandle);
        vTaskResume(buyTaskHandle);
        vTaskResume(fireTaskHandle);
        vTaskResume(moveTaskHandle);
    } else {
        digitalWrite(LED_PIN, LOW);
        vTaskSuspend(mouseTaskHandle);
        vTaskSuspend(buyTaskHandle);
        vTaskSuspend(fireTaskHandle);
        vTaskSuspend(moveTaskHandle);
    }
}
