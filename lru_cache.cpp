#include <iostream>
#include <map>

using namespace std;

struct Node
{
    int key;
    int val;
    Node *prev, *next;
    Node(int _key, int _value):key(_key), val(_value), prev(nullptr), next(nullptr){}
};

class DoubleList
{
private:
	int _size;

public:
    Node * head;
    Node * tail;
    DoubleList():_size(0), head(nullptr), tail(nullptr) {}

    int addFirst(Node * node)
    {
        if(node == nullptr) return -1;

        if(head == nullptr)
        {
            head = node;
            tail = node;
        }
        else
        {
            node->next = head;
            head->prev = node;
            head = node;
        }

		_size++;

        return 1;
    }

    int remove(Node * node)
    {
        if(node == nullptr) return -1;
        
        if(node->prev != nullptr)
        {
            node->prev->next = node->next;
            if(tail == node)
                tail = node->prev; 
        }
        if(node->next != nullptr)
        {
            node->next->prev = node->prev;
            if(head == node)
                head = node->next; 
        }

        delete node; //释放指针
        node = nullptr;

		_size--; 

		return 1;
    }

    int removeLast()
    {
        Node * node;
        if(tail == nullptr) return -1;        
        
        node = tail;
        if(head == tail)
        {
            head = nullptr;
            tail = nullptr;
        }
        else if(node->prev != nullptr)
        {
            node->prev->next = nullptr;
            tail = node->prev;
        }

		_size--;

        delete node;
        node = nullptr;

		return 1;
    }

	int size()
	{
		return _size;
	}
};

class LRUCache
{
private:
    int capacity;
    map<int, Node *>  hash_map;
    DoubleList data;

public:

    LRUCache(int cap)
    {
        capacity = cap;
    }

	~LRUCache()
	{
	}

    int get_capacity()
    {
        return capacity;
    }

    int get(int key)
    {
        if(hash_map.find(key) == hash_map.end()) return -1;
        
        Node * node = hash_map[key];
        int val = node->val;
        
        data.remove(node);

        Node * new_node = new Node(key, val);
        data.addFirst(new_node);
        hash_map[key] = new_node;

		return val;
    }

    int put(int key, int value)
    {
		if(data.size() == capacity)
		{
			int end_key = data.tail->key;
			data.removeLast();
			hash_map.erase(end_key);
		}

        if(hash_map.find(key) != hash_map.end())
        {
			hash_map.erase(key);
        }
		
		Node * node = new Node(key, value);
		data.addFirst(node);
		hash_map.insert({key, node});
		return 1;
    }
};

int main()
{
	LRUCache cache = LRUCache( 2 /* 缓存容量 */ );

	cout << cache.put(1, 1) << endl;
	cout << cache.put(2, 2) << endl;
	cout << cache.get(1) << endl;       // 返回  1
	cout << cache.put(3, 3) << endl;    // 该操作会使得关键字 2 作废
	cout << cache.get(2) << endl;       // 返回 -1 (未找到)
	cout << cache.put(4, 4) << endl;    // 该操作会使得关键字 1 作废
	cout << cache.get(1) << endl;       // 返回 -1 (未找到)
	cout << cache.get(3) << endl;       // 返回  3
	cout << cache.get(4) << endl;       // 返回  4
}
