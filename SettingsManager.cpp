#include <string.h>
#include "SettingsManager.h"

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

    return 0;
}

const char * settingFromStringByIndex(const struct Setting * settings_definition, void * settings_struct, size_t index, const char * value)
{
    return 0;
}

const char * settingToStringByIndex(const struct Setting * settings_definition, const void * settings_struct, size_t index, char * value)
{
    return 0;
}

const char * settingFromStringByName(const struct Setting * settings_definition, void * settings_struct, const char * name, const char * value)
{
    settings_definition = findSettingByName(settings_definition, name);
    if (settings_definition)
    {
        const char ** it = ((const char **)settings_struct + settings_definition->offset);
        *it = value;
        return 0;
    }

    return "Setting not found by name";
}

const char * settingToStringByName(const struct Setting * settings_definition, const void * settings_struct, const char * name, char * value)
{
    return 0;
}

