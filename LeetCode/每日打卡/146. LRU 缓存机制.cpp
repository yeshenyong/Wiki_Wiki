class LRUCache {
private:
    struct ListNode{
        int key;
        int value;
        ListNode* next;
        ListNode* pre;
        ListNode(int key, int value):key(key),value(value),next(nullptr),pre(nullptr){}
    };
    ListNode* dummy;
    ListNode* tail;
    int capacity;
    int size;
    unordered_map<int, ListNode*> addr;
public:
    LRUCache(int capacity) {
        dummy = new ListNode(-1, -1);
        tail = new ListNode(-1, -1);
        this->size = 0;
        this->capacity = capacity;
        dummy->next = tail;
        dummy->pre = nullptr;
        tail->pre = dummy;
        tail->next = nullptr;
    }
    
    int get(int key) {
        if (addr[key] == nullptr) return -1;
        int value = addr[key]->value;
        deletePtr(addr[key]);
        real_put(key, value);
        return value;
    }
    void put(int key, int value) {
        if (addr[key] != nullptr) {
            addr[key]->value = value;
            get(key);
            return;
        }
        if(size < capacity) {
            real_put(key, value);
            size++;
        } else {
            deletePtr(tail->pre);
            size--;
            real_put(key, value);
            size++;
        }
    }
    void real_put(int key, int value) {
        ListNode* node = new ListNode(key, value);
        node->next = dummy->next;
        dummy->next->pre = node;
        dummy->next = node;
        node->pre = dummy;
        addr[key] = node;
    }

    void deletePtr(ListNode* deleteP) {
        deleteP->pre->next = deleteP->next;
        deleteP->next->pre = deleteP->pre;
        addr[deleteP->key] = nullptr;
        delete deleteP;
    }
};