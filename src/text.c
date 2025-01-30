//
// Created by osami on 29.01.25.
//
#include "text.h"

#include <string.h>
#include <strings.h>

double get_count(const char *text) {
    size_t length = strlen(text);
    return (double)length/5;
}