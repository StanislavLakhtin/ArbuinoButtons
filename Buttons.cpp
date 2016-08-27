//
// Created by Stanislav Lakhtin on 23/07/16.
//

#include "Buttons.h"

Buttons::Buttons() {
    for (int i = 0; i < MAXLISTENERS; i++) {
        listener[i].pin = -1;
    }
}

bool Buttons::AddEventListener(int pin, void (*keyPressHandler)(void), void (*keyReleaseHandler)(void)){
    int index = this->findByPin(pin);
    if (index != -1) {
        Listener *selected = &listener[index];
        selected->pin = pin;
        selected->lastChange = millis();
        selected->state = Released;
        selected->onKeyPressHandler = keyPressHandler;
        selected->onKeyReleaseHandler = keyReleaseHandler;
        return true;
    } else {
        return false;
    }
}

//возвратить -1 если не найден или index, если найен соответствующий index
int Buttons::findByPin(int pin) {
    for (int i = 0; i < MAXLISTENERS; i++) {
        Listener current = listener[i];
        if (current.pin == pin) {
            return i;
        }
    }
    for (int i = 0; i < MAXLISTENERS; i++) {
        Listener current = listener[i];
        if (current.pin == -1) {
            return i;
        }
    }
    return -1;
}

void Buttons::ReleaseEventListeners(int pin) {
    for (int i = 0; i < MAXLISTENERS; i++) {
        Listener current = listener[i];
        if (current.pin == pin) {
            current.pin = -1;
            current.onKeyPressHandler = nullptr;
            current.onKeyReleaseHandler = nullptr;
        }
    }
}

void Buttons::processIt(void) {
    //read buttons
    long cT = millis();
    for (int i = 0; i < MAXLISTENERS; i++) {
        Listener *current = &listener[i];
        if (current->pin != -1) {;
            int currentState = digitalRead(current->pin);
            if (current->state != currentState && abs(cT - current->lastChange)>THRESHOLD) {
                current->lastChange = cT;
                switch (current->state) {
                    case (KeyPress) : {
                        current->state = KeyRelease;
                        if (current->onKeyReleaseHandler != nullptr) {
                            current->onKeyReleaseHandler();
                        }
                        break;
                    }
                    case (KeyRelease) : {
                        current->state = KeyPress;
                        if (current->onKeyPressHandler != nullptr){
                            current->onKeyPressHandler();
                        }
                        break;
                    }
                }
            }
        }
    }
}
