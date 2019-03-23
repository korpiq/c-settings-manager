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
        SETTING_LONG,
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
    "str",
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

void testSettingFromStringByNameStringOk()
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

void testSettingFromStringByNameLongOk()
{
    const char * value = "2";

    const char * result = settingFromStringByName(settings_test_metadata, &test_settings, "Long value", value);

    if (result != NULL)
    {
        printf("Unexpected error: %s\n", result);
    }

    assert(result == NULL);
    assert(test_settings.long_value == 2);
}

void testSettingFromStringByNameULongOk()
{
    const char * value = "6";

    const char * result = settingFromStringByName(settings_test_metadata, &test_settings, "Ulong value", value);

    if (result != NULL)
    {
        printf("Unexpected error: %s\n", result);
    }

    assert(result == NULL);
    assert(test_settings.ulong_value == 6);
}

void testSettingFromStringByNameDoubleOk()
{
    const char * value = "-0.5";

    const char * result = settingFromStringByName(settings_test_metadata, &test_settings, "Double value", value);

    if (result != NULL)
    {
        printf("Unexpected error: %s\n", result);
    }

    assert(result == NULL);
    assert(test_settings.double_value == -0.5);
}

void testSettingFromStringByNameBooleanOk()
{
    const char * value = "true";

    const char * result = settingFromStringByName(settings_test_metadata, &test_settings, "Boolean value", value);

    if (result != NULL)
    {
        printf("Unexpected error: %s\n", result);
    }

    assert(result == NULL);
    assert(test_settings.boolean_value == true);
}

int result_at = 0;
char result[200];

void serializeToResult(const char * name, const char * value)
{
    int len;

    if (result_at > 0)
    {
        result[result_at++] = ',';
    }

    len = strnlen(name, 20);
    strncpy(result + result_at, name, len);
    result[result_at += len] = ':';
    ++result_at;

    len = strnlen(value, 20);
    strncpy(result + result_at, value, len);
    result_at += len;
    result[result_at] = '\0';
}

void testSerializingOk()
{
    const char * expected = "String value:str,Long value:-3,Ulong value:9,Double value:-1.100000,Boolean value:false";
    serializeSettings(settings_test_metadata, &test_settings, serializeToResult);
    assert(!strncmp(expected, result, 200));
}

int main()
{
    testSerializingOk();
    testSettingFromStringByNameFailsOnBadName();
    testSettingFromStringByNameFailsOnTooLongString();
    testSettingFromStringByNameStringOk();
    testSettingFromStringByNameLongOk();
    testSettingFromStringByNameULongOk();
    testSettingFromStringByNameDoubleOk();
    testSettingFromStringByNameBooleanOk();

    return 0;
}
