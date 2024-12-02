#include <Arduino.h>
#include <PubSubClient.h>
#include "esp_check.h"
#include "wifi_handle.h"
#include "mqtt.h"

#define MQTT_TIMEOUT 10 * 1000
#define CLIENT_ID "ESP32-Lab201-Logger"

PubSubClient mqtt_client(wifi_client);

esp_err_t mqtt_init() {
    esp_err_t ret;
    Serial.println(F("Iniciando cliente MQTT"));

    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);
    ESP_GOTO_ON_ERROR(mqtt_connect(), err, "MQTT", "Falha ao se conectar com o broker MQTT");

    return ESP_OK;
err:
    // XXX PÂNICO
    return ret;
}

esp_err_t mqtt_connect() {
    Serial.println(F("Conectando ao broker MQTT"));

    uint32_t start = millis();

    while (!mqtt_client.connected() && millis() - start < MQTT_TIMEOUT) {
        Serial.print(F("MQTT Status: "));
        Serial.println(mqtt_client.state());

        if (mqtt_client.connect(CLIENT_ID, MQTT_USER, MQTT_KEY)) {
            Serial.println("MQTT conectado!");
            return ESP_OK;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
    // XXX PÂNICO
    return ESP_ERR_TIMEOUT;
}

esp_err_t mqtt_publish(const char *topic, const char *payload) {
    esp_err_t ret = ESP_OK;
    
    
    if (!mqtt_client.connected()) {
        ESP_RETURN_ON_ERROR(mqtt_connect(), "MQTT", "Falha ao se conectar ao broker mqtt para publicação");
    }

    ESP_GOTO_ON_FALSE(mqtt_client.publish(topic, payload), ESP_FAIL, err_pub, "MQTT", "Não foi possível publicar a mensagem");

err_pub:
    return ESP_FAIL;
}

