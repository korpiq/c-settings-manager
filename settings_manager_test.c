#include <string.h>
#include "settings_manager.h"
#include "assert.h"

struct settings_test
{
    char * string_value;
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
        5,
        5,
        SETTING_STRING
    },
    {
        "Long value",
        offsetof(struct settings_test, long_value),
        -2,
        2,
        SETTING_ULONG
    },
    {
        "Ulong value",
        offsetof(struct settings_test, ulong_value),
        0,
        8,
        SETTING_ULONG
    },
    {
        "Double value",
        offsetof(struct settings_test, double_value),
        -1,
        1,
        SETTING_DOUBLE
    },
    {
        "Boolean value",
        offsetof(struct settings_test, boolean_value),
        0,
        1,
        SETTING_BOOLEAN
    },
    {NULL}
};

struct settings_test test_settings =
{
    (char*)NULL,
    -3,
    9,
    -1.1,
    false
};

void testSettingFromStringByNameFailsOnBadName()
{
    assert(0 != settingFromStringByName(settings_test_metadata, &test_settings, "Bad name", "oops"));
}

void testSettingFromStringByNameOk()
{
    const char * value = "a string";

    assert(0 == settingFromStringByName(settings_test_metadata, &test_settings, "String value", value));

    assert(value != test_settings.string_value);
    assert(!strcmp(value, test_settings.string_value));
}

int main()
{
    testSettingFromStringByNameFailsOnBadName();
    testSettingFromStringByNameOk();
    return 0;
}
