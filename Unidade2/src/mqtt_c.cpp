#include <Arduino.h>
#include <PubSubClient.h>
#include "esp_check.h"
#include "config.h"
#include "secrets.h"
#include "wifi_h.h"
#include "mqtt_c.h"
#include "file_sys.h"

PubSubClient mqtt_client(wifi_client);

esp_err_t mqtt_init() {
    esp_err_t ret;
    
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        log("Falha ao obter a hora atual");
        log("Iniciando cliente MQTT");
    } else {
        char buffer[128];
        sprintf(buffer, "[%Y-%d-%d %H:%M:%S] ", &timeinfo);
        strcat(buffer, "Iniciando cliente MQTT");
        log(buffer);
    }

    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);
    ret = ESP_OK;
    return ret;
}

esp_err_t mqtt_connect() {
    if (mqtt_client.connected()) {
        Serial.println("Cliente MQTT já conectado.");
        return ESP_OK;
    }

    Serial.println(F("Conectando ao broker MQTT"));

    uint32_t start = millis();

    while (!mqtt_client.connected() && millis() - start < MQTT_TIMEOUT_MS) {
        Serial.print(F("MQTT Status: "));
        Serial.println(mqtt_client.state());

        if (mqtt_client.connect(CLIENT_ID, MQTT_USER, MQTT_KEY)) {
            Serial.println("MQTT conectado.");
            return ESP_OK;
        }
        Serial.println("aqui.");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
    // XXX PÂNICO
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        log("Falha ao obter a hora atual");
        log("Conexão com o broker MQTT excedeu o tempo limite.");
    } else {
        char buffer[128];
        sprintf(buffer, "[%Y-%d-%d %H:%M:%S] ", &timeinfo);
        strcat(buffer, "Conexão com o broker MQTT excedeu o tempo limite.");
        log(buffer);
    }
    return ESP_ERR_TIMEOUT;
}

esp_err_t mqtt_publish(const char *topic, const char *payload) {
    esp_err_t ret = ESP_OK;
    
    if (!mqtt_client.connected()) {
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            log("Falha ao obter a hora atual");
            log("Sem conexão com o broker MQTT no momento de publicação de dados");
        } else {
            char buffer[128];
            sprintf(buffer, "[%Y-%d-%d %H:%M:%S] ", &timeinfo);
            strcat(buffer, "Sem conexão com o broker MQTT no momento de publicação de dados");
            log(buffer);
        }
        ESP_RETURN_ON_ERROR(mqtt_connect(), "MQTT", "Falha ao se conectar ao broker mqtt para publicação");
    }

    ESP_GOTO_ON_FALSE(mqtt_client.publish(topic, payload), ESP_FAIL, err_pub, "MQTT", "Não foi possível publicar a mensagem");
    Serial.println((String("Publicação feita no tópico ") + String(topic)).c_str());
    mqtt_disconnect();
    return ESP_OK;

err_pub:
    // XXX PÂNICO
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        log("Falha ao obter a hora atual");
        log((String("Não foi possível publicar a mensagem no tópico ") + String(topic)).c_str());
    } else {
        char buffer[128];
        sprintf(buffer, "[%Y-%d-%d %H:%M:%S] ", &timeinfo);
        strcat(buffer, (String("Não foi possível publicar a mensagem no tópico ") + String(topic)).c_str());
        log(buffer);
    }
    mqtt_disconnect();
    return ESP_FAIL;
}

char *mqtt_get_topic(char *src, const char *subtopic) {
    strcat(strcat(src, CLIENT_ID), subtopic);
    return src;
}

void mqtt_disconnect() {
    mqtt_client.disconnect();
}

void mqtt_pool() {
    mqtt_client.loop();
}