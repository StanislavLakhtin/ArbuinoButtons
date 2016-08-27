//
// Created by Stanislav Lakhtin on 23/07/16.
//

#ifndef YAO_GUAI_BUTTONS_H
#define YAO_GUAI_BUTTONS_H

#include <Arduino.h>

#define MAXLISTENERS 4
#define THRESHOLD 250 //in millisec

enum KeyEventType {KeyPress, KeyRelease};
enum KeyState {Released=1, Pressed=0};

struct Listener {
    int pin;
    void (*onKeyPressHandler)(void);
    void (*onKeyReleaseHandler)(void);
    int state;
    long lastChange;
};

// основная идея в том, чтобы разгрузить микроконтроллер, избавив от опропса клавиш
// перенаправив вызовы на прерывание от всех клавиш сразу
class Buttons {
private:
    Listener  listener[MAXLISTENERS];
    int   findByPin(int pin);
public:
    Buttons();
    void processIt(void);
    bool AddEventListener(int pin, void (*keyPressHandler)(void), void (*keyReleaseHandler)(void));
    void ReleaseEventListeners(int pin);
};


#endif //YAO_GUAI_BUTTONS_H
