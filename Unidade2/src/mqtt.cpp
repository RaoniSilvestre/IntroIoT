#include <Arduino.h>
#include <PubSubClient.h>
#include "esp_check.h"
#include "config.h"
#include "secrets.h"
#include "wifi_handle.h"
#include "mqtt.h"
#include "file_sys.h"

PubSubClient mqtt_client(wifi_client);

esp_err_t mqtt_init() {
    esp_err_t ret;
    log("Iniciando cliente MQTT");

    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);
    ESP_GOTO_ON_ERROR(mqtt_connect(), err, "MQTT", "Falha ao se conectar com o broker MQTT");

    return ESP_OK;
err:
    // XXX PÂNICO
    log("Falha ao se conectar com o broker MQTT.");
    return ret;
}

esp_err_t mqtt_connect() {
    Serial.println(F("Conectando ao broker MQTT"));

    uint32_t start = millis();

    while (!mqtt_client.connected() && millis() - start < MQTT_TIMEOUT_MS) {
        Serial.print(F("MQTT Status: "));
        Serial.println(mqtt_client.state());

        if (mqtt_client.connect(CLIENT_ID, MQTT_USER, MQTT_KEY)) {
            log("MQTT conectado.");
            return ESP_OK;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    
    // XXX PÂNICO
    log("Conexão com o broker MQTT excedeu o tempo limite.");
    return ESP_ERR_TIMEOUT;
}

esp_err_t mqtt_publish(const char *topic, const char *payload) {
    esp_err_t ret = ESP_OK;
    
    if (!mqtt_client.connected()) {
        log("Sem conexão com o broker MQTT no momento de publicação de dados");
        ESP_RETURN_ON_ERROR(mqtt_connect(), "MQTT", "Falha ao se conectar ao broker mqtt para publicação");
    }

    ESP_GOTO_ON_FALSE(mqtt_client.publish(topic, payload), ESP_FAIL, err_pub, "MQTT", "Não foi possível publicar a mensagem");
    log((String("Publicação feita no tópico ") + String(topic)).c_str());
    return ESP_OK;

err_pub:
    // XXX PÂNICO
    log((String("Não foi possível publicar a mensagem no tópico ") + String(topic)).c_str());
    return ESP_FAIL;
}

char *mqtt_get_topic(const char *subtopic) {
    char *topic = (char *)malloc(strlen(CLIENT_ID) + strlen(subtopic) + 1);
    strcat(strcat(topic, CLIENT_ID), subtopic);
    return topic;
}

void mqtt_pool() {
    mqtt_client.loop();
}