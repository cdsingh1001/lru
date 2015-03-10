#ifndef LRU_CACHE_UTIL_H
#define LRU_CACHE_UTIL_H

#include "lru_cache.h"

typedef int Hash_Index;
extern Datum_Container* LRUCache_HashTable[MAX_ENTRIES];
extern Datum_Container* first;
extern Datum_Container* last;

#define LruCache_UpdateDataValue(entry, d) \
        entry->data.value = d.value

#define LruCache_CopyDatum(d, entry) \
        d->key = entry->data.key; \
        d->value = entry->data.value

#define LruCache_FillKeyInvalid(d) \
        d->key = -1

#define LruCache_UpdateNodePointers(node,n,p,c) \
        do { \
            node->next = n; \
            node->prev = p; \
            node->container = c; \
        } while(0)

#define LruCache_UpdateLast(last,entry) \
        do { \
            last->node->next = entry->node; \
            entry->node->prev = last->node; \
            entry->node->next = NULL; \
            last = entry; \
        } while(0)

#define LruCache_UpdateFirstLast(entry) \
        do { \
            first = last = entry; \
        } while(0)

static inline bool LruCache_IsEntryCorrect(int key, Datum_Container* entry)
{
    if (entry && (entry->data.key == key))
        return TRUE;
    else
        return FALSE;
}

bool LruCache_CalcHashIndex(int key, Hash_Index* Index);

Datum_Node* LruCache_NewDatumNode(Datum_Container* container);
void LruCache_DeleteDatumNode(Datum_Node *node);

Datum_Container* LruCache_NewContainer(Datum d);
void LruCache_DeleteContainer(Datum_Container* container);

bool LruCache_GetIndexEntry(int key, Hash_Index* index, Datum_Container** entry);
bool LruCache_IsEntryCorrect(int key, Datum_Container* entry);
void LruCache_TouchEntry(Datum_Container* entry);
void LruCache_InsertAtEnd(Datum_Container* entry);
void LruCache_RemoveContainerNode (Datum_Container* entry);
bool LruCache_RemoveContainer(Datum_Container* container);

#endif // LRU_CACHE_H


