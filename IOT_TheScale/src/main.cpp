#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Cấu hình LCD I2C
#define SDA_PIN 22  // SDA -> GPIO 22
#define SCL_PIN 21  // SCL -> GPIO 21

// Khởi tạo LCD (địa chỉ 0x27, 16 cột, 2 hàng)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Giả lập trọng lượng
float weight = 0;
int counter = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== TEST LCD I2C ===");
  Serial.println("Ket noi:");
  Serial.println("- SDA -> GPIO 22");
  Serial.println("- SCL -> GPIO 21");
  Serial.println("- VCC -> 5V");
  Serial.println("- GND -> GND");
  
  // Khởi tạo I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  
  // Khởi tạo LCD
  lcd.init();
  lcd.backlight();
  
  // Hiển thị khởi động
  lcd.setCursor(0, 0);
  lcd.print("Can Dien Tu");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  
  Serial.println("LCD da khoi tao!");
  Serial.println("Van bien tro neu khong thay chu!");
  
  delay(2000);
  
  lcd.clear();
}

void loop() {
  // Giả lập trọng lượng tăng dần
  weight = counter * 10.5;
  
  // Hiển thị trên LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Trong luong:");
  lcd.setCursor(0, 1);
  
  if (weight < 1000) {
    lcd.print(weight, 1);
    lcd.print(" g   ");
  } else {
    lcd.print(weight / 1000, 2);
    lcd.print(" kg  ");
  }
  
  // Hiển thị trên Serial
  Serial.print("Hien thi: ");
  Serial.print(weight, 1);
  Serial.println(" g");
  
  counter++;
  if (counter > 100) {
    counter = 0;
  }
  
  delay(1000);
}

/*
 * CODE DEMO NÀY CHỈ ĐỂ TEST!
 * 
 * Để cân thật hoạt động, bạn PHẢI:
 * 1. Nối 4 dây loadcell vào HX711
 * 2. Nối HX711 vào ESP32
 * 3. Sử dụng code thật với thư viện HX711
 * 
 * KHÔNG THỂ thay thế phần cứng bằng code!
 */

/*
 * HƯỚNG DẪN HIỆU CHUẨN:
 * 
 * 1. Upload code này lên ESP32
 * 2. Mở Serial Monitor (115200 baud)
 * 3. Đặt vật có khối lượng đã biết lên cân (ví dụ: 100g, 500g, 1kg)
 * 4. Ghi lại giá trị hiển thị
 * 5. Tính calibration_factor mới:
 *    calibration_factor_mới = calibration_factor_cũ * (khối_lượng_thực / khối_lượng_đọc_được)
 * 6. Thay đổi giá trị calibration_factor ở trên và upload lại
 * 7. Lặp lại cho đến khi chính xác
 * 
 * LƯU Ý:
 * - Module HX711 có màn LED 7 đoạn thường chạy độc lập với ESP32
 * - Màn LED hiển thị giá trị từ chip HX711 trực tiếp
 * - Trọng lượng chính xác sẽ hiển thị trên Serial Monitor
 */

/*
 * HƯỚNG DẪN HIỆU CHUẨN:
 * 
 * 1. Upload code này lên ESP32
 * 2. Mở Serial Monitor (115200 baud)
 * 3. Đặt vật có khối lượng đã biết lên cân (ví dụ: 100g, 500g, 1kg)
 * 4. Ghi lại giá trị hiển thị
 * 5. Tính calibration_factor mới:
 *    calibration_factor_mới = calibration_factor_cũ * (khối_lượng_thực / khối_lượng_đọc_được)
 * 6. Thay đổi giá trị calibration_factor ở trên và upload lại
 * 7. Lặp lại cho đến khi chính xác
 * 
 * KẾT NỐI PHẦN CỨNG:
 * 
 * HX711 -> ESP32:
 *   VCC -> 3.3V hoặc 5V
 *   GND -> GND
 *   DT  -> GPIO 16
 *   SCK -> GPIO 4
 * 
 * LCD I2C -> ESP32:
 *   VCC -> 5V
 *   GND -> GND
 *   SDA -> GPIO 21 (SDA mặc định)
 *   SCL -> GPIO 22 (SCL mặc định)
 * LCD I2C -> ESP32:
 *   VCC -> 5V
 *   GND -> GND
 *   SDA -> GPIO 21 (SDA mặc định)
 *   SCL -> GPIO 22 (SCL mặc định)
 */