#include <iostream>
#include <map>

using namespace std;

struct Node
{
    int key;
    int val;
    Node *prev, *next;
    Node(int _key, int _value){
       key = _key; 
       val = _value;
       prev = NULL;
       next = NULL;
    }
}

class DoubleList()
{
private;
    Node * head;
    Node * tail;

    DoubleList() {
        head = nullptr; 
        tail = nullptr;
    }

public:
    int addFirst(Node * node)
    {
        if(node == null_ptr) return -1;

        if(head == nullptr)
        {
            head = node;
            tail = node;
        }
        else
        {
            node->next = data;
            head->prev = node;
            head = node;
        }

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

        delete node;
        node = nullptr;
    }
}

class LRUCache
{
private:
    int cache_size;
    int capacity;
    map<int, Node *> hash_map;
    DoubleList data;

public:

    LRUCache(int cap)
    {
        cache_size = 0;
        capacity = cap;
        hash_map = new map<int, Node*>();
        data = new DoubleList();
    }

    int size()
    {
        return cache_size;
    }
    
    int capacity()
    {
        return capacity;
    }

    int get(int key)
    {
        if(hash_map.find(key) == hash_map.end()) return -1;
        
        Node * node = hash_map[key];
        int val = node->val;
        
        data.remove(node);

        Node * new_node = Node(key, val);
        data.addFirst(new_node);
        hash_map[key] = new_node;
    }

    int put(int key, int value)
    {
        if(hash_map.find(key) != hash_map.end())
        {
             
        }
    }
}
