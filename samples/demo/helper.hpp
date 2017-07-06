#pragma once

bool streq(const char *left, const char *right)
{
    if (!left  && !right)
        return true;
    if (left && right)
        return strcmp(left, right) == 0;
    return false;
}