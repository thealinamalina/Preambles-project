#include "simulation.h"
#include <stdio.h>
#include <time.h>
#include <stddef.h>
#include <stdlib.h>

#define START_CAPACITY 16

//возвращаемые значения: -1 - ошибка, 0 - успех, 1 - коллизия

int list_init(List* l) {
    l->calls = malloc(START_CAPACITY * sizeof(Call)); 
    if (!l->calls) {
        printf("Error: Memory allocation failed.\n");
        return -1;
    }
    l->size = 0;
    l->capacity = START_CAPACITY;
    return 0;
}

int list_add(List* l, Call_type type, int abonent_id, int preamble_number) {
    if (l->size + 1 > l->capacity) {
        size_t new_capacity = l->capacity * 2;
        Call* new_calls = realloc(l->calls, sizeof(Call) * new_capacity);
        if (!new_calls) {
            printf("Error: Memory allocation failed.\n");
            return -1;
        }
        l->calls = new_calls;
        l->capacity = new_capacity;
    }
    Call call;
    call.type = type;
    call.abonent_id = abonent_id;
    call.preamble_number = preamble_number;
    l->calls[l->size++] = call;
    return 0;
}

void list_free(List* l) {
    free(l->calls);
    l->calls = NULL;
    l->size = l->capacity = 0;
}

int attempt(int abonent_count, List* out_list) {
    if (out_list == NULL) {
        printf("Error: out_list is NULL.\n");
        return -1;
    }
    if (abonent_count <= 0 || abonent_count > MAX_PREAMBLES) {
        printf("Error: Incorrect number of abonents. Connection impossible.\n");
        return -1;
    }

    static int random_seed = 0;
    if (random_seed == 0) {
        srand(time(NULL));
        random_seed = 1;
    }

    int preambles[MAX_PREAMBLES];
    int count_usage[MAX_PREAMBLES] = {0};

    for (int i = 0; i < abonent_count; i++) {
        int preamble = rand() % MAX_PREAMBLES;
        preambles[i] = preamble;
        count_usage[preamble]++;
        if (list_add(out_list, ABONENT_SEND_PREAMBLE, i, preamble) != 0) {
            return -1;
        }
    }

    // Добавляем проверки БС
    for (int i = 0; i < MAX_PREAMBLES; i++) {
        if (count_usage[i] == 0) {
            continue;
        }
        if (list_add(out_list, BS_CHECK, -1, i) != 0) {
            return -1;
        }
    }
    // Добавляем ответы абонентов
    for (int i = 0; i < MAX_PREAMBLES; i++) {
        if (count_usage[i] == 0) {
            continue;
        }
        for (int j =0; j < abonent_count; j++) {
            if (preambles[j] == i) {
                if (list_add(out_list, ABONENT_ANSWER, j, i) != 0) {
                    return -1;
                }
            }
        }
    }

    int collision = 0;
    for (int i = 0; i < MAX_PREAMBLES; i++) {
        if (count_usage[i] > 1) {
            collision = 1;
            break;
        }
    }

    if (collision != 0) {
        if (list_add(out_list, BS_COLLISION, -1, -1) != 0) {
            return -1;
        }
        if (list_add(out_list, ABONENT_RETRY, -1, -1) != 0) {
            return -1;
        }
        return 1;
    } else {
        for (int i = 0; i < abonent_count; i++) {
            if (list_add(out_list, BS_SUCCESS, i, preambles[i]) != 0) {
                return -1;
            }
        }
        return 0;
    }
}
