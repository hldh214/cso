#include <Arduino.h>
//#include <Bounce2.h>
#include <AbsMouse.h>
#include <Keyboard.h>
#include <Arduino_FreeRTOS.h>


//#define LED_PIN 48
//#define BOOT_PIN 0

//Bounce2::Button bootButton = Bounce2::Button(); // INSTANTIATE A Bounce2::Button OBJECT

TaskHandle_t mouseTaskHandle;
TaskHandle_t buyTaskHandle;
TaskHandle_t F2AndReviveTaskHandle;
TaskHandle_t moveAndFireTaskHandle;

bool running = true;

void mouseClick(uint8_t button = MOUSE_LEFT) {
    AbsMouse.press(button);
    vTaskDelay(pdMS_TO_TICKS(50));
    AbsMouse.release(button);
}

[[noreturn]] void mouseTask(__attribute__((unused)) void *pvParameters) {
    while (true) {
        // waiting room -> game start
        AbsMouse.move(2750, 1405);
        mouseClick();

        vTaskDelay(pdMS_TO_TICKS(4000));

        // quit score board -> return to the waiting room
        AbsMouse.move(2750, 1730);
        mouseClick();

        vTaskDelay(pdMS_TO_TICKS(4000));
    }
}

[[noreturn]] void buyTask(__attribute__((unused)) void *pvParameters) {
    while (true) {
        Keyboard.write('b');
        vTaskDelay(pdMS_TO_TICKS(50));
        // press 's' key for melee weapon
        Keyboard.write('s');
        vTaskDelay(pdMS_TO_TICKS(50));
        // press 'b' key
        Keyboard.write('b');
        vTaskDelay(pdMS_TO_TICKS(50));
        // press 's' key for secondary weapon
        Keyboard.write('f');
        vTaskDelay(pdMS_TO_TICKS(50));
        // press '3' key for melee weapon
        Keyboard.write('3');
        vTaskDelay(pdMS_TO_TICKS(50));

        Keyboard.write('b');
        vTaskDelay(pdMS_TO_TICKS(50));
        // press 'ESC' key for close the popup window
        Keyboard.write(KEY_ESC);
        vTaskDelay(pdMS_TO_TICKS(50));

        // press 'r' key for revive
        Keyboard.write('r');

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

[[noreturn]] void F2AndReviveTask(__attribute__((unused)) void *pvParameters) {
    while (true) {
        // press 'f2' key for auto buying
        Keyboard.write(KEY_F2);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

[[noreturn]] void moveAndFireTask(__attribute__((unused)) void *pvParameters) {
    while (true) {
        Keyboard.press('p');
        Keyboard.press('w');
        Keyboard.press(KEY_RIGHT_ARROW);

        vTaskDelay(pdMS_TO_TICKS(6000));

        Keyboard.release('p');
        Keyboard.release('w');
        Keyboard.release(KEY_RIGHT_ARROW);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

__attribute__((unused)) void setup() {
    Keyboard.begin();
    AbsMouse.init(3840, 2160);

//    pinMode(LED_PIN, OUTPUT);
//    pinMode(BOOT_PIN, INPUT_PULLUP);
//    digitalWrite(LED_PIN, LOW);

//    bootButton.attach(BOOT_PIN, INPUT_PULLUP);
//    bootButton.interval(5);
//    bootButton.setPressedState(LOW);

    xTaskCreate(mouseTask, "Mouse Task", 128, nullptr, 1, &mouseTaskHandle);
    xTaskCreate(buyTask, "Buy Task", 128, nullptr, 1, &buyTaskHandle);
    xTaskCreate(F2AndReviveTask, "F2 And Revive Task", 128, nullptr, 1, &F2AndReviveTaskHandle);
    xTaskCreate(moveAndFireTask, "Move And Fire Task", 128, nullptr, 1, &moveAndFireTaskHandle);

//    // fixme: this is so ugly...
//    vTaskSuspend(mouseTaskHandle);
//    vTaskSuspend(buyTaskHandle);
//    vTaskSuspend(F2AndReviveTaskHandle);
//    vTaskSuspend(moveAndFireTaskHandle);

    vTaskStartScheduler();
}

void loop() {
    // UPDATE THE BUTTON BY CALLING .update() AT THE BEGINNING OF THE LOOP:
//    bootButton.update();
//
//    if (bootButton.pressed()) {
//        running = !running;
//    }

//    if (running) {
////        digitalWrite(LED_PIN, HIGH);
//        vTaskResume(mouseTaskHandle);
////        vTaskResume(buyTaskHandle);
////        vTaskResume(F2AndReviveTaskHandle);
////        vTaskResume(moveAndFireTaskHandle);
//    } else {
//        Keyboard.releaseAll();
////        digitalWrite(LED_PIN, LOW);
//        vTaskSuspend(mouseTaskHandle);
//        vTaskSuspend(buyTaskHandle);
//        vTaskSuspend(F2AndReviveTaskHandle);
//        vTaskSuspend(moveAndFireTaskHandle);
//    }
}
