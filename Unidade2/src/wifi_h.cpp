#include "esp_check.h"
#include "config.h"
#include "secrets.h"
#include "wifi_h.h"
#include "file_sys.h"

WiFiClient wifi_client;

void wifi_init() {
    uint8_t attempts = 0;

    log("Iniciando wifi");
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(HOSTNAME);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (wifi_connect() != ESP_OK && attempts < WIFI_CONNECT_ATTEMPTS) {
        log("Falha ao se conectar com a rede wifi");
        // XXX PÂNICO
        attempts++;
        if (attempts == WIFI_CONNECT_ATTEMPTS) {
            log("3 falhas consecutivas ao se conectar com a rede wifi. Reiniciando");
            // XXX SUPER PÂNICO
            ESP.restart();
        } 
    }
}

esp_err_t wifi_connect() {
    log("Conectando à rede wifi");

    esp_err_t ret = ESP_OK;

    uint32_t start = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - start < WIFI_TIMEOUT_MS) {
        vTaskDelay(pdMS_TO_TICKS(100));
        Serial.print(F("Status: "));
        Serial.println(WiFi.status());
    }
    ESP_GOTO_ON_FALSE(WiFi.status() == WL_CONNECTED, ESP_ERR_TIMEOUT, err, "WIFI", "Tentativas de conexão wifi excederam o tempo limite");
    return ESP_OK;
    
err:
    // XXX PÂNICO
    log("Tentativas de conexão wifi excederam o tempo limite.");
    return ret;
}