#ifndef API_CLIENT_H
#define API_CLIENT_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"

// Cấu trúc sản phẩm
struct Product {
    int productId;
    String productName;
    float targetWeight;  // Trọng lượng mục tiêu (GRAM) - API trả về KG, sẽ convert sang gram
};

// Cấu trúc Production hiện tại
struct ProductionData {
    String status;
    int productionId;
    int productCount;
    Product products[10];  // Tối đa 10 sản phẩm
};

// Kết quả submit package
struct SubmitResult {
    bool success;
    int statusCode;
    String message;
};

class APIClient {
private:
    HTTPClient http;
    String baseUrl;
    String deviceCode;
    
public:
    APIClient();
    
    // Kết nối WiFi
    bool connectWiFi();
    bool isWiFiConnected();
    
    // API calls
    bool getCurrentProduction(ProductionData &data);
    SubmitResult submitPackage(int productionId, int productId, float weight, int packageCount = 1);
    
    // Helper functions
    void disconnect();
};

#endif // API_CLIENT_H
