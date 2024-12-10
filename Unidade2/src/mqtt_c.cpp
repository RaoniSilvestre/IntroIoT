#include <Arduino.h>
#include <PubSubClient.h>
#include "esp_check.h"
#include "config.h"
#include "secrets.h"
#include "wifi_h.h"
#include "mqtt_c.h"
#include "file_sys.h"

PubSubClient mqtt_client(wifi_client);

esp_err_t mqtt_init()
{
    esp_err_t ret;

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        log("Falha ao obter a hora atual");
        log("Iniciando cliente MQTT");
    }
    else
    {
        String bffr = "[" + String(timeinfo.tm_year) + "/" + String(timeinfo.tm_mon) + "/" + String(timeinfo.tm_mday);
        bffr += " " + String(timeinfo.tm_hour) + ":" + String(timeinfo.tm_min) + ":" + String(timeinfo.tm_sec);
        bffr += " Iniciando cliente MQTT";
        log(bffr.c_str());
    }

    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);
    ret = ESP_OK;
    return ret;
}

esp_err_t mqtt_connect()
{
    if (mqtt_client.connected())
    {
        log("Cliente MQTT já conectado");
        return ESP_OK;
    }

    log("Conectando ao broker MQTT");

    uint32_t start = millis();

    while (!mqtt_client.connected() && millis() - start < MQTT_TIMEOUT_MS)
    {
        String mqttstatus = "MQTT Status: " + String(mqtt_client.state());
        log(mqttstatus.c_str());

        if (mqtt_client.connect(CLIENT_ID, MQTT_USER, MQTT_KEY))
        {
            log("MQTT conectado");
            return ESP_OK;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    // XXX PÂNICO
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        log("Falha ao obter a hora atual");
        log("Conexão com o broker MQTT excedeu o tempo limite.");
    }
    else
    {
        String bffr = "[" + String(timeinfo.tm_year) + "/" + String(timeinfo.tm_mon) + "/" + String(timeinfo.tm_mday);
        bffr += " " + String(timeinfo.tm_hour) + ":" + String(timeinfo.tm_min) + ":" + String(timeinfo.tm_sec) + "]";
        bffr += " Conexão com o broker MQTT excedeu o tempo limite.";
        log(bffr.c_str());
    }
    return ESP_ERR_TIMEOUT;
}

esp_err_t mqtt_publish(const char *topic, const char *payload)
{
    esp_err_t ret = ESP_OK;

    if (!mqtt_client.connected())
    {
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
            log("Falha ao obter a hora atual");
            log("Sem conexão com o broker MQTT no momento de publicação de dados");
        }
        else
        {
            String bffr = "[" + String(timeinfo.tm_year) + "/" + String(timeinfo.tm_mon) + "/" + String(timeinfo.tm_mday);
            bffr += " " + String(timeinfo.tm_hour) + ":" + String(timeinfo.tm_min) + ":" + String(timeinfo.tm_sec) + "]";
            bffr += " Sem conexão com o broker MQTT no momento de publicação de dados";

            log(bffr.c_str());
        }
        ESP_RETURN_ON_ERROR(mqtt_connect(), "MQTT", "Falha ao se conectar ao broker mqtt para publicação");
    }

    ESP_GOTO_ON_FALSE(mqtt_client.publish(topic, payload), ESP_FAIL, err_pub, "MQTT", "Não foi possível publicar a mensagem");
    log(("Publicação feita no tópico" + String(topic)).c_str());
    mqtt_disconnect();
    return ESP_OK;

err_pub:
    // XXX PÂNICO
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        log("Falha ao obter a hora atual");
        log((String("Não foi possível publicar a mensagem no tópico ") + String(topic)).c_str());
    }
    else
    {
        String bffr = "[" + String(timeinfo.tm_year) + "/" + String(timeinfo.tm_mon) + "/" + String(timeinfo.tm_mday);
        bffr += " " + String(timeinfo.tm_hour) + ":" + String(timeinfo.tm_min) + ":" + String(timeinfo.tm_sec) + "]";
        bffr += " Não foi possível publicar mensagem no tópico" + String(topic);
        log(bffr.c_str());
    }
    mqtt_disconnect();
    return ESP_FAIL;
}

String mqtt_get_topic(const char *subtopic)
{
    return String(CLIENT_ID) + String(subtopic);
}

void mqtt_disconnect()
{
    mqtt_client.disconnect();
}

void mqtt_pool()
{
    mqtt_client.loop();
}