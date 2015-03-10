#include "CppUTest/TestHarness.h"

extern "C"
{
#include "lru_cache.h"
}

TEST_GROUP(LRUCache)
{
    Datum d;
    Datum result;
 
    void setup()
    {
        LruCache_InitCache();
    }

    void teardown()
    {
       LruCache_ClearCache();
    }
};

TEST(LRUCache, LookupInEmptyCache)
{
    d.key = 5;
    CHECK_TRUE(LruCache_VerifyIntegrity());
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));
}

TEST(LRUCache, InsertNegativeKey)
{
    d.key = -5;
    d.value = -50;
    CHECK_FALSE(LruCache_Insert(d));
    LONGS_EQUAL(0, LruCache_CountCacheEntries());
}

TEST(LRUCache, InsertOnePairLookup)
{
    d.key = 5;
    d.value = 50;

    CHECK_TRUE(LruCache_Insert(d));
    CHECK_TRUE(LruCache_VerifyIntegrity());
    LONGS_EQUAL(TRUE, LruCache_Lookup(d.key,&result));
    LONGS_EQUAL(d.key, result.key);
    LONGS_EQUAL(d.value, result.value);

    LONGS_EQUAL(1, LruCache_CountCacheEntries());

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(5, result.key);
    LONGS_EQUAL(50, result.value);
}

TEST(LRUCache, InsertUpdateLookup)
{
    d.key = 5;
    d.value = 50;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 105;
    d.value = 1050;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 7;
    d.value = 70;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 107;
    d.value = 1070;
    CHECK_TRUE(LruCache_Insert(d));

    CHECK_TRUE(LruCache_VerifyIntegrity());

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(5, result.key);
    LONGS_EQUAL(50, result.value);

    d.key = 5;
    d.value = 150;
    CHECK_TRUE(LruCache_Insert(d));

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(105, result.key);
    LONGS_EQUAL(1050, result.value);

    d.key = 105;
    d.value = 15;
    CHECK_TRUE(LruCache_Insert(d));

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(7, result.key);
    LONGS_EQUAL(70, result.value);

    d.key = 7;
    d.value = 70;
    LONGS_EQUAL(TRUE, LruCache_Lookup(d.key,&result));
    LONGS_EQUAL(d.key, result.key);
    LONGS_EQUAL(d.value, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(107, result.key);
    LONGS_EQUAL(1070, result.value);

    LONGS_EQUAL(4, LruCache_CountCacheEntries());

}

TEST(LRUCache, InsertOnePairRemoveLookup)
{
    d.key = 7;
    d.value = 70;

    CHECK_TRUE(LruCache_Insert(d));
    CHECK_TRUE(LruCache_VerifyIntegrity());
    CHECK_TRUE(LruCache_Remove(d.key));
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));
    LONGS_EQUAL(0, LruCache_CountCacheEntries());
}


TEST(LRUCache, InsertOneRemoveLookupLoop)
{
    int i =0;

    for (i=0; i<10; i++) {
        d.key = 7;
        d.value = 70;

        CHECK_TRUE(LruCache_Insert(d));
        LONGS_EQUAL(TRUE, LruCache_Lookup(d.key,&result));
        CHECK_TRUE(LruCache_Remove(d.key));
        LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));
        CHECK_TRUE(LruCache_VerifyIntegrity());
    }
    LONGS_EQUAL(0, LruCache_CountCacheEntries());
}

TEST(LRUCache, InsertMultipleWithDifferentKeysLookup)
{
    d.key = 5;
    d.value = 50;
    CHECK_TRUE(LruCache_Insert(d));
    d.key = 7;
    d.value = 70;
    CHECK_TRUE(LruCache_Insert(d));
    d.key = 8;
    d.value = 80;
    CHECK_TRUE(LruCache_Insert(d));
    d.key = 9;
    d.value = 90;
    CHECK_TRUE(LruCache_Insert(d));

    CHECK_TRUE(LruCache_VerifyIntegrity());

    LONGS_EQUAL(TRUE, LruCache_Lookup(5,&result));
    LONGS_EQUAL(5, result.key);
    LONGS_EQUAL(50, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(7, result.key);
    LONGS_EQUAL(70, result.value);

    LONGS_EQUAL(TRUE, LruCache_Lookup(8,&result));
    LONGS_EQUAL(8, result.key);
    LONGS_EQUAL(80, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(7, result.key);
    LONGS_EQUAL(70, result.value);

    LONGS_EQUAL(TRUE, LruCache_Lookup(7,&result));
    LONGS_EQUAL(7, result.key);
    LONGS_EQUAL(70, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(9, result.key);
    LONGS_EQUAL(90, result.value);

    LONGS_EQUAL(TRUE, LruCache_Lookup(7,&result));
    LONGS_EQUAL(7, result.key);
    LONGS_EQUAL(70, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(9, result.key);
    LONGS_EQUAL(90, result.value);

    LONGS_EQUAL(4, LruCache_CountCacheEntries());
}

TEST(LRUCache, InsertMultipleRemoveOneLookup)
{
    d.key = 5;
    d.value = 50;
    CHECK_TRUE(LruCache_Insert(d));
    d.key = 7;
    d.value = 70;
    CHECK_TRUE(LruCache_Insert(d));
    d.key = 8;
    d.value = 80;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 7;
    d.value = 70;

    CHECK_TRUE(LruCache_Remove(d.key));
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    LONGS_EQUAL(TRUE, LruCache_Lookup(5,&result));
    LONGS_EQUAL(5, result.key);
    LONGS_EQUAL(50, result.value);

    LONGS_EQUAL(TRUE, LruCache_Lookup(8,&result));
    LONGS_EQUAL(8, result.key);
    LONGS_EQUAL(80, result.value);

    LONGS_EQUAL(2, LruCache_CountCacheEntries());
}

TEST(LRUCache, InsertMultipleWithSameKeyLookupRemove)
{
    d.key = 5;
    d.value = 50;
    CHECK_TRUE(LruCache_Insert(d));

    LONGS_EQUAL(TRUE, LruCache_Lookup(5,&result));
    LONGS_EQUAL(5, result.key);
    LONGS_EQUAL(50, result.value);

    d.key = 7;
    d.value = 70;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 5;
    d.value = 150;
    CHECK_TRUE(LruCache_Insert(d));

    LONGS_EQUAL(TRUE, LruCache_Lookup(5,&result));
    LONGS_EQUAL(5, result.key);
    LONGS_EQUAL(150, result.value);

    d.key = 7;
    d.value = 170;
    CHECK_TRUE(LruCache_Insert(d));

    CHECK_TRUE(LruCache_VerifyIntegrity());

    LONGS_EQUAL(2, LruCache_CountCacheEntries());

    d.key = 5;
    CHECK_TRUE(LruCache_Remove(d.key));
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    d.key = 7;
    CHECK_TRUE(LruCache_Remove(d.key));
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    LONGS_EQUAL(0, LruCache_CountCacheEntries());
}
TEST(LRUCache, CheckBoundaryValuesLookup)
{
    d.key = 0;
    d.value = 0;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 99;
    d.value = 999;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 100;
    d.value = 100;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 199;
    d.value = 9999;
    CHECK_TRUE(LruCache_Insert(d));

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(0, result.key);
    LONGS_EQUAL(0, result.value);

    CHECK_TRUE(LruCache_VerifyIntegrity());

    LONGS_EQUAL(TRUE, LruCache_Lookup(100,&result));
    LONGS_EQUAL(100, result.key);
    LONGS_EQUAL(100, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(0, result.key);
    LONGS_EQUAL(0, result.value);

    LONGS_EQUAL(TRUE, LruCache_Lookup(199,&result));
    LONGS_EQUAL(199, result.key);
    LONGS_EQUAL(9999, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(0, result.key);
    LONGS_EQUAL(0, result.value);

    LONGS_EQUAL(TRUE, LruCache_Lookup(0,&result));
    LONGS_EQUAL(0, result.key);
    LONGS_EQUAL(0, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(99, result.key);
    LONGS_EQUAL(999, result.value);

    LONGS_EQUAL(TRUE, LruCache_Lookup(99,&result));
    LONGS_EQUAL(99, result.key);
    LONGS_EQUAL(999, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(100, result.key);
    LONGS_EQUAL(100, result.value);

    LONGS_EQUAL(4, LruCache_CountCacheEntries());
}

TEST(LRUCache, CheckBoundaryValuesRemove)
{
    d.key = 0;
    d.value = 0;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 99;
    d.value = 999;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 100;
    d.value = 100;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 199;
    d.value = 9999;
    CHECK_TRUE(LruCache_Insert(d));

    CHECK_TRUE(LruCache_VerifyIntegrity());

    d.key = 199;
    CHECK_TRUE(LruCache_Remove(d.key));
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    d.key = 0;
    CHECK_TRUE(LruCache_Remove(d.key));
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    d.key = 99;
    CHECK_TRUE(LruCache_Remove(d.key));
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    d.key = 100;
    CHECK_TRUE(LruCache_Remove(d.key));
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    LONGS_EQUAL(0, LruCache_CountCacheEntries());

}

TEST(LRUCache, CheckValuesAboveMAX)
{
    d.key = 100;
    d.value = 100;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 101;
    d.value = 101;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 199;
    d.value = 19999;
    CHECK_TRUE(LruCache_Insert(d));

    CHECK_TRUE(LruCache_VerifyIntegrity());

    LONGS_EQUAL(TRUE, LruCache_Lookup(100,&result));
    LONGS_EQUAL(100, result.key);
    LONGS_EQUAL(100, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(101, result.key);
    LONGS_EQUAL(101, result.value);

    LONGS_EQUAL(TRUE, LruCache_Lookup(199,&result));
    LONGS_EQUAL(199, result.key);
    LONGS_EQUAL(19999, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(101, result.key);
    LONGS_EQUAL(101, result.value);

    LONGS_EQUAL(TRUE, LruCache_Lookup(101,&result));
    LONGS_EQUAL(101, result.key);
    LONGS_EQUAL(101, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(100, result.key);
    LONGS_EQUAL(100, result.value);

    LONGS_EQUAL(3, LruCache_CountCacheEntries());

}

TEST(LRUCache, InsertMultipleWithDifferentKeysAndSameHashIndexLookup)
{
    d.key = 5;
    d.value = 50;
    CHECK_TRUE(LruCache_Insert(d));
    d.key = 105;
    d.value = 1050;
    CHECK_TRUE(LruCache_Insert(d));
    d.key = 205;
    d.value = 2050;
    CHECK_TRUE(LruCache_Insert(d));
    d.key = 305;
    d.value = 3050;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 5;
    d.value = 50;
    LONGS_EQUAL(TRUE, LruCache_Lookup(d.key,&result));
    LONGS_EQUAL(d.key, result.key);
    LONGS_EQUAL(d.value, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(105, result.key);
    LONGS_EQUAL(1050, result.value);

    d.key = 105;
    d.value = 1050;
    LONGS_EQUAL(TRUE, LruCache_Lookup(d.key,&result));
    LONGS_EQUAL(d.key, result.key);
    LONGS_EQUAL(d.value, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(205, result.key);
    LONGS_EQUAL(2050, result.value);

    d.key = 305;
    d.value = 3050;
    LONGS_EQUAL(TRUE, LruCache_Lookup(d.key,&result));
    LONGS_EQUAL(d.key, result.key);
    LONGS_EQUAL(d.value, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(205, result.key);
    LONGS_EQUAL(2050, result.value);

    d.key = 205;
    d.value = 2050;
    LONGS_EQUAL(TRUE, LruCache_Lookup(d.key,&result));
    LONGS_EQUAL(d.key, result.key);
    LONGS_EQUAL(d.value, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(5, result.key);
    LONGS_EQUAL(50, result.value);

    LONGS_EQUAL(4, LruCache_CountCacheEntries());

    CHECK_TRUE(LruCache_VerifyIntegrity());
}

TEST(LRUCache, InsertMultipleWithDifferentKeysAndSameHashIndexRemoveAll)
{
    d.key = 5;
    d.value = 50;
    CHECK_TRUE(LruCache_Insert(d));
    d.key = 105;
    d.value = 1050;
    CHECK_TRUE(LruCache_Insert(d));
    d.key = 205;
    d.value = 2050;
    CHECK_TRUE(LruCache_Insert(d));
    d.key = 305;
    d.value = 3050;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 305;
    CHECK_TRUE(LruCache_Remove(d.key));
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    LONGS_EQUAL(TRUE, LruCache_FindMostRecent(&result));
    LONGS_EQUAL(205, result.key);
    LONGS_EQUAL(2050, result.value);

    d.key = 5;
    d.value = 50;
    LONGS_EQUAL(TRUE, LruCache_Lookup(d.key,&result));
    LONGS_EQUAL(d.key, result.key);
    LONGS_EQUAL(d.value, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindMostRecent(&result));
    LONGS_EQUAL(5, result.key);
    LONGS_EQUAL(50, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(105, result.key);
    LONGS_EQUAL(1050, result.value);

    d.key = 205;
    CHECK_TRUE(LruCache_Remove(d.key));
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    LONGS_EQUAL(TRUE, LruCache_FindMostRecent(&result));
    LONGS_EQUAL(5, result.key);
    LONGS_EQUAL(50, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(105, result.key);
    LONGS_EQUAL(1050, result.value);

    d.key = 105;
    CHECK_TRUE(LruCache_Remove(d.key));
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    LONGS_EQUAL(TRUE, LruCache_FindMostRecent(&result));
    LONGS_EQUAL(5, result.key);
    LONGS_EQUAL(50, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindOldest(&result));
    LONGS_EQUAL(5, result.key);
    LONGS_EQUAL(50, result.value);

    POINTERS_EQUAL(first, last);

    d.key = 5;
    CHECK_TRUE(LruCache_Remove(d.key));
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    LONGS_EQUAL(0, LruCache_CountCacheEntries());

    CHECK_TRUE(LruCache_VerifyIntegrity());
}

TEST(LRUCache, InsertMultipleWithDifferentKeysAndSameHashIndexRemoveOldest)
{
    d.key = 5;
    d.value = 50;
    CHECK_TRUE(LruCache_Insert(d));
    d.key = 105;
    d.value = 1050;
    CHECK_TRUE(LruCache_Insert(d));
    d.key = 205;
    d.value = 2050;
    CHECK_TRUE(LruCache_Insert(d));
    d.key = 305;
    d.value = 3050;
    CHECK_TRUE(LruCache_Insert(d));

    CHECK_TRUE(LruCache_RemoveOldest(&result));
    LONGS_EQUAL(5, result.key);
    LONGS_EQUAL(50, result.value);

    d.key = 5;
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    LONGS_EQUAL(TRUE, LruCache_FindMostRecent(&result));
    LONGS_EQUAL(305, result.key);
    LONGS_EQUAL(3050, result.value);

    d.key = 105;
    d.value = 1050;
    LONGS_EQUAL(TRUE, LruCache_Lookup(d.key,&result));
    LONGS_EQUAL(d.key, result.key);
    LONGS_EQUAL(d.value, result.value);

    LONGS_EQUAL(TRUE, LruCache_FindMostRecent(&result));
    LONGS_EQUAL(105, result.key);
    LONGS_EQUAL(1050, result.value);

    CHECK_TRUE(LruCache_RemoveOldest(&result));
    LONGS_EQUAL(205, result.key);
    LONGS_EQUAL(2050, result.value);

    d.key = 205;
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    d.key = 105;
    CHECK_TRUE(LruCache_Remove(d.key));
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    POINTERS_EQUAL(first, last);

    CHECK_TRUE(LruCache_RemoveOldest(&result));
    LONGS_EQUAL(305, result.key);
    LONGS_EQUAL(3050, result.value);

    d.key = 305;
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    POINTERS_EQUAL(first, last);


    LONGS_EQUAL(0, LruCache_CountCacheEntries());

    CHECK_TRUE(LruCache_VerifyIntegrity());
}

TEST(LRUCache, InsertMultipleRemoveOldest)
{
    d.key = 5;
    d.value = 50;
    CHECK_TRUE(LruCache_Insert(d));
    d.key = 7;
    d.value = 70;
    CHECK_TRUE(LruCache_Insert(d));
    d.key = 8;
    d.value = 80;
    CHECK_TRUE(LruCache_Insert(d));

    CHECK_TRUE(LruCache_RemoveOldest(&result));
    LONGS_EQUAL(5, result.key);
    LONGS_EQUAL(50, result.value);

    d.key = 5;
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    CHECK_TRUE(LruCache_RemoveOldest(&result));
    LONGS_EQUAL(7, result.key);
    LONGS_EQUAL(70, result.value);

    d.key = 7;
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    CHECK_TRUE(LruCache_RemoveOldest(&result));
    LONGS_EQUAL(8, result.key);
    LONGS_EQUAL(80, result.value);

    d.key = 8;
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    LONGS_EQUAL(0, LruCache_CountCacheEntries());
}

TEST(LRUCache, InsertMultipleRemoveOldestCombinations)
{
    d.key = 5;
    d.value = 50;
    CHECK_TRUE(LruCache_Insert(d));

    CHECK_TRUE(LruCache_RemoveOldest(&result));
    LONGS_EQUAL(5, result.key);
    LONGS_EQUAL(50, result.value);

    d.key = 5;
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    LONGS_EQUAL(0, LruCache_CountCacheEntries());

    d.key = 5;
    d.value = 50;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 7;
    d.value = 70;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 5;
    LONGS_EQUAL(TRUE, LruCache_Lookup(d.key,&result));
    LONGS_EQUAL(5, result.key);
    LONGS_EQUAL(50, result.value);

    CHECK_TRUE(LruCache_RemoveOldest(&result));
    LONGS_EQUAL(7, result.key);
    LONGS_EQUAL(70, result.value);

    d.key = 7;
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    LONGS_EQUAL(1, LruCache_CountCacheEntries());

    d.key = 8;
    d.value = 80;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 7;
    d.value = 70;
    CHECK_TRUE(LruCache_Insert(d));

    d.key = 5;
    LONGS_EQUAL(TRUE, LruCache_Lookup(d.key,&result));
    LONGS_EQUAL(5, result.key);
    LONGS_EQUAL(50, result.value);

    CHECK_TRUE(LruCache_RemoveOldest(&result));
    LONGS_EQUAL(8, result.key);
    LONGS_EQUAL(80, result.value);

    d.key = 8;
    LONGS_EQUAL(FALSE, LruCache_Lookup(d.key,&result));

    LONGS_EQUAL(2, LruCache_CountCacheEntries());
}

