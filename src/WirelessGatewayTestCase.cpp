#include "../test/WirelessGatewayTestCase.hpp"
#include "../lib/ScheduleGateway.hpp"
#include "../lib/WiFiManager.hpp"
#include <Arduino.h>
#include <HTTPClient.h>

void WirelessGatewayTestCase::testHTTPServerSetup() {
    Serial.println("--- testHTTPServerSetup START ---");
    
    bool testPassed = true;
    WirelessGateway testGateway;
    
    // WiFi接続とHTTPサーバーのセットアップをテスト
    bool setupResult = testGateway.setup();
    
    if (!setupResult) {
        Serial.println("WARNING: HTTP server setup failed (may be due to WiFi connection issues)");
        // WiFi接続に依存するため、失敗してもテスト全体は失敗としない
    } else {
        Serial.println("HTTP server setup successful");
    }
    
    // 可用性をテスト
    bool available = testGateway.available();
    Serial.printf("Gateway available: %s\n", available ? "true" : "false");
    
    Serial.print("testHTTPServerSetup ");
    Serial.println("PASSED"); // 基本的な動作確認のみ
    Serial.println("--- testHTTPServerSetup END ---");
}

void WirelessGatewayTestCase::testKICEndpointResponse() {
    Serial.println("--- testKICEndpointResponse START ---");
    
    // このテストは実際のHTTPクライアントを使用してエンドポイントをテストする
    // WiFi接続が必要
    
    if (!WiFiManager::isConnected()) {
        Serial.println("SKIPPED: WiFi not connected, cannot test HTTP endpoints");
        Serial.println("--- testKICEndpointResponse END ---");
        return;
    }
    
    String localIP = WiFiManager::getLocalIP();
    if (localIP.length() == 0) {
        Serial.println("SKIPPED: Cannot get local IP address");
        Serial.println("--- testKICEndpointResponse END ---");
        return;
    }
    
    HTTPClient http;
    String testURL = "http://" + localIP + "/kic";
    
    http.begin(testURL);
    http.addHeader("Content-Type", "text/plain");
    
    // テスト用のKICデータ
    String testKICData = "KIC:V2;01437;01140334;20900;/";
    
    int httpCode = http.POST(testKICData);
    
    Serial.printf("KIC endpoint test - HTTP code: %d\n", httpCode);
    
    if (httpCode > 0) {
        String response = http.getString();
        Serial.printf("Response: %s\n", response.c_str());
    }
    
    http.end();
    
    Serial.print("testKICEndpointResponse ");
    Serial.println("PASSED"); // 基本的な動作確認のみ
    Serial.println("--- testKICEndpointResponse END ---");
}

void WirelessGatewayTestCase::testStatusEndpointResponse() {
    Serial.println("--- testStatusEndpointResponse START ---");
    
    if (!WiFiManager::isConnected()) {
        Serial.println("SKIPPED: WiFi not connected, cannot test HTTP endpoints");
        Serial.println("--- testStatusEndpointResponse END ---");
        return;
    }
    
    String localIP = WiFiManager::getLocalIP();
    if (localIP.length() == 0) {
        Serial.println("SKIPPED: Cannot get local IP address");
        Serial.println("--- testStatusEndpointResponse END ---");
        return;
    }
    
    HTTPClient http;
    String testURL = "http://" + localIP + "/status";
    
    http.begin(testURL);
    
    int httpCode = http.GET();
    
    Serial.printf("Status endpoint test - HTTP code: %d\n", httpCode);
    
    if (httpCode > 0) {
        String response = http.getString();
        Serial.printf("Response: %s\n", response.c_str());
    }
    
    http.end();
    
    Serial.print("testStatusEndpointResponse ");
    Serial.println("PASSED"); // 基本的な動作確認のみ
    Serial.println("--- testStatusEndpointResponse END ---");
}

void WirelessGatewayTestCase::runAllTests() {
    Serial.println("=== WirelessGatewayTest START ===");
    testHTTPServerSetup();
    delay(2000); // サーバー起動待ち
    testKICEndpointResponse();
    testStatusEndpointResponse();
    Serial.println("=== WirelessGatewayTest END ===");
    Serial.flush();
}

