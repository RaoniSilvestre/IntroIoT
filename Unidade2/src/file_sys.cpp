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
    Serial.printf("Appending to file: %s\r\n", path);

    File file = SPIFFS.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("- failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        Serial.println("- message appended");
    }
    else
    {
        Serial.println("- append failed");
    }
    file.close();
}

void append_data(int value, int data_type)
{
    const char *formated_value = String(value).c_str();

    switch (data_type)
    {
    case TEMPERATURE_TYPE:
        append_file(TEMPERATURE_DATA_FILE, formated_value);
        break;
    case HUMIDITY_TYPE:
        append_file(HUMIDITY_DATA_FILE, formated_value);
        break;
    case HEAT_TYPE:
        append_file(HEAT_DATA_FILE, formated_value);
        break;
    default:
        break;
    }
}

void log(const char *message)
{
    Serial.println(message);
    append_file(LOG_FILE, message);
}
