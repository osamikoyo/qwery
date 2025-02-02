//
// Created by osami on 02.02.25.
//

#include "words.h"

#include <stdlib.h>

const char *common_words[] = {
    "жизнь",
    "время",
    "люди",
    "день",
    "работа",
    "глаз",
    "дом",
    "место",
    "слово",
    "дело",
    "смотреть",
    "говорить",
    "знать",
    "идти",
    "быть",
    "есть",
    "любить",
    "слышать",
    "думать",
    "стать",
    "жить",
    "хотеть",
    "писать",
    "ждать",
    "находить",
    "учиться",
    "играть",
    "сказать",
    "понимать",
    "ходить",
    "думать"
};

int count = sizeof(common_words) / sizeof(common_words[0]);

const char *get_random_word() {
    int random = rand()%(count-1);
    return common_words[random];
}


