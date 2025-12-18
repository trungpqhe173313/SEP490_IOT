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
    
    /**
     * Hiển thị thông tin kết nối WiFi
     */
    void showConnectingWiFi();
    void showWiFiConnected();
    void showWiFiError();
    
    /**
     * Hiển thị thông tin sản phẩm
     * @param productName Tên sản phẩm
     * @param targetWeight Trọng lượng mục tiêu (gram)
     * @param currentIndex Chỉ số sản phẩm hiện tại (1-based)
     * @param totalProducts Tổng số sản phẩm
     */
    void showProduct(const String& productName, float targetWeight, int currentIndex, int totalProducts);
    
    /**
     * Hiển thị trọng lượng đang cân với target
     * @param currentWeight Trọng lượng hiện tại
     * @param targetWeight Trọng lượng mục tiêu
     */
    void showWeightWithTarget(float currentWeight, float targetWeight);
    
    /**
     * Hiển thị đầy đủ: tên sản phẩm + trọng lượng + %
     * Format: Dòng 1: Tên sản phẩm [X/Y]
     *         Dòng 2: 1234/1500g  82%
     * @param productName Tên sản phẩm
     * @param currentWeight Trọng lượng hiện tại (gram)
     * @param targetWeight Trọng lượng mục tiêu (gram)
     * @param currentIndex Chỉ số sản phẩm hiện tại (1-based)
     * @param totalProducts Tổng số sản phẩm
     */
    void showProductWithWeight(const String& productName, float currentWeight, float targetWeight, int currentIndex, int totalProducts);
    
    /**
     * Hiển thị thông báo đã chốt bao
     */
    void showPackageConfirmed();
    
    /**
     * Hiển thị thông báo chờ production
     */
    void showWaitingProduction();
};

#endif // LCD_DISPLAY_H
