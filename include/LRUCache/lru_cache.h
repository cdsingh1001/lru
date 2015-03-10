#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include "datum.h"

#define MAX_ENTRIES 100
extern Datum_Container* first;
extern Datum_Container* last;

enum {
    TRUE = 1,
    FALSE = 0
};

// Insert datum into the cache
void LruCache_InitCache(void);

// Insert datum into the cache
bool LruCache_Insert(Datum d);

// If key is present, then fill d with datum for key.
// Returns: true if found, false otherwise
bool LruCache_Lookup(int key, Datum *d);

// Remove datum referenced by key, if any
bool LruCache_Remove(int key);

// Fill d with oldest accessed datum and remove it.
// Returns: false if nothing to remove, true otherwise
bool LruCache_RemoveOldest(Datum *d);

// Find oldest accessed datum, then fill d with datum.
// Returns: true if found oldest, false otherwise
bool LruCache_FindOldest(Datum *d);

// Find most recently accessed datum, then fill d with datum.
// Returns: true if found most recent , false otherwise
bool LruCache_FindMostRecent(Datum *d);

// Remove all entries from Cache
void LruCache_ClearCache(void);

// Count the number of entries in Cache
int LruCache_CountCacheEntries(void);

// Verify Integrity of the Cache
bool LruCache_VerifyIntegrity(void);


#endif // LRU_CACHE_H


