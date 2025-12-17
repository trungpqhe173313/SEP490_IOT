#include "lcd_display.h"

LCDDisplay::LCDDisplay() : lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS) {
}

bool LCDDisplay::begin() {
    Wire.begin(SDA_PIN, SCL_PIN);
    lcd.init();
    lcd.backlight();
    return true;
}

void LCDDisplay::showStartup() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Can Dien Tu");
    lcd.setCursor(0, 1);
    lcd.print("Khoi dong...");
}

void LCDDisplay::showTaring() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bo vat ra!");
    lcd.setCursor(0, 1);
    lcd.print("Dang TARE...");
}

void LCDDisplay::showReady() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("San sang!");
}

void LCDDisplay::showWeight(float weight, bool showWarning) {
    lcd.clear();
    lcd.setCursor(0, 0);
    
    // Hiển thị trọng lượng
    if (weight < 1000) {
        // Hiển thị gram
        lcd.print(weight, 1);
        lcd.print(" g");
    } else {
        // Hiển thị kg
        lcd.print(weight / 1000, 3);
        lcd.print(" kg");
    }
    
    // Hiển thị cảnh báo nếu cần
    if (showWarning) {
        lcd.setCursor(0, 1);
        lcd.print("CANH BAO!");
    }
}

void LCDDisplay::showError(const char* message) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Loi:");
    lcd.setCursor(0, 1);
    lcd.print(message);
}

void LCDDisplay::clear() {
    lcd.clear();
}

void LCDDisplay::backlight(bool on) {
    if (on) {
        lcd.backlight();
    } else {
        lcd.noBacklight();
    }
}
