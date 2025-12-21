#ifndef CONFIG_H
#define CONFIG_H

/*
 * CẤU HÌNH HỆ THỐNG CÂN ĐIỆN TỬ
 * Tất cả các hằng số và cấu hình cứng của dự án
 */

// ===== CẤU HÌNH PHẦN CỨNG =====
// Chân kết nối HX711 (Loadcell Amplifier)
#define LOADCELL_DOUT_PIN   16
#define LOADCELL_SCK_PIN    4

// Chân kết nối I2C cho LCD
#define SDA_PIN             22
#define SCL_PIN             21

// Chân điều khiển Buzzer
#define BUZZER_PIN          18

// Chân các nút bấm
#define BUTTON_PIN_NEXT     19  // Nút NEXT (Chuyển sản phẩm)
#define BUTTON_PIN_OK       23  // Nút OK (Chốt bao)
#define BUTTON_PIN_TARE     14  // Nút TARE (Đặt về 0)

// Địa chỉ I2C của LCD
#define LCD_ADDRESS         0x27
#define LCD_COLS            16
#define LCD_ROWS            2

// ===== CẤU HÌNH CÂN =====
// Hệ số hiệu chuẩn loadcell (thay bằng giá trị thực tế sau khi hiệu chỉnh)
#define CALIBRATION_FACTOR  360.58f

// Trọng lượng tối thiểu để coi là có vật (gram)
#define MIN_WEIGHT          1.0f

// Trọng lượng tối đa của cân (gram)
#define MAX_WEIGHT          5000.0f

// Ngưỡng cảnh báo (gram)
#define WEIGHT_THRESHOLD    360.0f

// Ngưỡng sai số cho phép (gram) - chỉ gửi khi trong khoảng ±500g so với target
#define WEIGHT_TOLERANCE    500.0f

// ===== CẤU HÌNH GIAO TIẾP =====
// Tốc độ Serial Monitor
#define SERIAL_BAUD_RATE    115200

// Thời gian đọc cân (milliseconds)
#define SCALE_READ_DELAY    500

// Số lần lấy mẫu trung bình khi đọc cân
#define SCALE_SAMPLES       10

// ===== CẤU HÌNH BUZZER =====
// Thời gian kêu buzzer (milliseconds)
#define BUZZER_BEEP_DURATION    50

// Logic buzzer (true nếu HIGH = kêu, false nếu LOW = kêu)
#define BUZZER_ACTIVE_HIGH      false

// ===== CẤU HÌNH WIFI =====
#define WIFI_SSID               "Trung's Galaxy A14"
#define WIFI_PASSWORD           "12345678"
#define WIFI_TIMEOUT_MS         20000  // 20 giây timeout

// ===== CẤU HÌNH API =====
#define API_BASE_URL            "http://10.183.225.128:5224"
#define API_TIMEOUT_MS          5000
#define DEVICE_CODE             "SCALE_01"
#define PRODUCTION_CHECK_INTERVAL_MS  10000  // Kiểm tra production mỗi 10s

// Endpoints
#define API_GET_PRODUCTION      "/api/iot/production/current"
#define API_POST_PACKAGE        "/api/iot/packages"

// ===== CẤU HÌNH NÚT BẤM =====
#define BUTTON_DEBOUNCE_MS      50     // Chống dội nút
#define BUTTON_LONG_PRESS_MS    1000   // Nhấn giữ 1s

#endif // CONFIG_H
