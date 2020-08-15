//queue.cpp --Queue and Customer methods
#include "queue.h"
#include <cstdlib> //for rand()

Queue::Queue(int qs) : qsize(qs)
{
    front = rear = nullptr;
    items = 0;
}
Queue::~Queue()
{
    Node * temp;
    while (front != nullptr) {
        temp = front;//save address of front item
        front = front->next;//reset pointer to next item
        delete temp;//delete former front
    }
}

bool Queue::isempty() const
{
    return items == 0;
}

bool Queue::isfull() const
{
    return items == qsize;
}

int Queue::queueCount() const
{
    return items;
}

//add item to queue
bool Queue::enqueue(const Item & item)
{
    if (isfull())
        return false;
    Node *add = new Node; //create node
    //on failure,new throws std::bad_alloc exception
    add->item = item;//set node pointer
    add->next = nullptr;
    items ++;
    if (front == nullptr)
        front == add;//if queue is empty place item at front
    else
        rear->next = add;//have rear pointer to new node
    rear = add;
    return true;
}
//place front item into item variable and remove from queue
bool Queue::dequeue(Item & item)
{
    if (front == nullptr)
        return false;
    item = front->item;//set item to first item in queue
    items--;
    Node * temp = front;//save location of first item
    front = front->next;//rest front to next item
    delete temp;//delete former first item
    if (items == 0)
        rear = nullptr;
    return true;
}

//time set to a random value in the range 1-3
void Customer::set(long when)
{
    processtime = std::rand() % 3 + 1;
    arrive = when;
}
