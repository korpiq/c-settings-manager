#include <stddef.h>
#include <stdbool.h>

#define SETTING_NAME_MAXLEN 20
#define SETTING_VALUE_MAXLEN 200

enum SettingType
{
    SETTING_STRING,
    SETTING_LONG,
    SETTING_ULONG,
    SETTING_DOUBLE,
    SETTING_BOOLEAN
};

struct Setting
{
    const char * name;
    size_t offset;
    enum SettingType setting_type;
    union {
        struct setting_string_limits { size_t max; const char * (*is_valid)(const char *); } string_limits;
        struct setting_long_limits { long min, max; } long_limits;
        struct setting_ulong_limits { unsigned long min, max; } ulong_limits;
        struct setting_double_limits { double min, max; } double_limits;
    };
};

typedef void (*settingConsumerFunction)(const char * name, const char * value);

void serializeSettings(const struct Setting * settings_definition, const void * settings_struct, const settingConsumerFunction * consumer);
const char * settingFromStringByName(const struct Setting * settings_definition, void * settings_struct, const char * name, const char * value);
const char * settingToStringByName(const struct Setting * settings_definition, const void * settings_struct, const char * name, char * value);
