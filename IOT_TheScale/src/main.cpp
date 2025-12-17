#include <Arduino.h>
#include "config.h"
#include "lcd_display.h"
#include "scale_sensor.h"
#include "buzzer.h"

/*
 * CÂN ĐIỆN TỬ 5KG
 * ESP32 + Loadcell 5kg + HX711 + LCD I2C
 * 
 * Cấu trúc dự án:
 * - config.h: Cấu hình hệ thống và hằng số
 * - lcd_display: Module quản lý màn hình LCD
 * - scale_sensor: Module quản lý cảm biến cân
 * - buzzer: Module quản lý buzzer cảnh báo
 */

// Khởi tạo các module
LCDDisplay display;
ScaleSensor scale;
Buzzer buzzer;

void setup() {
  // Khởi tạo Serial
  Serial.begin(SERIAL_BAUD_RATE);
  delay(1000);
  Serial.println("\n=== CAN DIEN TU 5KG ===");
  
  // Khởi tạo Buzzer
  buzzer.begin();
  
  // Khởi tạo LCD
  display.begin();
  display.showStartup();
  
  // Khởi tạo cảm biến cân
  if (!scale.begin()) {
    display.showError("HX711!");
    while(1) delay(1000);
  }
  
  // TARE - Reset về 0
  display.showTaring();
  delay(2000);
  scale.tare();
  
  Serial.println("\n=== SAN SANG CAN ===\n");
  display.showReady();
  delay(1500);
}

void loop() {
  // Kiểm tra cảm biến có sẵn sàng không
  if (!scale.isReady()) {
    Serial.println("Loi: HX711 khong san sang!");
    display.showError("Can!");
    delay(SCALE_READ_DELAY);
    return;
  }
  
  // Đọc trọng lượng
  float weight = scale.readWeight();
  
  // Kiểm tra lỗi đọc cân
  if (weight < 0) {
    display.showError("Doc can!");
    delay(SCALE_READ_DELAY);
    return;
  }
  
  // Kiểm tra và cảnh báo nếu vượt ngưỡng
  bool isWarning = (weight >= WEIGHT_THRESHOLD);
  if (isWarning) {
    buzzer.beep();
  }
  
  // Hiển thị trên LCD
  display.showWeight(weight, isWarning);
  
  // Hiển thị trên Serial Monitor
  Serial.print("Trong luong: ");
  if (weight < 1000) {
    Serial.print(weight, 1);
    Serial.println(" g");
  } else {
    Serial.print(weight / 1000, 3);
    Serial.println(" kg");
  }
  
  delay(SCALE_READ_DELAY);
}
