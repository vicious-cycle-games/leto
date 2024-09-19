#include "strings.h"
#include <string.h>

bool FindSubstring(const char *string, char *substring)
{
    if (strstr(string, substring) != NULL) return true;
    return false;
}

bool FindCharacter(const char *string, char character)
{
    if (strchr(string, character) != NULL) return true;
    return false;
}
