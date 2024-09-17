#include "strings.h"
#include <string.h>

bool FindSubstring(char *string, char *substring)
{
    if (strstr(string, substring) != NULL) return true;
    return false;
}

char *GetSubstring(const char *string, char *beginning, char *end) {}
