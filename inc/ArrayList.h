#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <iostream>

template <class T> class ArrayList;

template <class T>
std::ostream &operator<<(std::ostream &os, const ArrayList<T> &list);

template <class T> class ArrayList {
protected:
    int count;
    int capacity;
    T *data;

    void copyToArr(T *out, int size) {
        uint end = size < count ? size : count;

        for (uint i = 0; i < end; i++) {
            out[i] = data[i];
        }
    }

    void inflate() {
        capacity *= 2;

        T *old = data;
        data = new T[capacity];

        for (int i = 0; i < count; i++) {
            data[i] = old[i];
        }

        delete[] old;
    }

    void deflate() {
        capacity /= 2;

        T *old = data;
        data = new T[capacity];

        for (int i = 0; i < count; i++) {
            data[i] = old[i];
        }

        delete[] old;
    }

public:
    ArrayList() {
        count = 0;
        capacity = 1;
        data = new T[capacity];
    }
    ArrayList(int initial_size) {
        count = initial_size;
        capacity = initial_size + 1;
        data = new T[capacity];
    }

    ArrayList(const ArrayList &other) {
        count = other.count;
        capacity = other.capacity;
        data = new T[capacity];

        for (int i = 0; i < other.count; i++) {
            data[i] = other.data[i];
        }
    }

    ArrayList &operator=(const ArrayList &other) {
        if (capacity == other.capacity) {
            count = other.count;

            for (int i = 0; i < other.count; i++) {
                data[i] = other.data[i];
            }
        } else {
            T *old = data;

            count = other.count;
            capacity = other.capacity;
            data = new T[capacity];

            for (int i = 0; i < other.count; i++) {
                data[i] = other.data[i];
            }

            delete[] old;
        }

        return *this;
    }

    void append(T value) {
        data[count] = value;
        count++;

        if (count == capacity) {
            inflate();
        }
    }

    void prepend(T value) {
        for (int i = count; i > 0; i--) {
            data[i] = data[i - 1];
        }
        data[0] = value;

        count++;
        if (count == capacity) {
            inflate();
        }
    }

    T removeFirst() {
        if (count == 0) {
            throw std::logic_error("ArrayList is empty!");
        }

        T target = data[0];

        for (int i = 0; i < count - 1; i++) {
            data[i] = data[i + 1];
        }

        count--;
        if (capacity / 2 > count) {
            deflate();
        }

        return target;
    }

    T removeLast() {
        if (count == 0) {
            throw std::logic_error("ArrayList is empty!");
        }

        T target = data[count - 1];

        count--;
        if (capacity / 2 > count) {
            deflate();
        }

        return target;
    }

    bool search(T value) const {
        for (int i = 0; i < count; i++) {
            if (value == data[i]) {
                return true;
            }
        }
        return false;
    }

    void insert(int index, T value) {
        if (index < 0 || index > count) {
            throw std::logic_error("Index is out of bounds");
        }

        for (int i = count; i > index; i--) {
            data[i] = data[i - 1];
        }
        data[index] = value;

        count++;
        if (count == capacity) {
            inflate();
        }
    }

    T findMin() const {
        T m = data[0];

        for (int i = 1; i < count; i++) {
            if (data[i] < m) {
                m = data[i];
            }
        }

        return m;
    }

    T &operator[](int index) const {
        if (index < 0 || index >= count) {
            throw std::logic_error("Array index out of bounds.");
        }

        return data[index];
    }
    void resize(int new_size) {
        if (new_size < capacity) {
            count = new_size;
        } else {
            T *new_data = new T[new_size];
            copyToArr(new_data, new_size);
            T *old = data;
            delete[] old;
            data = new_data;
            count = new_size;
            capacity = new_size;
        }
    }

    int size() const { return count; }

    int getCapacity() const { return capacity; }

    void stats() const {
        std::cout << "Count:     " << count << std::endl;
        std::cout << "Capacity:  " << capacity << std::endl;
        std::cout << std::endl;
    }

    ~ArrayList() { delete[] data; }

    friend std::ostream &operator<< <T>(std::ostream &os,
                                        const ArrayList<T> &list);

    friend struct TestArrayList;
    friend struct Graph;

    struct Iterator {
        T *ptr;
        Iterator(T *p) { ptr = p; }
        T operator*() { return *ptr; }
        Iterator &operator++() {
            ptr++;
            return *this;
        }
        bool operator!=(Iterator other) const { return other.ptr != ptr; }
    };
    Iterator begin() const { return Iterator(data); }
    Iterator end() const { return Iterator(data + count); }
};

template <class T>
std::ostream &operator<<(std::ostream &os, const ArrayList<T> &list) {
    os << "[";
    for (int i = 0; i < list.count; i++) {
        os << list.data[i];
        if (i < list.count - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

#endif