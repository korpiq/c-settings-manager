#include <stddef.h>

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
    size_t
        offset;
    double
        min,
        max;
    enum SettingType setting_type;
};

typedef void (*settingConsumerFunction)(const char * name, const char * value);

void serializeSettings(const struct Setting * settings_definition, const void * settings_struct, const settingConsumerFunction * consumer);
const char * settingFromStringByIndex(const struct Setting * settings_definition, void * settings_struct, size_t index, const char * value);
const char * settingToStringByIndex(const struct Setting * settings_definition, const void * settings_struct, size_t index, char * value);
const char * settingFromStringByName(const struct Setting * settings_definition, void * settings_struct, const char * name, const char * value);
const char * settingToStringByName(const struct Setting * settings_definition, const void * settings_struct, const char * name, char * value);

template <typename T>
class SettingConverter
{
public:
    T fromString(T original, const char * s, size_t max_size);
    const char * toString(const T * value, char * buf, size_t bufsiz);
};
