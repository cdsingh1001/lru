#include "lru_cache_util.h"

bool LruCache_CalcHashIndex(int key, Hash_Index* index)
{
    if ((key < 0) || !index) {
        return FALSE;
    }
    *index = key % MAX_ENTRIES;
    return TRUE;
} 

bool LruCache_GetIndexEntry(int key, Hash_Index* index, Datum_Container** entry)
{
    if (!LruCache_CalcHashIndex(key, index)) {
        return FALSE;
    }
    *entry = LRUCache_HashTable[*index];

    return TRUE;
}

Datum_Node* LruCache_NewDatumNode(Datum_Container* container)
{
    Datum_Node* node;
    node = (Datum_Node*)malloc(sizeof(Datum_Node));
    if (!node) {
        printf("Memory allocation failure\n");
	return NULL;
    }

    LruCache_UpdateNodePointers(node, NULL, NULL, container);
    return node;
}

void LruCache_DeleteDatumNode(Datum_Node *node)
{
    if (node) {
        LruCache_UpdateNodePointers(node, NULL, NULL, NULL);
        free(node);
    }
}

void LruCache_RemoveContainerNode (Datum_Container* entry)
{
    Datum_Node* prev = NULL;
    Datum_Node* next = NULL;

    prev = entry->node->prev;
    next = entry->node->next;

    if (prev) {
        prev->next = next;
    }

    if (next) {
        if (first == entry) {
            first = next->container;
        }
        next->prev = prev;
    }

    if (last) {
        if ((first == entry) && (first == last)) {
            LruCache_UpdateFirstLast(NULL);
            return;
        }
        if (last == entry) {
            last = last->node->prev->container;
        }
    }
}

Datum_Container* LruCache_NewContainer(Datum d)
{
    Datum_Container* container = NULL;

    container = (Datum_Container*)malloc(sizeof(Datum_Container));
    if (!container) {
        printf("Memory allocation failure\n");
        return NULL;
    }

    container->data.key   = d.key;
    container->data.value = d.value;
    container->node       = LruCache_NewDatumNode(container);
    container->next       = NULL;
    return container;
}

void LruCache_DeleteContainer(Datum_Container* container)
{
    if (container) {
        LruCache_DeleteDatumNode(container->node);
        container->node = NULL;
        container->next = NULL;
        free(container);
    }
}

bool LruCache_RemoveContainer(Datum_Container* container)
{
    int key;
    Hash_Index index;
    Datum_Container* entry = NULL;
    Datum_Container* prev = NULL;

    key = container->data.key;

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
            LruCache_DeleteContainer(entry);
            return TRUE;
        }
        prev = entry;
        entry = entry->next;
    }

    return FALSE;
}

void LruCache_TouchEntry (Datum_Container* entry)
{
    Datum_Node* prev = NULL;
    Datum_Node* next = NULL;


    prev = entry->node->prev;
    next = entry->node->next;


    if (prev) {
        if (last != entry) {
            prev->next = next;
        }
    }

    if (next) {
        if (first == entry) {
            first = next->container;
        }
        next->prev = prev;
    }

    if (last) {
        if (last != entry) {
            LruCache_UpdateLast(last, entry);
        }
    }
    else {
        LruCache_UpdateFirstLast(entry);
    }
}

void LruCache_InsertAtEnd (Datum_Container* entry)
{
    if (last) {
        LruCache_UpdateLast(last, entry);
    }
    else {
        LruCache_UpdateFirstLast(entry);
    }
}
