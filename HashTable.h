//
// Created by Yasmeena Khalaf on 12/07/2026.
//

#ifndef DS_WET2_SPRING_2026_3_HASHTABLE_H
#define DS_WET2_SPRING_2026_3_HASHTABLE_H


template<typename K, typename V>
class HashTable {
private:
    struct HashNode {
        K key;
        V value;
        HashNode* next;
        HashNode(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    HashNode** table;
    int capacity;
    int size;

    int hashFunc(const K& key) const {
        long long k = static_cast<long long>(key);
        return static_cast<int>(((k % capacity) + capacity) % capacity);
    }

    void rehash() {
        int oldCapacity = capacity;
        int newCapacity = capacity * 2;

        HashNode** newTable = new HashNode*[newCapacity]();

        for (int i = 0; i < oldCapacity; ++i) {
            HashNode* curr = table[i];

            while (curr != nullptr) {
                HashNode* nextNode = curr->next;

                long long k = static_cast<long long>(curr->key);
                int newIndex =
                    static_cast<int>(((k % newCapacity) + newCapacity) %
                                     newCapacity);

                curr->next = newTable[newIndex];
                newTable[newIndex] = curr;

                curr = nextNode;
            }
        }

        delete[] table;

        table = newTable;
        capacity = newCapacity;

    }

public:
    explicit HashTable(int initCapacity = 16) : capacity(initCapacity), size(0) {
        table = new HashNode*[capacity]();
    }

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    ~HashTable() {
        for (int i = 0; i < capacity; ++i) {
            HashNode* curr = table[i];
            while (curr != nullptr) {
                HashNode* toDelete = curr;
                curr = curr->next;
                delete toDelete;
            }
        }
        delete[] table;
    }

    template<typename Func>
    void forEachValue(Func func) {
        for (int i = 0; i < capacity; ++i) {
            HashNode* current = table[i];

            while (current != nullptr) {
                func(current->value);
                current = current->next;
            }
        }
    }

    // הוספת איבר לטבלה
    bool insert(const K& key, const V& value) {

        if (size >= capacity)
        {
            rehash();
        }

        int idx = hashFunc(key);
        HashNode* curr = table[idx];

        while (curr != nullptr) {
            if (curr->key == key) {
                return false;
            }
            curr = curr->next;
        }


        HashNode* newNode = new HashNode(key, value);
        newNode->next = table[idx];
        table[idx] = newNode;
        size++;

        return true;
    }



    // חיפוש איבר בטבלה
    V* find(const K& key) const {
        int idx = hashFunc(key);
        HashNode* curr = table[idx];
        while (curr != nullptr) {
            if (curr->key == key) {
                return &(curr->value);
            }
            curr = curr->next;
        }
        return nullptr;
    }



    // מחיקת איבר מהטבלה
    bool remove(const K& key) {
        int idx = hashFunc(key);
        HashNode* curr = table[idx];
        HashNode* prev = nullptr;

        while (curr != nullptr) {
            if (curr->key == key) {
                if (prev == nullptr) {
                    table[idx] = curr->next;
                } else {
                    prev->next = curr->next;
                }
                delete curr;
                size--;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }


    int getSize() const {
        return size;
    }
};


#endif //DS_WET2_SPRING_2026_3_HASHTABLE_H
