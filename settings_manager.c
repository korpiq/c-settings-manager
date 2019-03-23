#include <stdio.h>
#include <string.h>
#include "settings_manager.h"

#define SETTING_ERROR_LEN 120
char setting_error[SETTING_ERROR_LEN + 1];

void serializeSettings(const struct Setting * settings_definition, const void * settings_struct, const settingConsumerFunction * consumer)
{

}

const struct Setting * findSettingByName(const struct Setting * settings_definition, const char * name)
{
    while (settings_definition && settings_definition->name)
    {
        if(!strncmp(name, settings_definition->name, SETTING_NAME_MAXLEN))
        {
            return settings_definition;
        };
        ++settings_definition;
    };

    return NULL;
}

const char * settingFromStringByIndex(const struct Setting * settings_definition, void * settings_struct, size_t index, const char * value)
{
    return NULL;
}

const char * settingToStringByIndex(const struct Setting * settings_definition, const void * settings_struct, size_t index, char * value)
{
    return NULL;
}

const char * setStringSettingFromString(const struct Setting * setting_definition, char * setting, const char * value)
{
    size_t max_len = setting_definition->string_limits.max;
    size_t value_len = strnlen(value, max_len);

    if (value_len >= max_len && value[max_len] != '\0')
    {
        snprintf(
            setting_error,
            SETTING_ERROR_LEN,
            "Text value is too long (%zu > %zu) for setting \"%.60s\"",
            value_len,
            setting_definition->string_limits.max,
            setting_definition->name
        );
        return setting_error;
    }

    if (setting_definition->string_limits.is_valid != NULL)
    {
        const char * result = setting_definition->string_limits.is_valid(value);
        if (result != NULL)
        {
            return result;
        }
    }

    strncpy(setting, value, value_len);
    setting[value_len] = '\0';

    return NULL;
}

const char * setSettingFromString(const struct Setting * setting_definition, void * setting, const char * value)
{
    switch(setting_definition->setting_type)
    {
        case SETTING_STRING: return setStringSettingFromString(setting_definition, setting, value);
    }

    snprintf(
        setting_error,
        SETTING_ERROR_LEN,
        "Invalid type (%02d) for setting \"%.80s\"",
        setting_definition->setting_type,
        setting_definition->name
    );
    return setting_error;
}

const char * settingFromStringByName(const struct Setting * settings_definition, void * settings_struct, const char * name, const char * value)
{
    settings_definition = findSettingByName(settings_definition, name);
    if (settings_definition == NULL)
    {
        snprintf(
            setting_error,
            SETTING_ERROR_LEN,
            "Setting not found by name: \"%.80s\"",
            name
        );
        return setting_error;
    }

    return setSettingFromString(settings_definition, (settings_struct + settings_definition->offset), value);
}

const char * settingToStringByName(const struct Setting * settings_definition, const void * settings_struct, const char * name, char * value)
{
    return NULL;
}
