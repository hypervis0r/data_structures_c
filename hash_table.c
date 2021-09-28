#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "linked_list.h"

struct person_info
{
    char* name;
    int age;
};

struct hash_table_item
{
    struct hash_table_item* next;
    char* key;
    struct person_info* value;
};

struct hash_table
{
    struct hash_table_item** items;
    size_t size;
};

void print_hash_table(struct hash_table* table);

void* search_linked_list(struct hash_table_item* head, char* key)
{
    do
    {
        if (strcmp(head->key, key) == 0)
            return head->value;
    } while ((head = head->next));

    return NULL;
}

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
    item->next = NULL;

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

    table->items = calloc(table->size, sizeof(*table->items));

    return table;
}

void free_hash_table(struct hash_table* table)
{
    struct hash_table_item* item = NULL;
    struct hash_table_item* popped = NULL;

    for (int i = 0; i < table->size; i++)
    {
        item = table->items[i];
        if (item != NULL)
        {
            /*
                Free every item in the chain
                
                TODO: maybe popping every value from the end isn't
                good for performance, maybe we free from the head?
            */
            do
            {
                popped = (struct hash_table_item *)pop_linked_list((struct Node*)item);
                
                if (popped == item)
                {
                    free_hash_table_item(item);
                    table->items[i] = NULL;
                    break;
                }

                free_hash_table_item(popped);
            } while (popped != NULL);
        }
    }

    print_hash_table(table);

    free(table);
}

void hash_table_insert(struct hash_table* table, struct hash_table_item* item)
{
    struct hash_table_item* indexed_item = NULL;

    /*
        Get hash table index
    */
    uint64_t index = hash_string_djb2(item->key) % table->size;

    indexed_item = table->items[index];

    if (indexed_item == NULL)
    {
        /*
            Insert item into hash index
        */
        table->items[index] = item;
    }
    else
    {
        /* 
            Hash collision, prepend new item to the
            head of the chain.

            TODO: Find a way to modify existing entries without
            costing insert time complexity to become O(n)
        */
        table->items[index] = (struct hash_table_item*)prepend_linked_list(
                (struct Node*)indexed_item, 
                (struct Node*)item);
    }
}

void *hash_table_search(struct hash_table* table, char* key)
{
    /*
        Get hash table index
    */
    uint64_t index = hash_string_djb2(key) % table->size;

    if (table->items[index] == NULL)
        return NULL;
    
    /*
        If the key at the index is different from search key,
        we have to walk the chain to find the value. We do this
        anyways as there is no need to branch here.
    */
    return search_linked_list(table->items[index], key); 
}

void print_hash_table(struct hash_table* table)
{
    struct hash_table_item* item = NULL;

    /*
        Print every valid item index.
    */
    for (int i = 0; i < table->size; i++)
    {
        item = table->items[i];

        if (item)
        {
            /*
                Walk item chain to print all items
            */
            do
            {
                printf("%d:\tname:%s\tage:%d\tnext:%p\n", i, item->value->name, item->value->age, item->next);
            } while ((item = item->next));
        }
    }
}

int main(void)
{
    struct hash_table* table = create_hash_table(3);

    struct person_info john = { "john", 22 };
    struct person_info ringo = { "ringo", 33 };
    struct person_info george = { "george", 44 };

    struct hash_table_item* john_item = create_hash_table_item(
            john.name, strlen(john.name) + 1, 
            &john, sizeof(john));

    struct hash_table_item* ringo_item = create_hash_table_item(
            ringo.name, strlen(ringo.name) + 1,
            &ringo, sizeof(ringo));

    struct hash_table_item* george_item = create_hash_table_item(
            george.name, strlen(george.name) + 1,
            &george, sizeof(george));

    printf("\n------[initial]------\n");

    hash_table_insert(table, john_item);
    hash_table_insert(table, ringo_item);
    hash_table_insert(table, george_item);

    print_hash_table(table);

    printf("\n------[search]------\n");
    
    struct person_info* john2 = hash_table_search(table, "john");
    printf("john:\tname:%s\tage:%d\n", john2->name, john2->age);
    
    struct person_info* ringo2 = hash_table_search(table, "ringo");
    printf("ringo:\tname:%s\tage:%d\n", ringo2->name, ringo2->age);

    struct person_info* george2 = hash_table_search(table, "george");
    printf("george:\tname:%s\tage:%d\n", george2->name, george2->age);

    free_hash_table(table);
}
