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

void LCDDisplay::showConnectingWiFi() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ket noi WiFi...");
}

void LCDDisplay::showWiFiConnected() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi OK!");
}

void LCDDisplay::showWiFiError() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Loi WiFi!");
    lcd.setCursor(0, 1);
    lcd.print("Kiem tra lai");
}

void LCDDisplay::showProduct(const String& productName, float targetWeight, int currentIndex, int totalProducts) {
    lcd.clear();
    
    // Dòng 1: Tên sản phẩm (cắt bớt nếu quá dài)
    lcd.setCursor(0, 0);
    String displayName = productName;
    if (displayName.length() > 16) {
        displayName = displayName.substring(0, 13) + "...";
    }
    lcd.print(displayName);
    
    // Dòng 2: Target weight và số thứ tự
    lcd.setCursor(0, 1);
    lcd.print(targetWeight, 0);
    lcd.print("g ");
    lcd.print("[");
    lcd.print(currentIndex);
    lcd.print("/");
    lcd.print(totalProducts);
    lcd.print("]");
}

void LCDDisplay::showWeightWithTarget(float currentWeight, float targetWeight) {
    lcd.clear();
    
    // Dòng 1: Trọng lượng hiện tại
    lcd.setCursor(0, 0);
    lcd.print("Can: ");
    lcd.print(currentWeight, 1);
    lcd.print("g");
    
    // Dòng 2: Target và % đạt được
    lcd.setCursor(0, 1);
    lcd.print("Muc tieu:");
    lcd.print(targetWeight, 0);
    lcd.print("g");
}

void LCDDisplay::showPackageConfirmed() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Da chot bao!");
    lcd.setCursor(0, 1);
    lcd.print("Dang gui...");
}

void LCDDisplay::showWaitingProduction() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cho production");
    lcd.setCursor(0, 1);
    lcd.print("hoac an TARE");
}

void LCDDisplay::showProductWithWeight(const String& productName, float currentWeight, float targetWeight, int currentIndex, int totalProducts) {
    lcd.clear();
    
    // Dòng 1: Tên sản phẩm (rút gọn) + [X/Y]
    lcd.setCursor(0, 0);
    String displayName = productName;
    
    // Tính số ký tự còn lại cho tên sản phẩm (16 - " [X/Y]")
    int indexChars = 4;  // " [1/1]" tối thiểu
    if (totalProducts >= 10 || currentIndex >= 10) indexChars = 5;  // " [10/1]" hoặc " [1/10]"
    if (totalProducts >= 10 && currentIndex >= 10) indexChars = 6;  // " [10/10]"
    
    int maxNameLen = 16 - indexChars;
    if (displayName.length() > maxNameLen) {
        displayName = displayName.substring(0, maxNameLen - 2) + "..";
    }
    
    lcd.print(displayName);
    
    // Hiển thị [X/Y]
    lcd.print(" [");
    lcd.print(currentIndex);
    lcd.print("/");
    lcd.print(totalProducts);
    lcd.print("]");
    
    // Dòng 2: 1.2/50kg  82% hoặc 800/900g  88%
    lcd.setCursor(0, 1);

    int percent = 0;
    if (targetWeight > 0) {
        percent = (int)((currentWeight / targetWeight) * 100);
        if (percent > 999) percent = 999;
    }

    String weightStr;
    if (targetWeight >= 1000.0f) {
        // Hiển thị kg
        float curKg = currentWeight / 1000.0f;
        float tarKg = targetWeight / 1000.0f;
        weightStr = String(curKg, 1) + "/" + String(tarKg, 0) + "kg";
    } else {
        // Hiển thị gram
        weightStr = String((int)currentWeight) + "/" + String((int)targetWeight) + "g";
    }

    lcd.print(weightStr);

    int spaces = 16 - weightStr.length() - String(percent).length() - 1;
    if (spaces < 1) spaces = 1;
    for (int i = 0; i < spaces; i++) {
        lcd.print(" ");
    }
    lcd.print(percent);
    lcd.print("%");
}
