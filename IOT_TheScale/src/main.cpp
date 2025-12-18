#include <Arduino.h>
#include "config.h"
#include "lcd_display.h"
#include "scale_sensor.h"
#include "buzzer.h"
#include "api_client.h"

/*
 * CÂN ĐIỆN TỬ 5KG - KẾT NỐI API .NET
 * ESP32 + Loadcell 5kg + HX711 + LCD I2C + WiFi + API
 * 
 * Cấu trúc dự án:
 * - config.h: Cấu hình hệ thống và hằng số
 * - lcd_display: Module quản lý màn hình LCD
 * - scale_sensor: Module quản lý cảm biến cân
 * - buzzer: Module quản lý buzzer cảnh báo
 * - api_client: Module kết nối WiFi và API
 * 
 * Luồng hoạt động:
 * 1. Kết nối WiFi
 * 2. Lấy thông tin production từ API (sản phẩm nào cần đóng bao)
 * 3. Hiển thị sản phẩm hiện tại trên LCD
 * 4. Cân và cho vào bao
 * 5. Nhấn OK để chốt bao và gửi về API
 * 6. Nhấn NEXT để chuyển sang sản phẩm tiếp theo
 * 7. Nhấn TARE để reset cân về 0
 */

// Khởi tạo các module
LCDDisplay display;
ScaleSensor scale;
Buzzer buzzer;
APIClient apiClient;

// Biến lưu trữ production data
ProductionData currentProduction;
bool hasProduction = false;
int currentProductIndex = 0;
int lastProductionId = 0;  // Lưu ID để check thay đổi

// Biến cho nút bấm
unsigned long lastButtonPress[3] = {0, 0, 0};  // NEXT, OK, TARE
bool buttonState[3] = {HIGH, HIGH, HIGH};
bool lastButtonState[3] = {HIGH, HIGH, HIGH};

// Biến cho polling production
unsigned long lastProductionCheck = 0;

// Hàm kiểm tra và xử lý nút bấm
bool isButtonPressed(int buttonIndex, int buttonPin) {
  bool reading = digitalRead(buttonPin);
  
  if (reading != lastButtonState[buttonIndex]) {
    lastButtonPress[buttonIndex] = millis();
  }
  
  if ((millis() - lastButtonPress[buttonIndex]) > BUTTON_DEBOUNCE_MS) {
    if (reading != buttonState[buttonIndex]) {
      buttonState[buttonIndex] = reading;
      
      if (buttonState[buttonIndex] == LOW) {  // Nút được nhấn
        lastButtonState[buttonIndex] = reading;
        return true;
      }
    }
  }
  
  lastButtonState[buttonIndex] = reading;
  return false;
}

// Hàm lấy production từ API
void fetchProduction() {
  Serial.println("\n=== LAY THONG TIN PRODUCTION ===");
  
  if (apiClient.getCurrentProduction(currentProduction)) {
    if (currentProduction.status == "PROCESSING" && currentProduction.productCount > 0) {
      hasProduction = true;
      currentProductIndex = 0;
      lastProductionId = currentProduction.productionId;
      
      // Hiển thị sản phẩm đầu tiên
      display.showProduct(
        currentProduction.products[0].productName,
        currentProduction.products[0].targetWeight,
        1,
        currentProduction.productCount
      );
      
      buzzer.beep();
      delay(1000);
    } else {
      hasProduction = false;
      lastProductionId = 0;
      display.showWaitingProduction();
      if (currentProduction.status == "NONE") {
        Serial.println("Khong co production nao duoc gan cho thiet bi");
      } else {
        Serial.println("Khong co production hoac da COMPLETED");
      }
    }
  } else {
    hasProduction = false;
    lastProductionId = 0;
    display.showError("API!");
    Serial.println("Loi lay production tu API");
  }
}

// Hàm check production có thay đổi không (dùng cho polling)
void checkProductionStatus() {
  ProductionData tempData;
  
  if (!apiClient.getCurrentProduction(tempData)) {
    // Lỗi API - không làm gì
    Serial.println("Loi check production status");
    return;
  }
  
  // Nếu đang có production
  if (hasProduction) {
    // Check xem production hiện tại còn PROCESSING không
    if (tempData.productionId == lastProductionId && tempData.status == "PROCESSING") {
      // Vẫn như cũ - không làm gì
      Serial.println("Production van con PROCESSING - khong thay doi");
      return;
    } else {
      // Production đã thay đổi hoặc kết thúc
      Serial.println("Production da thay doi hoac ket thuc!");
      fetchProduction();  // Refresh lại
    }
  } else {
    // Đang không có production - check có production mới không
    if (tempData.status == "PROCESSING" && tempData.productCount > 0) {
      Serial.println("Co production moi!");
      fetchProduction();  // Có production mới - hiển thị
    } else {
      Serial.println("Van chua co production");
    }
  }
}

void setup() {
  // Khởi tạo Serial
  Serial.begin(SERIAL_BAUD_RATE);
  delay(1000);
  Serial.println("\n=== CAN DIEN TU 5KG - API .NET ===");
  
  // Khởi tạo các nút bấm
  pinMode(BUTTON_PIN_NEXT, INPUT_PULLUP);
  pinMode(BUTTON_PIN_OK, INPUT_PULLUP);
  pinMode(BUTTON_PIN_TARE, INPUT_PULLUP);
  
  // Khởi tạo Buzzer
  buzzer.begin();
  
  // Khởi tạo LCD
  display.begin();
  display.showStartup();
  delay(2000);
  
  // Khởi tạo cảm biến cân
  if (!scale.begin()) {
    display.showError("HX711!");
    while(1) delay(1000);
  }
  
  // TARE - Reset về 0
  display.showTaring();
  delay(2000);
  scale.tare();
  
  // Kết nối WiFi
  display.showConnectingWiFi();
  if (!apiClient.connectWiFi()) {
    display.showWiFiError();
    Serial.println("Loi ket noi WiFi! Khoi dong lai...");
    delay(5000);
    ESP.restart();
  }
  
  display.showWiFiConnected();
  delay(2000);
  
  // Lấy thông tin production
  fetchProduction();
  
  Serial.println("\n=== SAN SANG CAN ===\n");
}

void loop() {
  // Polling - Tự động kiểm tra production mỗi 60s
  if (millis() - lastProductionCheck > PRODUCTION_CHECK_INTERVAL_MS) {
    Serial.println("\n>>> AUTO CHECK PRODUCTION <<<");
    checkProductionStatus();  // Chỉ check thay đổi, không refresh
    lastProductionCheck = millis();
  }
  
  // Xử lý nút TARE - Reset cân về 0 và lấy production mới
  if (isButtonPressed(2, BUTTON_PIN_TARE)) {
    Serial.println("\n>>> NUT TARE <<<");
    display.showTaring();
    delay(1000);
    scale.tare();
    buzzer.beep();
    
    // Lấy lại production mới
    fetchProduction();
    lastProductionCheck = millis();  // Reset timer
    delay(500);
    return;
  }
  
  // Nếu không có production, chỉ hiển thị chờ
  if (!hasProduction) {
    display.showWaitingProduction();
    delay(1000);
    return;
  }
  
  // Xử lý nút NEXT - Chuyển sang sản phẩm tiếp theo
  if (isButtonPressed(0, BUTTON_PIN_NEXT)) {
    Serial.println("\n>>> NUT NEXT <<<");
    currentProductIndex++;
    if (currentProductIndex >= currentProduction.productCount) {
      currentProductIndex = 0;  // Quay lại sản phẩm đầu tiên
    }
    
    display.showProduct(
      currentProduction.products[currentProductIndex].productName,
      currentProduction.products[currentProductIndex].targetWeight,
      currentProductIndex + 1,
      currentProduction.productCount
    );
    
    buzzer.beep();
    
    Serial.print("Chuyen sang san pham: ");
    Serial.println(currentProduction.products[currentProductIndex].productName);
    
    delay(1500);
    return;
  }
  
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
  
  // Xử lý nút OK - Chốt bao và gửi về API
  if (isButtonPressed(1, BUTTON_PIN_OK)) {
    Serial.println("\n>>> NUT OK - CHOT BAO <<<");
    
    // Kiểm tra trọng lượng tối thiểu
    if (weight < MIN_WEIGHT) {
      Serial.println("Trong luong qua nho! Khong gui.");
      display.showError("Qua nhe!");
      buzzer.beep();
      delay(1500);
      return;
    }
    
    // Hiển thị đang gửi
    display.showPackageConfirmed();
    buzzer.beep();
    delay(200);
    buzzer.beep();
    
    // Gửi dữ liệu về API
    SubmitResult result = apiClient.submitPackage(
      currentProduction.productionId,
      currentProduction.products[currentProductIndex].productId,
      weight,
      1  // 1 bao
    );
    
    if (result.success) {
      // 201 - Success
      Serial.println("GUI THANH CONG!");
            
      // Beep 3 lần báo thành công
      for (int i = 0; i < 3; i++) {
        buzzer.beep();
        delay(100);
      }
      
      // Hiển thị lại sản phẩm hiện tại
      display.showProduct(
        currentProduction.products[currentProductIndex].productName,
        currentProduction.products[currentProductIndex].targetWeight,
        currentProductIndex + 1,
        currentProduction.productCount
      );
      
      delay(1500);
    } else if (result.statusCode == 409) {
      // 409 - Production đã kết thúc
      Serial.println("Production da ket thuc! Lay production moi...");
      display.showError("Ca ket thuc!");
      buzzer.beep();
      delay(2000);
      
      hasProduction = false;
      fetchProduction();  // Tự động lấy production mới
      lastProductionCheck = millis();  // Reset timer
      
    } else if (result.statusCode == 403) {
      // 403 - Device không được assign
      Serial.println("Device khong duoc gan! Quet lai QR...");
      display.showError("Quet lai QR!");
      buzzer.beep();
      delay(3000);
      
      hasProduction = false;
      display.showWaitingProduction();
      
    } else if (result.statusCode == 400) {
      // 400 - Lỗi cấu hình
      Serial.println("Loi cau hinh Device/Production!");
      display.showError("Loi cau hinh!");
      buzzer.beep();
      delay(5000);
      
    } else {
      // Lỗi khác
      Serial.println("GUI THAT BAI!");
      display.showError("GUI loi!");
      buzzer.beep();
      delay(2000);
    }
    
    return;
  }
  
  // Hiển thị trọng lượng với target và tên sản phẩm
  float targetWeight = currentProduction.products[currentProductIndex].targetWeight;
  display.showProductWithWeight(
    currentProduction.products[currentProductIndex].productName,
    weight,
    targetWeight,
    currentProductIndex + 1,
    currentProduction.productCount
  );
  
  // Cảnh báo nếu vượt target + 10%
  if (weight > targetWeight * 1.1) {
    buzzer.beep();
  }
  
  // Hiển thị trên Serial Monitor
  Serial.print("San pham: ");
  Serial.print(currentProduction.products[currentProductIndex].productName);
  Serial.print(" | Can: ");
  Serial.print(weight, 1);
  Serial.print("g / Target: ");
  Serial.print(targetWeight, 0);
  Serial.println("g");
  
  delay(SCALE_READ_DELAY);
}
