#include <stdio.h>
#include <string.h>
#include "settings_manager.h"

#define SETTING_BUFFER_LEN 120
char setting_buffer[SETTING_BUFFER_LEN + 1];

#define SETTING_STRINGIFIED(format, value) (snprintf(setting_buffer, SETTING_BUFFER_LEN, format, value) ? setting_buffer : NULL)

const char * settingToString(const struct Setting * setting_definition, const void * setting)
{
    switch (setting_definition->setting_type)
    {
        case SETTING_STRING:  return setting;
        case SETTING_BOOLEAN: return *(bool*) setting ? "true" : "false";
        case SETTING_LONG:    return SETTING_STRINGIFIED("%ld", *(long*) setting);
        case SETTING_ULONG:   return SETTING_STRINGIFIED("%lu", *(unsigned long*) setting);
        case SETTING_DOUBLE:  return SETTING_STRINGIFIED("%f", *(double*) setting);
    }

    return NULL;
}

void serializeSettings(const struct Setting * settings_definition, const void * settings_struct, settingConsumerFunction * consumer)
{
    while (settings_definition && settings_definition->name)
    {
        void * setting = (void*) ((char*)settings_struct + settings_definition->offset);
        const char * value = settingToString(settings_definition, setting);
        consumer(settings_definition->name, value);
        ++settings_definition;
    };
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

const char * setStringSettingFromString(const struct Setting * setting_definition, char * setting, const char * value)
{
    size_t max_len = setting_definition->string_limits.max;
    size_t value_len = strnlen(value, max_len);

    if (value_len >= max_len && value[max_len] != '\0')
    {
        snprintf(
            setting_buffer,
            SETTING_BUFFER_LEN,
            "Text value is too long (%zu) for setting \"%.60s\"",
            value_len,
            setting_definition->name
        );
        return setting_buffer;
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

const char * setBooleanSettingFromString(const struct Setting * setting_definition, bool * setting, const char * value)
{
    if (!strcmp("true", value))
    {
        *setting = true;
    }
    else if (!strcmp("false", value))
    {
        *setting = false;
    }
    else
    {
        snprintf(
            setting_buffer,
            SETTING_BUFFER_LEN,
            "Value for boolean setting \"%.60s\" must be \"true\" or \"false\"",
            setting_definition->name
        );
        return setting_buffer;
    }

    return NULL;
}

const char * setNumericSettingFromString(const struct Setting * setting_definition, const char * format, const char * type_name, void * setting, const char * value)
{
    if (sscanf(value, format, setting) == 1)
    {
        return NULL;
    }

    snprintf(
        setting_buffer,
        SETTING_BUFFER_LEN,
        "Value for setting \"%.60s\" is not a valid %.8s number",
        setting_definition->name,
        type_name
    );

    return setting_buffer;
}

const char * setSettingFromString(const struct Setting * setting_definition, void * setting, const char * value)
{
    switch(setting_definition->setting_type)
    {
        case SETTING_STRING:  return setStringSettingFromString(setting_definition, setting, value);
        case SETTING_LONG:    return setNumericSettingFromString(setting_definition, "%ld", "whole", setting, value);
        case SETTING_ULONG:   return setNumericSettingFromString(setting_definition, "%lu", "natural", setting, value);
        case SETTING_DOUBLE:  return setNumericSettingFromString(setting_definition, "%lf", "real", setting, value);
        case SETTING_BOOLEAN: return setBooleanSettingFromString(setting_definition, (bool*) setting, value);
    }

    snprintf(
        setting_buffer,
        SETTING_BUFFER_LEN,
        "Invalid type (%02d) for setting \"%.80s\"",
        setting_definition->setting_type,
        setting_definition->name
    );
    return setting_buffer;
}

const char * settingFromStringByName(const struct Setting * settings_definition, void * settings_struct, const char * name, const char * value)
{
    settings_definition = findSettingByName(settings_definition, name);
    if (settings_definition == NULL)
    {
        snprintf(
            setting_buffer,
            SETTING_BUFFER_LEN,
            "Setting not found by name: \"%.80s\"",
            name
        );
        return setting_buffer;
    }

    return setSettingFromString(settings_definition, ((char*)settings_struct + settings_definition->offset), value);
}

const char * settingToStringByName(const struct Setting * settings_definition, const void * settings_struct, const char * name, char * value)
{
    return NULL;
}
