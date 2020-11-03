//LRU CACHE Algorithm
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct lru_hash_node_s
{
	long key;
	void *data; 

	struct lru_hash_node_s *h_next; //哈希链表的下一个
	struct lru_hash_node_s *prev; 	//双向链表的前一个
	struct lru_hash_node_s *next; 	//双向链表的下一个	
}lru_hash_node_t;

typedef struct lru_hash_cache_s
{
	int cache_size;
	int hash_table_size;	
	struct lru_hash_node_s * *hash_table;	

	struct lru_hash_node_s *head;
	struct lru_hash_node_s *tail;
		
}lru_hash_cache_t;

lru_hash_cache_t * create_lru_hash_cache(int hash_table_size)
{
	lru_hash_cache_t * lru_cache = (lru_hash_cache_t*)malloc(sizeof(lru_hash_cache_t));
	
	if(lru_cache == NULL) return NULL;
	memset(lru_cache, 0, sizeof(lru_hash_cache_t));

	lru_cache->hash_table_size = hash_table_size;

	lru_cache->hash_table = (lru_hash_node_t**) malloc(sizeof(lru_hash_node_t *) * hash_table_size);
	memset(lru_cache->hash_table, 0, sizeof(lru_hash_node_t *) * hash_table_size);

	return lru_cache;
}

lru_hash_node_t * get_node(lru_hash_cache_t * lru_cache, long key)
{
	lru_hash_node_t * node, * head;
	if(lru_cache == NULL) return NULL;

	node = head = lru_cache->hash_table[key % lru_cache->hash_table_size];
	
	lru_hash_node_t * prev = NULL;
	while(node != NULL 
		&& node->key != key)
	{
		prev = node;
		node = node->h_next;
	}

	if(prev != NULL)
	{
		if(node != NULL)
			prev->h_next = node->h_next;	
		else
			prev->h_next = NULL;
	}
	if(node != NULL)
	{
		node->h_next = head;
		lru_cache->hash_table[key % lru_cache->hash_table_size] = node;
	}
	
	return node;
}

void clear_lru_cache_node(lru_hash_cache_t* lru_cache, long key)
{
	int hash_key = key % lru_cache->hash_table_size;
	lru_hash_node_t * node = get_node(lru_cache, key);
	
	if(node == NULL) return;

	lru_cache->hash_table[hash_key] = node->h_next;

	if(node->prev != NULL)
		node->prev->next = node->next;
	if(node->next != NULL)
		node->next->prev = node->prev;

	if (lru_cache->head == node)
	{
		lru_cache->head = node->next;
	}
	if(lru_cache->tail == node)
	{
		lru_cache->tail = node->prev;
	}
	
	node->data = NULL;
	free(node);
	lru_cache->cache_size--;
}

lru_hash_node_t * create_cache_node(int key, void * data)
{
	lru_hash_node_t * node = malloc(sizeof(lru_hash_node_t));

	if(node == NULL) return NULL;

	memset(node, 0, sizeof(lru_hash_node_t));
	node->key = key;
	node->data = data;

	return node;
}

void insert_lru_cache_node(lru_hash_cache_t * lru_cache, long key, void * data)
{
	lru_hash_node_t * node = get_node(lru_cache, key);	

	if(node != NULL)
	{
		node->data = data;
		return;
	}
	else
	{
		lru_hash_node_t * node = create_cache_node(key, data);
		if(lru_cache->head == NULL)
			lru_cache->head = node;	
		if(lru_cache->tail == NULL)
			lru_cache->tail = node;
		else
		{
			//追加于双向链表尾部
			node->prev = lru_cache->tail;
			lru_cache->tail->next = node;
			lru_cache->tail = node;
		}
		
		node->h_next = lru_cache->hash_table[key % lru_cache->hash_table_size];
		lru_cache->hash_table[key % lru_cache->hash_table_size] = node;
		lru_cache->cache_size++;
	}
}

void release_lru_hash_cache(lru_hash_cache_t * lru_cache)
{
	lru_hash_node_t * node = lru_cache->head;
	lru_hash_node_t * next_node = NULL;
	while(node != NULL)
	{
		lru_hash_node_t * next_node = node->next;
		free(node);
		node = next_node;
	}
	
	free(lru_cache->hash_table);
	lru_cache->hash_table = NULL;
	free(lru_cache);
	lru_cache = NULL;
}


// ================================== test case ==================================
void print_cache(lru_hash_cache_t * lru_cache)
{
	if(lru_cache == NULL)
	{
		printf("empty cache\n");
		return;
	}
	lru_hash_node_t * node = lru_cache->head;
	int i = 0;
	printf("cache content:\n");
	printf("cache size: %d\n", lru_cache->cache_size);
	while(node != NULL)
	{
		printf("node[%d] key=%ld, data=%s\n", i, node->key, (char *)node->data);
		node = node->next;
		i++;
	}
}

void print_cache_by_hash_key(lru_hash_cache_t * lru_cache, long hash_key)
{
	if(lru_cache == NULL) return;
	lru_hash_node_t * node = lru_cache->hash_table[hash_key];
	int i = 0;
	printf("cache hash_linked content:\n");
	while(node != NULL)
	{
		printf("node[%d] key=%ld, data=%s\n", i, node->key, (char *)node->data);
		node = node->h_next;
		i++;
	}
}

void test()
{
	lru_hash_cache_t * lru_cache = create_lru_hash_cache(5);
	char t0[] = "sgsfsds";
	char t1[] = "sgvvvv";
	char t2[] = "sgpppp";
	insert_lru_cache_node(lru_cache, 2, t0);
	insert_lru_cache_node(lru_cache, 7, t1);
	insert_lru_cache_node(lru_cache, 4, t2);

	print_cache_by_hash_key(lru_cache, 2);
	clear_lru_cache_node(lru_cache, 2);
	print_cache(lru_cache);
	release_lru_hash_cache(lru_cache);
}


int
main()
{
	test();
}
