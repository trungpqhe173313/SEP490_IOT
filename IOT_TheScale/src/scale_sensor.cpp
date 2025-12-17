#include "scale_sensor.h"
#include <Arduino.h>

ScaleSensor::ScaleSensor() {
    calibrationFactor = CALIBRATION_FACTOR;
}

bool ScaleSensor::begin() {
    Serial.println("Dang khoi tao HX711...");
    
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    delay(1000);
    
    if (!scale.is_ready()) {
        Serial.println("LOI: HX711 khong ket noi!");
        return false;
    }
    
    Serial.println("HX711 OK!");
    scale.set_scale(calibrationFactor);
    
    return true;
}

bool ScaleSensor::isReady() {
    return scale.is_ready();
}

void ScaleSensor::tare() {
    Serial.println("Dang TARE (reset ve 0)...");
    Serial.println("Bo tat ca vat ra khoi can!");
    
    scale.tare();
    
    Serial.println("TARE thanh cong!");
}

float ScaleSensor::readWeight(int samples) {
    if (!scale.is_ready()) {
        return -1.0f;
    }
    
    float weight = scale.get_units(samples);
    return processWeight(weight);
}

void ScaleSensor::setCalibrationFactor(float factor) {
    calibrationFactor = factor;
    scale.set_scale(calibrationFactor);
}

float ScaleSensor::getCalibrationFactor() {
    return calibrationFactor;
}

float ScaleSensor::processWeight(float weight) {
    // Tự động về 0 khi < MIN_WEIGHT
    if (abs(weight) < MIN_WEIGHT) {
        weight = 0;
    }
    
    // Giới hạn tối đa
    if (weight > MAX_WEIGHT) {
        weight = MAX_WEIGHT;
    }
    
    // Giới hạn tối thiểu (không âm)
    if (weight < 0) {
        weight = 0;
    }
    
    return weight;
}
