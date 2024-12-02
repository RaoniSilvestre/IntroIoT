#include "esp_check.h"
#include "wifi_handle.h"

#define WIFI_TIMEOUT 30 * 1000

WiFiClient wifi_client;

void wifi_init() {
    uint8_t attempts = 0;

    Serial.println(F("Iniciando wifi"));
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(HOSTNAME);
    //WiFi.begin(SSID, PASS);

    while (wifi_connect != ESP_OK && attempts < 3) {
        Serial.println(F("Falha ao se conectar com a rede wifi"));
        // XXX PÂNICO
        attempts++;
        if (attempts == 3) {
            Serial.println(F("3 falhas consecutivas ao se conectar com a rede wifi. Reiniciando"));
            // XXX SUPER PÂNICO
            ESP.restart();
        } 
    }
}

esp_err_t wifi_connect() {
    Serial.println(F("Conectando à rede wifi"));

    esp_err_t ret = ESP_OK;

    uint32_t start = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - start < WIFI_TIMEOUT) {
        vTaskDelay(pdMS_TO_TICKS(100));
        Serial.print(F("Status: "));
        Serial.println(WiFi.status());
    }
    ESP_GOTO_ON_FALSE(WiFi.status() == WL_CONNECTED, ESP_ERR_TIMEOUT, err, "WIFI", "Tentativas de conexão wifi excederam o tempo limite");
    return ESP_OK;
    
err:
    // XXX PÂNICO

    return ret;
}