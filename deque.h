#ifndef DEQUE_H
#define DEQUE_H

// Generic double-ended queue class
template <typename T>
class Deque
{
private:
    // Node linking Structure
    //
    //              Head                          Tail      
    // nullptr  <- [node] ->   <- [node] ->   <- [node] ->  nullptr
    //         prev data next prev data next prev data next 
    //
    struct node
    {
        T data;
        node *prev = nullptr;
        node *next = nullptr;
    };

    node *head = nullptr;
    node *tail = nullptr;

    // Out of bounds Exception Class
    class OutOfBounds
    {
    };

public:

    ~Deque();

    // Accessors
    int size();
    bool is_empty();
    T front();
    T back();
    // TODO: copy constructor
    // TODO: move constructor
    // TODO: [] operator overloading

    // Modifiers
    void push_back(T data);
    void push_front(T data);
    T pop_back();
    T pop_front();
    void clear();
};

template <typename T>
inline Deque<T>::~Deque()
{
    clear();

    delete head;
    delete tail;
    head = nullptr;
    tail = nullptr;
}

template <typename T>
inline int Deque<T>::size()
{
        node *cursor = head;
        int counter = 0;

        while (cursor != nullptr) {
            counter += 1;
            cursor = cursor->next;
        }

        return counter;
}

template <typename T>
inline bool Deque<T>::is_empty()
{
    if (head == nullptr || tail == nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <typename T>
inline T Deque<T>::front()
{
    if (head == nullptr)
    {
        throw OutOfBounds();
    }
    else
    {
        return head->data;
    }
}

template <typename T>
inline T Deque<T>::back()
{
    if (tail == nullptr)
    {
        throw OutOfBounds();
    }
    else
    {
        return tail->data;
    }
}

template <typename T>
inline void Deque<T>::push_back(T data)
{
    if (tail == nullptr)
    {
        tail = new node;
        tail->data = data;
        head = tail;
    }
    else
    {
        node *new_node = new node;

        new_node->data = data;

        tail->next = new_node;
        new_node->prev = tail;
        
        tail = new_node;

        delete new_node;
        new_node = nullptr;
    }
}

template <typename T>
inline void Deque<T>::push_front(T data)
{
    if (head == nullptr)
    {
        head = new node;
        head->data = data;
        tail = head;
    }
    else
    {
        node *new_node = new node;

        new_node->data = data;

        head->prev = new_node;
        new_node->next = head;

        head = new_node;
        delete new_node;
        new_node = nullptr;
    }
}

template <typename T>
inline T Deque<T>::pop_back()
{
    if (tail == nullptr)
    {
        throw OutOfBounds();
    }
    else
    {
        node *temp = tail;
        T returnData = temp->data;
        
        if (tail == head) {
            tail = nullptr;
            head = nullptr;
        } else {
            tail = tail->prev;
            tail->next = nullptr;
        }
        
        delete temp->next;
        delete temp->prev;
        temp->next = nullptr;
        temp->prev = nullptr;
        delete temp;
        temp = nullptr;

        return returnData;
    }
}

template <typename T>
inline T Deque<T>::pop_front()
{
    if (head == nullptr)
    {
        throw OutOfBounds();
    }
    else
    {
        node *temp = head;
        T returnData = temp->data;

        if (tail == head)
        {
            tail = nullptr;
            head = nullptr;
        }
        else
        {
            head = head->next;
            head->prev = nullptr;
        }

        delete temp->next;
        delete temp->prev;
        temp->next = nullptr;
        temp->prev = nullptr;
        delete temp;
        temp = nullptr;

        return returnData;
    }
}

template <typename T>
inline void Deque<T>::clear()
{
    while (head != nullptr)
    {
        pop_back();
    }
}

#endif
