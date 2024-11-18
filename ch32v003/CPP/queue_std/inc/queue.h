#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <string.h>

namespace Queue {

template <int SIZE = 4, int SIZE_ARR = 1024>
class Queue {
  public:
    struct Data {
        const char* data;
        uint32_t len;
    };
    static uint32_t size();
    static Data pop();
    static bool push(const char* arr, uint32_t len);
    static bool getMutex() { return mutex; }

  private:
    static uint32_t* last;
    static uint32_t* first;
    static uint32_t bufLenInd[SIZE];
    static char buf[SIZE][SIZE_ARR];
    static bool mutex;
};

template <int SIZE, int SIZE_ARR>
uint32_t* Queue<SIZE, SIZE_ARR>::last = nullptr;
template <int SIZE, int SIZE_ARR>
uint32_t* Queue<SIZE, SIZE_ARR>::first = nullptr;
template <int SIZE, int SIZE_ARR>
uint32_t Queue<SIZE, SIZE_ARR>::bufLenInd[SIZE] = {0};
template <int SIZE, int SIZE_ARR>
char Queue<SIZE, SIZE_ARR>::buf[SIZE][SIZE_ARR] = {0};
template <int SIZE, int SIZE_ARR>
bool Queue<SIZE, SIZE_ARR>::mutex = false;

template <int SIZE, int SIZE_ARR>
uint32_t Queue<SIZE, SIZE_ARR>::size() {
    if (first == nullptr) {
        return 0;
    } else if (first == last) {
        return 1;
    } else if (first < last) {
        return (last - first) + 1;
    } else {
        return (SIZE + 1 - (first - last));
    }
}

template <int SIZE, int SIZE_ARR>
typename Queue<SIZE, SIZE_ARR>::Data Queue<SIZE, SIZE_ARR>::pop() {
    if (mutex) {
        Data data{nullptr, 0};
        return data;
    };
    mutex = true;
    if (first == nullptr) {
        mutex = false;
        Data data{nullptr, 0};
        return data;
    } else if (first == last) {
        // one value in queue
        uint32_t temp = first - bufLenInd;
        first = nullptr;
        last = nullptr;
        mutex = false;
        Data data{&buf[temp][0], bufLenInd[temp]};
        return data;
    } else if (first < last) {
        uint32_t temp = first - bufLenInd;
        first++;
        mutex = false;
        Data data{&buf[temp][0], bufLenInd[temp]};
        return data;
    } else {
        uint32_t temp = first - bufLenInd;
        Data data{&buf[temp][0], bufLenInd[temp]};
        if (first < &bufLenInd[SIZE] - 1) {
            first++;
        } else {
            first = bufLenInd;
        }
        mutex = false;
        return data;
    }
    Data data{nullptr, 0};
    return data;
    mutex = false;
}

template <int SIZE, int SIZE_ARR>
bool Queue<SIZE, SIZE_ARR>::push(const char* arr, uint32_t len) {
    if(len >= SIZE_ARR) return false;
    if (mutex) return false;
    mutex = true;
    if (first == nullptr) {
        bufLenInd[0] = len;
        memcpy(buf, arr, len);
        first = bufLenInd;
        last = bufLenInd;
    } else if (first == last) {
        // one element
        if (last == &bufLenInd[SIZE - 1]) {
            last = bufLenInd;
            bufLenInd[0] = len;
            memcpy(buf, arr, len);
        } else {
            last++;
            bufLenInd[last - bufLenInd] = len;
            memcpy(&buf[last - bufLenInd][0], arr, len);
        }
    } else if (first < last) {
        if (last < (&bufLenInd[SIZE]) - 1) {
            last++;
            bufLenInd[last - bufLenInd] = len;
            memcpy(&buf[last - bufLenInd][0], arr, len);
        } else {
            if (first != bufLenInd) {
                last = bufLenInd;
                bufLenInd[0] = len;
                memcpy(buf, arr, len);
            } else {
                mutex = false;
                return false;
            }
        }
    } else {
        if (last < first - 1) {
            last++;
            bufLenInd[last - bufLenInd] = len;
            memcpy(&buf[last - bufLenInd][0], arr, len);
        } else {
            mutex = false;
            return false;
        }
    }
    mutex = false;
    return true;
}

using QueueReceive = Queue<4, 6>;

} // namespace Queue

#endif // QUEUE_H
