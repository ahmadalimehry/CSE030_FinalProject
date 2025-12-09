#ifndef HASHMAP_H
#define HASHMAP_H 1
#include <ArrayList.h>
#include <iostream>
#include <ostream>
#include <string>
using std::string;

// only maps str->str, doesn't allow for deleting keys, cause thats for losers
class KeyIterator;

struct KeyEntry {
    KeyEntry *prev_key;
    KeyEntry *next_key;
    char *value;
};
template <typename T> class Hashmap {

    struct KeyEntry *first_entry;
    struct KeyEntry *last_entry;
    uint size;
    float load_factor;
    struct KeyEntry *_map_key;
    ArrayList<T> _map_value;

    uint hashString(string s) const {
        uint out = 0;
        for (uint i = 0; i < s.length(); i++) {
            char c = s[i];
            out *= 101;
            out += c;
        }
        return out;
    }
    char *stringToPtr(string s) {
        char *key_copy = (char *)malloc(sizeof(char) * (s.length() + 1));
        for (uint i = 0; i < s.length(); i++) {
            (*(key_copy + i)) = s[i];
        }
        *(key_copy + s.length()) = '\0'; // null terminate
        return key_copy;
    }
    uint getIndex(string s) const {
        return getIndex(s, getCapacity(), _map_key);
    }

    uint getIndex(string s, uint capacity, struct KeyEntry *_map_key) const {
        uint i = hashString(s);
        uint index = i % capacity;
        while ((_map_key[index].value != nullptr) &&
               (s != _map_key[index].value)) {
            // linear probing
            i++;
            index = i % capacity;
        }
        return index;
    }
    void inflate() {
        uint capacity = getCapacity();
        while ((float)size / capacity > load_factor) {
            capacity *= 2;
        }
        if (capacity == getCapacity())
            return;
        // std::cout << "Inflating to Capacity: " << capacity << "\n";
        struct KeyEntry *new_map_key =
            (struct KeyEntry *)calloc(sizeof(struct KeyEntry), capacity);
        auto new_map_value = ArrayList<T>(capacity);
        KeyEntry *first = nullptr;
        KeyEntry *prev = nullptr;
        for (auto kv : items()) {
            char *key = kv.first;
            T value = kv.second;
            uint index =
                getIndex(key, capacity, new_map_key); // might need std::string

            new_map_key[index].value = key;
            new_map_key[index].prev_key = prev;

            if (prev != nullptr)
                prev->next_key = &(new_map_key[index]);
            prev = &(new_map_key[index]);
            new_map_value[index] = value;

            if (first == nullptr) {
                first = &(new_map_key[index]);
            }
        }
        free(_map_key);

        _map_value = new_map_value;
        _map_key = new_map_key;
        first_entry = first;
        last_entry = prev;
    }

public:
    Hashmap() : Hashmap(0.7, 10) {}
    Hashmap(float load_factor, uint initial_capacity) {
        first_entry = nullptr;
        last_entry = nullptr;
        size = 0;
        this->load_factor = load_factor;
        _map_key = (KeyEntry *)calloc(sizeof(KeyEntry), initial_capacity);
        _map_value.resize(initial_capacity);
    }
    bool contains(string key) {
        uint index = getIndex(key);
        return _map_key[index].value != nullptr;
    }
    T get(string key) const {
        uint index = getIndex(key);
        return _map_value[index];
    }

    void set(string key, T value) {

        uint index = getIndex(key);

        if (_map_key[index].value != nullptr)
            free((void *)_map_key[index].value);
        else {
            size++;
            _map_key[index].next_key = nullptr;
            if (first_entry == nullptr) {
                first_entry = &(_map_key[index]);
            }
            if (last_entry != nullptr) {
                last_entry->next_key = &(_map_key[index]);
                _map_key[index].prev_key = last_entry;
            }
        }
        _map_key[index].value = stringToPtr(key);

        _map_value[index] = T(value);

        last_entry = &(_map_key[index]);
        inflate(); // only actually resizes if needed
    }
    uint getSize() const { return size; }
    uint getCapacity() const { return _map_value.size(); }
    void setCapacity(uint capacity) { _map_value.resize(capacity); }

    ~Hashmap() {
        for (uint i = 0; i < getCapacity(); i++) {
            if (_map_key[i].value != nullptr) {
                free((void *)_map_key[i].value);
            }
        }
        free(_map_key);
    }

    ArrayList<char *> keys() const {
        ArrayList<char *> out;
        struct KeyEntry *curEntry = first_entry;
        while (curEntry != nullptr) {
            out.append(curEntry->value);
            curEntry = curEntry->next_key;
        }
        return out;
    }
    ArrayList<std::pair<char *, T>> items() const {
        ArrayList<std::pair<char *, T>> out;
        struct KeyEntry *curEntry = first_entry;
        uint i = size;
        while (curEntry != nullptr && (i--)) {

            uint index = curEntry - _map_key;
            out.append(
                std::pair<char *, T>(curEntry->value, _map_value[index]));
            curEntry = curEntry->next_key;
        }
        return out;
    }

    // friend std::ostream &operator<<(std::ostream &os, const Hashmap<T> &map);
};
template <typename T>
std::ostream &operator<<(std::ostream &os, const Hashmap<T> &map) {
    os << "HashMap: {";
    if (map.getSize()) {
        auto items = map.items();
        int i = 0;
        for (; i < items.size(); i++) {
            auto kv = items[i];
            os << kv.first << ": " << kv.second;
            if (i + 1 != items.size()) {
                os << ", ";
            }
        }
    }
    os << "}";
    return os;
}

#endif