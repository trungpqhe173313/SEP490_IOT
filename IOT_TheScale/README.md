# CÂN ĐIỆN TỬ IOT - Cấu trúc dự án

## 📁 Cấu trúc dự án

```
IOT_TheScale/
├── platformio.ini          # Cấu hình PlatformIO
├── include/                # Header files
│   ├── config.h           # Cấu hình hệ thống và hằng số
│   ├── lcd_display.h      # Module quản lý màn hình LCD
│   ├── scale_sensor.h     # Module quản lý cảm biến cân
│   └── buzzer.h           # Module quản lý buzzer
├── src/                    # Source files
│   ├── main.cpp           # File chính
│   ├── lcd_display.cpp    # Implementation LCD
│   ├── scale_sensor.cpp   # Implementation cảm biến cân
│   └── buzzer.cpp         # Implementation buzzer
├── lib/                    # Thư viện bên ngoài
└── test/                   # Test files
```

## 🎯 Mô tả các module

### 1. **config.h**
Chứa tất cả cấu hình cứng của hệ thống:
- Định nghĩa chân GPIO
- Thông số cảm biến
- Ngưỡng cảnh báo
- Cấu hình giao tiếp

### 2. **lcd_display (Header + Implementation)**
Module quản lý màn hình LCD I2C:
- `begin()`: Khởi tạo LCD
- `showStartup()`: Hiển thị màn hình khởi động
- `showTaring()`: Hiển thị đang TARE
- `showReady()`: Hiển thị sẵn sàng
- `showWeight()`: Hiển thị trọng lượng
- `showError()`: Hiển thị lỗi

### 3. **scale_sensor (Header + Implementation)**
Module quản lý cảm biến cân Loadcell + HX711:
- `begin()`: Khởi tạo cảm biến
- `isReady()`: Kiểm tra trạng thái
- `tare()`: Reset cân về 0
- `readWeight()`: Đọc trọng lượng
- `setCalibrationFactor()`: Đặt hệ số hiệu chuẩn
- `processWeight()`: Xử lý dữ liệu trọng lượng

### 4. **buzzer (Header + Implementation)**
Module điều khiển buzzer cảnh báo:
- `begin()`: Khởi tạo buzzer
- `on()`: Bật buzzer
- `off()`: Tắt buzzer
- `beep()`: Kêu 1 tiếng bíp ngắn
- `beepPattern()`: Kêu theo mẫu

### 5. **main.cpp**
File chính điều phối các module:
- `setup()`: Khởi tạo hệ thống
- `loop()`: Vòng lặp chính đọc cân và hiển thị

## ✨ Ưu điểm của cấu trúc mới

1. **Tách biệt trách nhiệm**: Mỗi module chỉ làm một nhiệm vụ cụ thể
2. **Dễ bảo trì**: Thay đổi một module không ảnh hưởng các module khác
3. **Dễ test**: Có thể test từng module độc lập
4. **Dễ mở rộng**: Thêm tính năng mới dễ dàng hơn
5. **Code sạch hơn**: main.cpp ngắn gọn, dễ hiểu
6. **Tái sử dụng**: Các module có thể dùng cho dự án khác

## 🔧 Hướng dẫn sử dụng

### Thay đổi cấu hình
Chỉnh sửa các hằng số trong [include/config.h](include/config.h)

### Thêm tính năng LCD mới
Thêm phương thức vào class `LCDDisplay` trong [include/lcd_display.h](include/lcd_display.h)

### Hiệu chỉnh cân
Gọi `scale.setCalibrationFactor(new_value)` trong [src/main.cpp](src/main.cpp)

### Thay đổi mẫu kêu buzzer
Sử dụng `buzzer.beepPattern(times, duration, interval)` trong [src/main.cpp](src/main.cpp)

## 📝 Build và Upload

```bash
# Build project
pio run

# Upload to ESP32
pio run --target upload

# Monitor Serial
pio device monitor
```

## 🛠️ Phát triển tiếp

Các tính năng có thể thêm:
- [ ] Kết nối WiFi và IoT Cloud
- [ ] Lưu lịch sử cân vào SPIFFS/SD Card
- [ ] Web server để xem dữ liệu
- [ ] MQTT để gửi dữ liệu real-time
- [ ] Chế độ hiệu chỉnh cân qua Serial
