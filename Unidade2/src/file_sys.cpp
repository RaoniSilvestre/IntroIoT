#include <file_sys.h>

#define LOG_FILE "/logs.txt"
#define HEAT_DATA_FILE "/heat_data.txt"
#define TEMPERATURE_DATA_FILE "/temperature_data.txt"
#define HUMIDITY_DATA_FILE "/humidity_data.txt"

void spiffs_init()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("SPIFFS Mount Failed");
        return;
    }
}

void read_file(const char *path)
{
    String logging = "Reading file" + String(path);
    log(logging.c_str());

    File file = SPIFFS.open(path);
    if (!file || file.isDirectory())
    {
        log("- failed to open file for reading");
        return;
    }

    log("- read from file:");
    while (file.available())
    {
        log(file.read());
    }
    file.close();
}

void write_file(const char *path, const char *message)
{
    String logging = "Writing file: " + String(path);
    log(logging.c_str());

    File file = SPIFFS.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("- failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.println("- file written");
    }
    else
    {
        Serial.println("- write failed");
    }
    file.close();
}

void append_file(const char *path, const char *message)
{

    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)) {
        Serial.println("[ERROR] Failed to get current time when logging " + String(message));
        return;
    }

    String bffr = "[" + String(timeinfo.tm_year) + "/" + String(timeinfo.tm_mon) + "/" + String(timeinfo.tm_mday);
    bffr += " " + String(timeinfo.tm_hour) + ":" + String(timeinfo.tm_min) + ":" + String(timeinfo.tm_sec) + "]";
    bffr += " " + String(message);

    File file = SPIFFS.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("[ERROR] Failed to open file for appending");
        return;
    }
    if (!file.print(message))
    {
        Serial.println("[ERROR] Append failed for " + String(bffr));
    } else {
        Serial.println("[LOG] " + bffr);
    }


    file.close();
}

void append_data(float value, int data_type)
{
    const char* formattedValue = String(value).c_str();

    switch (data_type)
    {
    case TEMPERATURE_TYPE:
        append_file(TEMPERATURE_DATA_FILE, formattedValue);
        break;
    case HUMIDITY_TYPE:
        append_file(HUMIDITY_DATA_FILE, formattedValue);
        break;
    case HEAT_TYPE:
        append_file(HEAT_DATA_FILE, formattedValue);
        break;
    default:
        break;
    }
}

void log(const char *message)
{
    append_file(LOG_FILE, message);
}
