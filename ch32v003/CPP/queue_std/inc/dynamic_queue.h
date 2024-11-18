#ifndef DYNAMIC_QUEUE
#define DYNAMIC_QUEUE

// like double linked list

template <typename T> class DQueue {
  public:
    struct Node {
        T t;
        Node* next = nullptr;
        Node* prev = nullptr;
    };
    Node* first = nullptr;
    Node* last = nullptr;
    bool push(T t);
    T pop();
    bool isEmpty() {
        if (!last) {
            return true;
        } else {
            return false;
        }
    }
};

template <typename T> bool DQueue<T>::push(T t) {
    if (!first) {
        last = first = new Node(t);
        if (!last) return false;
    } else if (first == last) {
        // one element
        last = new Node(t);
        if (!last) return false;
        first->next = last;
        last->prev = first;
    } else {
        Node* temp = new Node(t);
        if (!temp) return false;
        last->next = temp;
        temp->prev = last;
        last = temp;
    }
    return true;
}
template <typename T> T DQueue<T>::pop() {
    if (!first)
        return T();
    else {
        Node* t = first->next;
        t->prev = nullptr;
        T temp = first->t;
        delete (first);
        first = t;
        return temp;
    }
    return T();
}

#endif // DYNAMIC_QUEUE
