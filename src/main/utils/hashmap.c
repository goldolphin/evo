/**
 * @author goldolphin
 *         2015-07-08 09:40:40.
 */

#include <string.h>
#include "hashmap.h"

typedef struct {
    hashmap_entry_t * next;
} entry_base_t;

struct hashmap_entry_s {
    entry_base_t super;
    entry_base_t * prev;
    bool used;
    pair_t key_value;
};

struct hashmap_bucket_s {
    entry_base_t super;
};

static inline void hashmap_reset(hashmap_t * map) {
    map->size = 0;
    for (size_t i = 0; i < map->bucket_num; ++i) {
        map->buckets[i].super.next = NULL;
    }
    for (size_t i = 0; i < map->capacity; ++i) {
        map->entries[i].used = false;
    }
}

static inline void hashmap_alloc(hashmap_t * map, size_t capacity) {
    map->capacity = capacity;
    map->bucket_num = (size_t) (map->capacity * map->factor);
    map->buckets = new_array(hashmap_bucket_t, map->bucket_num);
    map->entries = new_array(hashmap_entry_t, map->capacity);
}

void hashmap_init(hashmap_t * map, size_t initial_capacity, double factor /* (0, 1] */, hash_func_t hash_func, equal_func_t equal_func) {
    ensure(factor > 0 && factor <= 1.0);
    map->hash_func = hash_func;
    map->equal_func = equal_func;
    map->factor = factor;
    hashmap_alloc(map, initial_capacity);
    hashmap_reset(map);
}

void hashmap_init1(hashmap_t *map, size_t initial_capacity, hash_func_t hash_func, equal_func_t equal_func) {
    hashmap_init(map, initial_capacity, 0.75, hash_func, equal_func);
}

void hashmap_destroy(hashmap_t * map) {
    free(map->buckets);
    free(map->entries);
}

static void hashmap_grow(hashmap_t *map) {
    // Save the snapshot.
    size_t capacity = map->capacity;
    hashmap_bucket_t *buckets = map->buckets;
    hashmap_entry_t *entries = map->entries;

    // Resize the map.
    hashmap_alloc(map, 2*capacity);
    hashmap_reset(map);

    // Copy data.
    for (size_t i = 0; i < capacity; ++i) {
        hashmap_entry_t * entry = &entries[i];
        if (entry->used) {
            hashmap_put(map, entry->key_value.key, entry->key_value.value);
        }
    }

    // Destroy the snapshot.
    free(buckets);
    free(entries);
}

static inline hashmap_iterator_t hashmap_put_new(hashmap_t * map, hashmap_bucket_t * bucket, size_t hash, void * key, void * value) {
    // Ensure capacity.
    while (map->size >= map->bucket_num) {
        hashmap_grow(map);
        bucket = &map->buckets[hash % map->bucket_num]; // We must recalculate bucket address.
    }

    // Allocate a new entry.
    for (size_t i = hash % map->capacity; ; ) {
        hashmap_entry_t * entry = &map->entries[i];
        if (entry->used) {
            i = (i+1) % map->capacity;
            continue;
        }
        pair_set(&entry->key_value, key, value);
        entry->used = true;

        // add to the chain.
        entry->prev = &bucket->super;
        entry->super.next = bucket->super.next;
        if (entry->super.next != NULL) {
            entry->super.next->prev = &entry->super;
        }
        bucket->super.next = entry;
        ++ map->size;
        return entry;
    }
}

/**
 * Fails if an old binding for the key exists.
 */
hashmap_iterator_t hashmap_put(hashmap_t * map, void * key, void * value) {
    size_t hash = map->hash_func(key);
    hashmap_bucket_t * bucket = &map->buckets[hash % map->bucket_num];

    // If the entry exists.
    for (hashmap_entry_t * entry = bucket->super.next; entry != NULL; entry = entry->super.next) {
        if (map->equal_func(key, entry->key_value.key)) {
            return hashmap_end(map);
        }
    }

    // Otherwise
    return hashmap_put_new(map, bucket, hash, key, value);
}

hashmap_iterator_t hashmap_find(hashmap_t *map, void *key) {
    size_t index = map->hash_func(key) % map->bucket_num;
    hashmap_bucket_t * bucket = &map->buckets[index];
    for (hashmap_iterator_t entry = bucket->super.next; entry != NULL; entry = entry->super.next) {
        if (map->equal_func(key, entry->key_value.key)) {
            return entry;
        }
    }
    return hashmap_end(map);
}

void hashmap_remove(hashmap_t * map, hashmap_iterator_t iter) {
    iter->prev->next = iter->super.next;
    if (iter->super.next != NULL) {
        iter->super.next->prev = iter->prev;
    }
    iter->used = false;
    -- map->size;
}

bool hashmap_get(hashmap_t * map, void * key, pair_t * key_value) {
    hashmap_iterator_t entry = hashmap_find(map, key);
    if (entry != NULL) {
        hashmap_iterator_get(entry, key_value);
        return true;
    }
    return false;
}

void hashmap_clear(hashmap_t * map) {
    if (map->size == 0) return;
    hashmap_reset(map);
}

static inline hashmap_iterator_t find_valid_entry(hashmap_t *map, hashmap_iterator_t iter) {
    hashmap_iterator_t end = &map->entries[map->capacity];
    for (; iter < end; iter = &iter[1]) {
        if (iter->used) {
            return iter;
        }
    }
    return hashmap_end(map);
}

hashmap_iterator_t hashmap_begin(hashmap_t * map) {
    return find_valid_entry(map, &map->entries[0]);
}

hashmap_iterator_t hashmap_next(hashmap_t * map, hashmap_iterator_t iter) {
    ensure(iter != hashmap_end(map));
    return find_valid_entry(map, &iter[1]);
}

void hashmap_iterator_get(hashmap_entry_t *iter, pair_t *key_value) {
    pair_copy(key_value, &iter->key_value);
}

void hashmap_iterator_set_value(hashmap_entry_t *iter, void * value) {
    iter->key_value.value = value;
}

size_t naive_hash_func (void * key) {
    return (size_t) key;
}

bool naive_equal_func (void * key1, void * key2) {
    return key1 == key2;
}

size_t str_hash_func (void * key) {
    size_t h = 0;
    for (char * s = key; s[0] != '\0' ; ++s) {
        h = 31*h + s[0];
    }
    return h;
}

bool str_equal_func (void * key1, void * key2) {
    return strcmp(key1, key2) == 0;
}
