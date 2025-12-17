#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>
#include "config.h"

/**
 * Class quản lý buzzer cảnh báo
 */
class Buzzer {
private:
    uint8_t pin;
    bool isActiveHigh;
    
public:
    /**
     * Constructor
     */
    Buzzer();
    
    /**
     * Khởi tạo buzzer
     */
    void begin();
    
    /**
     * Bật buzzer
     */
    void on();
    
    /**
     * Tắt buzzer
     */
    void off();
    
    /**
     * Kêu buzzer một tiếng bíp ngắn
     * @param duration Thời gian kêu (milliseconds)
     */
    void beep(unsigned long duration = BUZZER_BEEP_DURATION);
    
    /**
     * Kêu buzzer theo mẫu (số lần và khoảng thời gian)
     * @param times Số lần kêu
     * @param duration Thời gian mỗi lần kêu
     * @param interval Khoảng cách giữa các lần kêu
     */
    void beepPattern(int times, unsigned long duration, unsigned long interval);
};

#endif // BUZZER_H
