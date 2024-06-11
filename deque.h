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
    Deque();
    ~Deque();
    Deque(const Deque &object);
    Deque(Deque &&object);

    // Accessors

    int size();
    bool is_empty();
    T &front();
    T &back();
    T &operator[](int i);

    // Modifiers

    void push_back(T data);
    void push_front(T data);
    T pop_back();
    T pop_front();
    void clear();
    T &operator=(T &rval);
    T &operator=(T &&rval);
    void erase(int i);
};

template <typename T>
inline Deque<T>::Deque()
{
}

template <typename T>
inline Deque<T>::~Deque()
{
    clear();

    delete head;
    delete tail;
    head = nullptr;
    tail = nullptr;
}

/// @brief Copy constructor
/// @param object object to copy
template <typename T>
inline Deque<T>::Deque(const Deque &object)
{
    if (object.size() == 0)
    {
        return;
    }

    for (int i = 0; i < object.size(); i++)
    {
        this->push_back(object[i]);
    }
}

/// @brief Move constructor
/// @param object object to steal
template <typename T>
inline Deque<T>::Deque(Deque &&object)
{
    head = object.head;
    tail = object.tail;

    object.head = nullptr;
    object.tail = nullptr;
}

template <typename T>
inline int Deque<T>::size()
{
    node *cursor = head;
    int counter = 0;

    while (cursor != nullptr)
    {
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
inline T &Deque<T>::front()
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
inline T &Deque<T>::back()
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

/// @brief
/// @tparam T datatype
/// @param i index
/// @return reference to object at index i
/// @exception OutOfBounds returned when referencing an invalid index
template <typename T>
inline T &Deque<T>::operator[](int i)
{
    if (i >= this->size())
    {
        throw OutOfBounds();
    }

    node *cursor = this->head;
    for (; i > 0; i--)
    {
        cursor = cursor->next;
    }

    return cursor->data;
}

/// @brief Add element at the end of the deque
/// @param data value to be copied or moved to the new element
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

        if (tail == head)
        {
            tail = nullptr;
            head = nullptr;
        }
        else
        {
            tail = tail->prev;
            tail->next = nullptr;
        }

        delete temp->next;
        temp->next = nullptr;

        temp->prev = nullptr;
        delete temp->prev;
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

        temp->next = nullptr;
        delete temp->next;
        temp->next = nullptr;

        delete temp->prev;
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

/// @brief Copy assignment operator
/// @param rval
/// @return
template <typename T>
inline T &Deque<T>::operator=(T &rval)
{
    if (this != &rval)
    {
        node *cursor = rval.head;
        while (cursor != nullptr)
        {
            push_back(cursor->data);
            cursor = cursor->next;
        }
    }

    return *this;
}

/// @brief Move assignment operator
/// @param rval
/// @return
template <typename T>
inline T &Deque<T>::operator=(T &&rval)
{
    if (this != &rval)
    {
        swap(head, rval.head);
        swap(tail, rval.tail);

        rval.head = nullptr;
        rval.tail = nullptr;
    }

    return *this;
}

/// @brief Erases an element
/// @param i index of element to erase
/// @exception OutOfBounds returned when referencing an invalid index
template <typename T>
inline void Deque<T>::erase(int i)
{
    if (i >= size())
        throw OutOfBounds();

    if (head == nullptr) {
        return;
    }

    node *currentNode = head;
    for (; i>0; i--) {
        currentNode = currentNode->next;
    }

    node *nextNode = currentNode->next;
    node *prevNode = currentNode->prev;

    if (prevNode != nullptr) {
        prevNode->next = nextNode;
    }

    if (nextNode != nullptr) {
        nextNode->prev = prevNode;
    }
}

#endif
