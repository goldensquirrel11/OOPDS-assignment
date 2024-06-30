/*
 *
 * This file contains the Deque (Double-ended queue) class that is used
 * as a container for queue-like behavior
 *
 * --- Code Summary ---
 * The deque is implemented using a node-linking structure with node containing
 * pointers to the previous node & pointers to the next node. The deque also
 * has the added ability to reference contained elements using the [] operator.
 *
 * --- Reasoning for creating deque instead of queue ---
 * I had initially chose to implement a deque instead of a normal queue class
 * because I was uncertain at time about how I would like to structure my
 * entire program. So I thought that having the flexibility of deque might help
 * future me make more flexible decisions about my program structure.
 *
 */

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

    /*---------------------------------------------*/
    /*                 Accessors                   */
    /*---------------------------------------------*/

    int size();
    bool is_empty();
    T &front();
    T &back();
    T &operator[](int i);

    /*---------------------------------------------*/
    /*                 Modifiers                   */
    /*---------------------------------------------*/

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

/// @brief Returns the number of elements in the deque
/// @return int detailing how many elements are in the deque
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

/// @brief checks whether the deque is empty
/// @return true if the deque is empty, false otherwise
template <typename T>
inline bool Deque<T>::is_empty()
{
    if (head == nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/// @brief references the element at the front of the deque
/// @return reference to the element at the front of the deque
/// @exception OutOfBounds thrown if the deque is empty
template <typename T>
inline T &Deque<T>::front()
{
    if (is_empty())
    {
        throw OutOfBounds();
    }
    
    return head->data;
}

/// @brief references the element at the back of the deque
/// @return reference to the element at the back of the deque
/// @exception OutOfBounds thrown if the deque is empty
template <typename T>
inline T &Deque<T>::back()
{
    if (is_empty())
    {
        throw OutOfBounds();
    }

    return tail->data;
}

/// @brief
/// @tparam T datatype
/// @param i index
/// @return reference to object at index i
/// @exception OutOfBounds thrown when referencing an invalid index
template <typename T>
inline T &Deque<T>::operator[](int i)
{
    if (i >= this->size() || i < 0)
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
    // Create new node
    node *new_node = new node;
    new_node->data = data;

    // if the list is empty
    if (tail == nullptr)
    {
        tail = new_node;
        head = new_node;

        return;
    }

    tail->next = new_node;
    new_node->prev = tail;

    tail = new_node;
}

/// @brief Pushes a new element to the front of the deque
/// @param data element to push to the front of the deque
template <typename T>
inline void Deque<T>::push_front(T data)
{
    // Create new node
    node *new_node = new node;
    new_node->data = data;
    
    // If the list is empty
    if (is_empty())
    {
        head = new_node;
        tail = new_node;

        return;
    }

    head->prev = new_node;
    new_node->next = head;

    head = new_node;
}

/// @brief Pops out the object at the back of the deque
/// @return copy of object at the back of the deque
/// @exception OutOfBounds thrown if the deque is empty
template <typename T>
inline T Deque<T>::pop_back()
{
    if (tail == nullptr)
    {
        throw OutOfBounds();
    }

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

    temp->next = nullptr;
    temp->prev = nullptr;

    delete temp->next;
    delete temp->prev;

    temp->prev = nullptr;
    temp->next = nullptr;

    delete temp;
    temp = nullptr;

    return returnData;
}

/// @brief Pops out the object at the front of the deque
/// @return copy of object at the front of the deque
/// @exception OutOfBounds thrown if the deque is empty
template <typename T>
inline T Deque<T>::pop_front()
{
    if (is_empty())
    {
        throw OutOfBounds();
    }

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
    temp->prev = nullptr;

    delete temp->next;
    delete temp->prev;
    
    temp->prev = nullptr;
    temp->next = nullptr;

    delete temp;
    temp = nullptr;

    return returnData;
}

/// @brief Clears all elements from the deque
template <typename T>
inline void Deque<T>::clear()
{
    while (!is_empty())
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
/// @exception OutOfBounds thrown when referencing an invalid index
template <typename T>
inline void Deque<T>::erase(int i)
{
    if (i >= size() || i < 0) {
        throw OutOfBounds();
    }

    if (is_empty())
    {
        throw OutOfBounds();
    }

    node *currentNode = head;
    for (; i > 0; i--)
    {
        currentNode = currentNode->next;
    }

    node *nextNode = currentNode->next;
    node *prevNode = currentNode->prev;

    if (prevNode != nullptr)
    {
        prevNode->next = nextNode;
    }

    if (nextNode != nullptr)
    {
        nextNode->prev = prevNode;
    }

    if (currentNode == tail) {
        tail = prevNode;
    }

    if (currentNode == head)
    {
        head = nextNode;
    }

    nextNode = nullptr;
    prevNode = nullptr;
    delete nextNode;
    delete prevNode;
    nextNode = nullptr;
    prevNode = nullptr;

    delete currentNode;
    currentNode = nullptr;
}

#endif
