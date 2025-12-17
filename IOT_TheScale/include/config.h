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

// Địa chỉ I2C của LCD
#define LCD_ADDRESS         0x27
#define LCD_COLS            16
#define LCD_ROWS            2

// ===== CẤU HÌNH CÂN =====
// Hệ số hiệu chuẩn loadcell (thay bằng giá trị thực tế sau khi hiệu chỉnh)
#define CALIBRATION_FACTOR  360.58f

// Trọng lượng tối thiểu để coi là có vật (gram)
#define MIN_WEIGHT          5.0f

// Trọng lượng tối đa của cân (gram)
#define MAX_WEIGHT          5000.0f

// Ngưỡng cảnh báo (gram)
#define WEIGHT_THRESHOLD    360.0f

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

#endif // CONFIG_H
