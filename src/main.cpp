#include <Arduino.h>
#include <USBHIDMouse.h>
#include <USBHIDKeyboard.h>
#include <USB.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_PIN 48
#define BOOT_PIN 0

USBHIDMouse Mouse;
USBHIDKeyboard Keyboard;

TaskHandle_t mouseTaskHandle;
TaskHandle_t buyTaskHandle;
TaskHandle_t fireTaskHandle;
TaskHandle_t moveTaskHandle;

bool running = false;

[[noreturn]] void mouseTask(__attribute__((unused)) void *pvParameters) {
    while (true) {
        Mouse.move(0, -1);
        vTaskDelay(pdMS_TO_TICKS(10));
        Mouse.move(0, 1);

        vTaskDelay(pdMS_TO_TICKS(8000));
    }
}

[[noreturn]] void buyTask(__attribute__((unused)) void *pvParameters) {
    while (true) {
        // press 'b' key
        Keyboard.write('b');
        // press 'f' key for melee weapon
        Keyboard.write('f');
        // press '3' key for melee weapon
        Keyboard.write('3');

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

[[noreturn]] void fireTask(__attribute__((unused)) void *pvParameters) {
    while (true) {
        // press 'p' key
        Keyboard.write('p');

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

[[noreturn]] void moveTask(__attribute__((unused)) void *pvParameters) {
    while (true) {
        // press 'w' key
        Keyboard.write('w');
        // press 'a' key
        Keyboard.write('a');

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void bootPressed() {
    running = !running;
}

__attribute__((unused)) void setup() {
    Mouse.begin();
    Keyboard.begin();
    USB.begin();

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    attachInterrupt(BOOT_PIN, bootPressed, FALLING);

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
