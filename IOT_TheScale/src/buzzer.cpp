#include "buzzer.h"

Buzzer::Buzzer() {
    pin = BUZZER_PIN;
    isActiveHigh = BUZZER_ACTIVE_HIGH;
}

void Buzzer::begin() {
    pinMode(pin, OUTPUT);
    off();  // Tắt buzzer khi khởi động
    delay(100);
}

void Buzzer::on() {
    digitalWrite(pin, isActiveHigh ? HIGH : LOW);
}

void Buzzer::off() {
    digitalWrite(pin, isActiveHigh ? LOW : HIGH);
}

void Buzzer::beep(unsigned long duration) {
    on();
    delay(duration);
    off();
}

void Buzzer::beepPattern(int times, unsigned long duration, unsigned long interval) {
    for (int i = 0; i < times; i++) {
        on();
        delay(duration);
        off();
        
        if (i < times - 1) {  // Không delay sau lần kêu cuối
            delay(interval);
        }
    }
}
