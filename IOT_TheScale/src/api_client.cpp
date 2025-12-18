#include "api_client.h"

APIClient::APIClient() {
    baseUrl = String(API_BASE_URL);
    deviceCode = String(DEVICE_CODE);
}

bool APIClient::connectWiFi() {
    Serial.println("\n=== KET NOI WIFI ===");
    Serial.print("SSID: ");
    Serial.println(WIFI_SSID);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - startTime > WIFI_TIMEOUT_MS) {
            Serial.println("\nLoi: WiFi timeout!");
            return false;
        }
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nWiFi da ket noi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    return true;
}

bool APIClient::isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

bool APIClient::getCurrentProduction(ProductionData &data) {
    if (!isWiFiConnected()) {
        Serial.println("Loi: WiFi khong ket noi!");
        return false;
    }
    
    // Tạo URL với device code
    String url = baseUrl + String(API_GET_PRODUCTION) + "?deviceCode=" + deviceCode;
    
    Serial.println("\n=== GOI API LAY PRODUCTION ===");
    Serial.print("URL: ");
    Serial.println(url);
    
    http.begin(url);
    http.setTimeout(API_TIMEOUT_MS);
    http.addHeader("Content-Type", "application/json");
    
    int httpCode = http.GET();
    String payload = http.getString();
    http.end();
    
    Serial.print("HTTP Code: ");
    Serial.println(httpCode);
    Serial.println("Response:");
    Serial.println(payload);
    
    // Parse JSON
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (error) {
        Serial.print("Loi parse JSON: ");
        Serial.println(error.c_str());
        return false;
    }
    
    // Nếu 404 → không có production (trạng thái hợp lệ)
    if (httpCode == 404) {
        Serial.println("Khong co production duoc gan (404)");
        data.status = "NONE";
        data.productionId = 0;
        data.productCount = 0;
        return true;
    }

    // Nếu lỗi HTTP khác
    if (httpCode != HTTP_CODE_OK) {
        Serial.print("Loi HTTP: ");
        Serial.println(httpCode);
        return false;
    }

    // HTTP 200 OK - Lấy dữ liệu (không có wrapper "data")
    data.status = doc["status"].as<String>();
    data.productionId = doc["productionId"];
    
    JsonArray productsArray = doc["products"].as<JsonArray>();
    data.productCount = productsArray.size();
    
    int index = 0;
    for (JsonObject product : productsArray) {
        if (index >= 10) break;  // Giới hạn 10 sản phẩm
        
        data.products[index].productId = product["productId"];
        data.products[index].productName = product["productName"].as<String>();
        
        // API trả về KG, convert sang GRAM
        float weightInKg = product["targetWeight"].as<float>();
        data.products[index].targetWeight = weightInKg * 1000.0f;  // kg -> g
        
        index++;
    }
    
    Serial.println("\n=== THONG TIN PRODUCTION ===");
    Serial.print("Status: ");
    Serial.println(data.status);
    Serial.print("Production ID: ");
    Serial.println(data.productionId);
    Serial.print("So san pham: ");
    Serial.println(data.productCount);
    
    for (int i = 0; i < data.productCount; i++) {
        Serial.print("- ");
        Serial.print(data.products[i].productName);
        Serial.print(" (");
        Serial.print(data.products[i].targetWeight / 1000.0f, 1);  // Hiển thị kg
        Serial.print("kg = ");
        Serial.print(data.products[i].targetWeight, 0);  // Hiển thị gram
        Serial.println("g)");
    }
    
    return true;
}

SubmitResult APIClient::submitPackage(int productionId, int productId, float weight, int packageCount) {
    SubmitResult result;
    result.success = false;
    result.statusCode = 0;
    result.message = "";
    
    if (!isWiFiConnected()) {
        Serial.println("Loi: WiFi khong ket noi!");
        result.message = "WiFi khong ket noi";
        return result;
    }
    
    String url = baseUrl + String(API_POST_PACKAGE);
    
    Serial.println("\n=== GUI DU LIEU BAO ===");
    Serial.print("URL: ");
    Serial.println(url);
    
    // Tạo JSON payload
    JsonDocument doc;
    doc["deviceCode"] = deviceCode;
    doc["productionId"] = productionId;
    doc["productId"] = productId;
    doc["weight"] = weight / 1000.0f;  // g -> kg (DB lưu theo kg)
    doc["packageCount"] = packageCount;
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    Serial.print("Payload: ");
    Serial.println(jsonString);
    
    http.begin(url);
    http.setTimeout(API_TIMEOUT_MS);
    http.addHeader("Content-Type", "application/json");
    
    int httpCode = http.POST(jsonString);
    String response = http.getString();
    http.end();
    
    result.statusCode = httpCode;
    
    Serial.print("HTTP Code: ");
    Serial.println(httpCode);
    Serial.println("Response:");
    Serial.println(response);
    
    // Parse JSON response
    JsonDocument responseDoc;
    DeserializationError error = deserializeJson(responseDoc, response);
    
    if (!error) {
        result.message = responseDoc["error"]["message"] | responseDoc["message"] | "Unknown error";
    }
    
    // Xử lý theo status code
    if (httpCode == 201 || httpCode == 200) {
        result.success = true;
        result.message = "Success";
        Serial.println("GUI THANH CONG!");
    } else if (httpCode == 409) {
        result.message = "Ca san xuat da ket thuc";
        Serial.println("Loi 409: Production khong active");
    } else if (httpCode == 403) {
        result.message = "Thiet bi chua duoc gan";
        Serial.println("Loi 403: Device khong duoc gan vao production");
    } else if (httpCode == 400) {
        Serial.println("Loi 400: Cau hinh sai");
    } else {
        Serial.print("Loi HTTP: ");
        Serial.println(httpCode);
    }
    
    return result;
}

void APIClient::disconnect() {
    WiFi.disconnect();
    Serial.println("WiFi da ngat ket noi");
}
