#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "config.h"

/**
 * Class quản lý màn hình LCD I2C
 * Cung cấp các phương thức dễ sử dụng để hiển thị thông tin
 */
class LCDDisplay {
private:
    LiquidCrystal_I2C lcd;
    
public:
    /**
     * Constructor
     */
    LCDDisplay();
    
    /**
     * Khởi tạo LCD
     * @return true nếu thành công
     */
    bool begin();
    
    /**
     * Hiển thị màn hình khởi động
     */
    void showStartup();
    
    /**
     * Hiển thị thông báo đang TARE (reset về 0)
     */
    void showTaring();
    
    /**
     * Hiển thị thông báo sẵn sàng
     */
    void showReady();
    
    /**
     * Hiển thị trọng lượng
     * @param weight Trọng lượng (gram)
     * @param showWarning Hiển thị cảnh báo hay không
     */
    void showWeight(float weight, bool showWarning = false);
    
    /**
     * Hiển thị thông báo lỗi
     * @param message Nội dung lỗi
     */
    void showError(const char* message);
    
    /**
     * Xóa màn hình
     */
    void clear();
    
    /**
     * Bật/tắt đèn nền
     * @param on true = bật, false = tắt
     */
    void backlight(bool on = true);
};

#endif // LCD_DISPLAY_H
