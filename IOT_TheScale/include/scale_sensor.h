#ifndef SCALE_SENSOR_H
#define SCALE_SENSOR_H

#include <HX711.h>
#include "config.h"

/**
 * Class quản lý cảm biến cân (Loadcell + HX711)
 */
class ScaleSensor {
private:
    HX711 scale;
    float calibrationFactor;
    
public:
    /**
     * Constructor
     */
    ScaleSensor();
    
    /**
     * Khởi tạo cảm biến cân
     * @return true nếu thành công
     */
    bool begin();
    
    /**
     * Kiểm tra cảm biến có sẵn sàng không
     * @return true nếu sẵn sàng
     */
    bool isReady();
    
    /**
     * TARE - Reset cân về 0
     * Gọi hàm này khi không có vật trên cân
     */
    void tare();
    
    /**
     * Đọc trọng lượng hiện tại
     * @param samples Số lần lấy mẫu để tính trung bình
     * @return Trọng lượng (gram)
     */
    float readWeight(int samples = SCALE_SAMPLES);
    
    /**
     * Đặt hệ số hiệu chuẩn mới
     * @param factor Hệ số hiệu chuẩn
     */
    void setCalibrationFactor(float factor);
    
    /**
     * Lấy hệ số hiệu chuẩn hiện tại
     * @return Hệ số hiệu chuẩn
     */
    float getCalibrationFactor();
    
    /**
     * Xử lý trọng lượng đọc được (giới hạn min/max, làm tròn)
     * @param weight Trọng lượng thô
     * @return Trọng lượng sau khi xử lý
     */
    float processWeight(float weight);
};

#endif // SCALE_SENSOR_H
