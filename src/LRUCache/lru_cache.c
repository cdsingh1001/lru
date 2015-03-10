#include "lru_cache.h"
#include "lru_cache_util.h"

Datum_Container* LRUCache_HashTable[MAX_ENTRIES];
Datum_Container* first = NULL;
Datum_Container* last = NULL;

// Insert datum into the cache
bool LruCache_Insert(Datum d)
{
    Hash_Index index;
    Datum_Container* entry = NULL;
    Datum_Container* prev = NULL;

    if (!LruCache_GetIndexEntry(d.key, &index, &entry)) {
        return FALSE;
    }
    if (!entry) {
        entry = LruCache_NewContainer(d);
	    LRUCache_HashTable[index] = entry;
    }
    else {
        /* There is already an entry at head.
         * That means, either it's an update on the same key OR
         * it's an insert on the same bucket - chaining
         * */
        while (entry) {
            if (LruCache_IsEntryCorrect(d.key, entry)) {
                LruCache_UpdateDataValue(entry, d);
                LruCache_TouchEntry(entry);
                return TRUE;
            }
            prev = entry;
            entry = entry->next;
        }
        entry = LruCache_NewContainer(d);
        prev->next = entry;
    }

    LruCache_InsertAtEnd (entry);
    return TRUE;

}

// If key is present, then fill d with datum for key.
// Returns: true if found, false otherwise
bool LruCache_Lookup(int key, Datum *d)
{
    Hash_Index index;
    Datum_Container* entry = NULL;

    if (!LruCache_GetIndexEntry(key, &index, &entry)) {
        return FALSE;
    }

    while (entry) {
        if (LruCache_IsEntryCorrect(key, entry)) {
            LruCache_CopyDatum(d,entry);
            LruCache_TouchEntry(entry);
            return TRUE;
        }
        entry = entry->next;
    }

    return FALSE;
}

// Remove datum referenced by key, if any
bool LruCache_Remove(int key)
{
    Hash_Index index;
    Datum_Container* entry = NULL;
    Datum_Container* prev = NULL;

    if (!LruCache_GetIndexEntry(key, &index, &entry)) {
        return FALSE;
    }

    while (entry) {
        if (LruCache_IsEntryCorrect(key, entry)) {
            if (!prev) {
                LRUCache_HashTable[index] = entry->next;
            }
            else {
                prev->next = entry->next;
            }
            LruCache_RemoveContainerNode(entry);
            LruCache_DeleteContainer(entry);
            return TRUE;
        }
        prev = entry;
        entry = entry->next;
    }

    return FALSE;
}

// Fill d with oldest accessed datum and remove it.
// Returns: false if nothing to remove, true otherwise
bool LruCache_RemoveOldest(Datum *d)
{
    Datum_Container* entry = first;

    if (!LruCache_FindOldest(d)) {
        return FALSE;
    }

    LruCache_RemoveContainerNode(entry);
    if(!LruCache_RemoveContainer(entry)) {
        return FALSE;
    }

    return TRUE;
}

// Find oldest accessed datum, then fill d with datum.
// Returns: true if found oldest, false otherwise
bool LruCache_FindOldest(Datum *d)
{
    if (first) {
        LruCache_CopyDatum(d, first);
        return TRUE;
    }
    LruCache_FillKeyInvalid(d);
    return FALSE;
}

// Find most recently accessed datum, then fill d with datum.
// Returns: true if found most recent , false otherwise
bool LruCache_FindMostRecent(Datum *d)
{
    if (last) {
        LruCache_CopyDatum(d, last);
        return TRUE;
    }
    LruCache_FillKeyInvalid(d);
    return FALSE;
}

// Verify Integrity of the Cache
bool LruCache_VerifyIntegrity(void)
{
    int i = 0;
    int index = 0;
    int key;
    Datum_Container* entry = NULL;

    if (first) {
        if (first->node->prev) {
            return FALSE;
        }
    }

    if (last) {
        if (last->node->next) {
            return FALSE;
        }
    }

    for (i=0; i< MAX_ENTRIES; i++) {
        entry = LRUCache_HashTable[i];
        while (entry) {
            key = entry->data.key;
            if (!LruCache_CalcHashIndex(key, &index)) {
                continue;
            }
            if (index != i) {
                return FALSE;
            }
            if (entry->node->container != entry) {
                return FALSE;
            }
            entry = entry->next;
        }
    }
    return TRUE;
}

// Insert datum into the cache
void LruCache_InitCache(void)
{
    int i = 0;
   // Datum d;

    for (i=0; i< MAX_ENTRIES; i++) {
        LRUCache_HashTable[i] = NULL;
    }

    if (first || last) {
        assert(0);
    }
}

// Remove all entries from Cache
void LruCache_ClearCache(void)
{
    int i;
    Datum_Container* entry = NULL;

    if (first) {
        first = NULL;
    }
    if (last) {
        last = NULL;
    }

    for (i=0; i<MAX_ENTRIES; i++) {
        entry = LRUCache_HashTable[i];
        while (entry) {
            LRUCache_HashTable[i] = entry->next;
            LruCache_DeleteContainer(entry);
            entry = LRUCache_HashTable[i];
        }
        LRUCache_HashTable[i] = NULL;
    }
}

// Count the number of entries in Cache
int LruCache_CountCacheEntries(void)
{
    int i;
    int count = 0;
    Datum_Container* entry = NULL;

    for (i=0; i<MAX_ENTRIES; i++) {
        entry = LRUCache_HashTable[i];
        while (entry) {
            count++;
            entry = entry->next;
        }
    }
    return count;
}
