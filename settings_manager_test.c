#include <string.h>
#include "settings_manager.h"
#include "assert.h"
#include "stdio.h"

struct settings_test
{
    char string_value[6];
    long long_value;
    unsigned long ulong_value;
    double double_value;
    bool boolean_value;
};

struct Setting settings_test_metadata[] =
{
    {
        "String value",
        offsetof(struct settings_test, string_value),
        SETTING_STRING,
        .string_limits = { 5, NULL }
    },
    {
        "Long value",
        offsetof(struct settings_test, long_value),
        SETTING_ULONG,
        .long_limits = { -2, 2 }
    },
    {
        "Ulong value",
        offsetof(struct settings_test, ulong_value),
        SETTING_ULONG,
        .ulong_limits = { 0, 8 }
    },
    {
        "Double value",
        offsetof(struct settings_test, double_value),
        SETTING_DOUBLE,
        .double_limits = { -1.0, 1.0 }
    },
    {
        "Boolean value",
        offsetof(struct settings_test, boolean_value),
        SETTING_BOOLEAN
    },
    {NULL}
};

struct settings_test test_settings =
{
    "",
    -3,
    9,
    -1.1,
    false
};

void testSettingFromStringByNameFailsOnBadName()
{
    assert(0 != settingFromStringByName(settings_test_metadata, &test_settings, "Bad name", "oops"));
}

void testSettingFromStringByNameFailsOnTooLongString()
{
    assert(0 != settingFromStringByName(settings_test_metadata, &test_settings, "String value", "too long string"));
}

void testSettingFromStringByNameOk()
{
    const char * value = "strin";

    const char * result = settingFromStringByName(settings_test_metadata, &test_settings, "String value", value);

    if (result != NULL)
    {
        printf("Unexpected error: %s\n", result);
    }

    assert(result == NULL);
    assert(value != test_settings.string_value);
    assert(!strcmp(value, test_settings.string_value));
}

int main()
{
    testSettingFromStringByNameFailsOnBadName();
    testSettingFromStringByNameFailsOnTooLongString();
    testSettingFromStringByNameOk();
    return 0;
}
