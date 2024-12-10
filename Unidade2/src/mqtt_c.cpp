#include "mqtt_c.h"
#include "config.h"
#include "esp_check.h"
#include "file_sys.h"
#include "secrets.h"
#include "wifi_h.h"
#include <Arduino.h>
#include <PubSubClient.h>

PubSubClient mqtt_client(wifi_client);

esp_err_t mqtt_init() {
  esp_err_t ret;
  log("Iniciando cliente MQTT");
  mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);
  ret = ESP_OK;
  log("MQTT iniciado");
  return ret;
}

esp_err_t mqtt_connect() {
  if (mqtt_client.connected()) {
    log("Cliente MQTT já conectado");
    return ESP_OK;
  }

  log("Conectando ao broker MQTT");

  uint32_t start = millis();
  while (!mqtt_client.connected() && millis() - start < MQTT_TIMEOUT_MS) {
    log("MQTT Status: " + String(mqtt_client.state()));

    if (mqtt_client.connect(CLIENT_ID, MQTT_USER, MQTT_KEY)) {
      log("MQTT conectado");
      return ESP_OK;
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }

  log("Conexão com o broker MQTT excedeu o tempo limite.");

  return ESP_ERR_TIMEOUT;
}

esp_err_t mqtt_publish(const char *topic, const char *payload) {
  esp_err_t ret = ESP_OK;

  if (!mqtt_client.connected()) {
    log("Sem conexão com o broker MQTT no momento de publicação de dados");
    ESP_RETURN_ON_ERROR(mqtt_connect(), "MQTT",
                        "Falha ao se conectar ao broker mqtt para publicação");
  }

  ESP_GOTO_ON_FALSE(mqtt_client.publish(topic, payload), ESP_FAIL, err_pub,
                    "MQTT", "Não foi possível publicar a mensagem");
  log(("Publicação feita no tópico" + String(topic)).c_str());
  mqtt_disconnect();
  return ESP_OK;

err_pub:
  // XXX PÂNICO
  log(("Não foi possível publicar mensagem no tópico" + String(topic))
            .c_str());
  mqtt_disconnect();
  return ESP_FAIL;
}

String mqtt_get_topic(const char *subtopic) {
  return String(CLIENT_ID) + String(subtopic);
}

void mqtt_disconnect() { mqtt_client.disconnect(); }

void mqtt_pool() { mqtt_client.loop(); }