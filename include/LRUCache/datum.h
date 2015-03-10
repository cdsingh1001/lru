#ifndef DATUM_H
#define DATUM_H

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
//#include "common.h"

struct Datum_Container_s;

typedef struct Datum_Node_ {
    struct Datum_Node_* next;
    struct Datum_Node_* prev;
    struct Datum_Container_s* container;
} Datum_Node;

typedef struct Datum_s {
    int key;
    int value;
} Datum;

typedef struct Datum_Container_s {
    Datum data;
    Datum_Node* node;
    struct Datum_Container_s* next;
} Datum_Container;

#define DATUM_FORMAT "(%d, %d)"

static inline void Datum_Print (Datum d)
{
    printf(DATUM_FORMAT, d.key, d.value);
}

static inline bool Datum_FromString (Datum *d, const char *string)
{
    return 2 == sscanf(string, DATUM_FORMAT, &d->key, &d->value);
}

static inline bool Datum_Read (Datum *d, FILE *file)
{
    return 2 == fscanf(file, DATUM_FORMAT, &d->key, &d->value);
}

#endif // DATUM_H

