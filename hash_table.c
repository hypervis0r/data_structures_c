#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct person_info
{
    char* name;
    int age;
};

struct hash_table_item
{
    char* key;
    struct person_info* value;
};

struct hash_table
{
    struct hash_table_item** items;
    size_t size;
    size_t count;
};

uint64_t hash_string_djb2(char* str)
{
    uint64_t hash = 0x1337133713371337;
	char c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;

	return hash;
}

struct hash_table_item* create_hash_table_item(char* key, size_t key_len, void* value, size_t value_size)
{
    struct hash_table_item* item = malloc(sizeof(*item));

    item->key = malloc(key_len);
    item->value = malloc(value_size);

    memcpy(item->key, key, key_len);
    memcpy(item->value, value, value_size);

    return item;
}

void free_hash_table_item(struct hash_table_item* item)
{
    free(item->value);
    free(item->key);

    free(item);
}

struct hash_table* create_hash_table(size_t size)
{
    struct hash_table* table = malloc(sizeof(*table));

    table->size = size;
    table->count = 0;

    table->items = calloc(table->size, sizeof(*table->items));

    return table;
}

void free_hash_table(struct hash_table* table)
{
    struct hash_table_item* item = NULL;

    for (int i = 0; i < table->size; i++)
    {
        item = table->items[i];
        if (item != NULL)
            free_hash_table_item(item);
    }

    free(table);
}

void hash_table_insert(struct hash_table* table, struct hash_table_item* item)
{
    struct hash_table_item* indexed_item = NULL;

    uint64_t index = hash_string_djb2(item->key) % table->size;

    indexed_item = table->items[index];

    if (indexed_item == NULL)
    {
        if (table->count >= table->size)
        {
            printf("[-] Table overflow");
            exit(-1);
        }

        table->items[index] = item;
        ++table->count;
    }
}

struct hash_table_item* hash_table_search(struct hash_table* table, char* key)
{
    uint64_t index = hash_string_djb2(key) % table->size;

    if (table->items[index] == NULL)
        return NULL;

    return strcmp(table->items[index]->key, key) ? NULL : table->items[index];
}

void print_hash_table(struct hash_table* table)
{
    struct hash_table_item* item = NULL;

    for (int i = 0; i < table->size; i++)
    {
        item = table->items[i];

        if (item)
            printf("%d:\tname:%s\tage:%d\n", i, item->value->name, item->value->age);
    }
}

int main(void)
{
    struct hash_table* table = create_hash_table(256);

    struct person_info john = { "john", 22 };
    struct person_info ringo = { "ringo", 33 };

    struct hash_table_item* john_item = create_hash_table_item(
            john.name, strlen(john.name) + 1, 
            &john, sizeof(john));

    struct hash_table_item* ringo_item = create_hash_table_item(
            ringo.name, strlen(ringo.name) + 1,
            &ringo, sizeof(ringo));

    hash_table_insert(table, john_item);
    hash_table_insert(table, ringo_item);

    print_hash_table(table);
}
