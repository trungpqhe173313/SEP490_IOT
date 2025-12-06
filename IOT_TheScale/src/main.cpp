#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HX711.h>

/*
 * CÂN ĐIỆN TỬ 5KG
 * ESP32 + Loadcell 5kg + HX711 + LCD I2C
 */

// Cấu hình chân
#define LOADCELL_DOUT_PIN 16
#define LOADCELL_SCK_PIN 4
#define SDA_PIN 22
#define SCL_PIN 21
#define BUZZER_PIN 18

// Ngưỡng cảnh báo (gram)
#define WEIGHT_THRESHOLD 360.0

// Khởi tạo
LiquidCrystal_I2C lcd(0x27, 16, 2);
HX711 scale;

// Hệ số hiệu chuẩn (thay bằng giá trị đã hiệu chỉnh)
float calibration_factor = 360.58;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== CAN DIEN TU 5KG ===");
  
  // Khởi tạo Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  
  // Khởi tạo LCD
  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Can Dien Tu");
  lcd.setCursor(0, 1);
  lcd.print("Khoi dong...");
  
  // Khởi tạo HX711
  Serial.println("Dang khoi tao HX711...");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  delay(1000);
  
  // Kiểm tra kết nối
  if (!scale.is_ready()) {
    Serial.println("LOI: HX711 khong ket noi!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Loi HX711!");
    while(1) delay(1000);
  }
  
  Serial.println("HX711 OK!");
  
  // Đặt hệ số hiệu chuẩn
  scale.set_scale(calibration_factor);
  
  // TARE - Reset về 0
  Serial.println("Dang TARE (reset ve 0)...");
  Serial.println("Bo tat ca vat ra khoi can!");
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bo vat ra!");
  lcd.setCursor(0, 1);
  lcd.print("Dang TARE...");
  
  delay(3000);
  scale.tare();
  
  Serial.println("TARE thanh cong!");
  Serial.println("\n=== SAN SANG CAN ===\n");
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("San sang!");
  delay(1500);
}

void loop() {
  if (scale.is_ready()) {
    // Đọc trọng lượng (gram)
    float weight = scale.get_units(10);
    
    // Tự động về 0 khi < 5g
    if (abs(weight) < 5.0) {
      weight = 0;
    }
    
    // Giới hạn tối đa 5000g (5kg)
    if (weight > 5000) {
      weight = 5000;
    }
    
    // Cảnh báo nếu vượt ngưỡng - kêu nhẹ 1 tiếng bíp ngắn
    if (weight >= WEIGHT_THRESHOLD) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(50);  // Chỉ kêu 50ms (rất ngắn)
      digitalWrite(BUZZER_PIN, LOW);
    } else {
      digitalWrite(BUZZER_PIN, LOW);
    }
    
    // Hiển thị trên LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    
    if (weight < 1000) {
      lcd.print(weight, 1);
      lcd.print(" g");
    } else {
      lcd.print(weight / 1000, 3);
      lcd.print(" kg");
    }
    
    // Hiển thị cảnh báo trên LCD
    if (weight >= WEIGHT_THRESHOLD) {
      lcd.setCursor(0, 1);
      lcd.print("CANH BAO!");
    }
    
    // Hiển thị trên Serial Monitor
    Serial.print("Trong luong: ");
    if (weight < 1000) {
      Serial.print(weight, 1);
      Serial.println(" g");
    } else {
      Serial.print(weight / 1000, 3);
      Serial.println(" kg");
    }
    
  } else {
    Serial.println("Loi: HX711 khong san sang!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Loi can!");
  }
  
  delay(500);
}
