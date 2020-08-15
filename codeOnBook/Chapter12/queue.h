//queue.h --interface for a queue
#ifdef QUEUE_H_
#define QUEUE_H_
class Customer
{
private:
    long arrive;    //arrival time for customer
    int processtime;//processing time for customer
public:
    Customer() {arrive = processtime = 0;}
    void set(long when);
    long when() const {return arrive;}
    int ptime() const {return processtime;}
};
typedef Customer Item;
class Queue
{
private:
    //class scope definitions
    //Node is a nested structure definition local to this
    struct Node {Item item;struct Node * next;};
    enum {Q_SIZE = 10;}
    //static Q_SIZE = 10;
    //private class members
    Node * front;//pointer to front of QUeues
    Node * rear;//to rear of queue
    int items;//current number of items in queue
    const int qsize;//maximum number of items in queue
    //preemptive definitios to prevent public copying
Queue(const Queue & q) : qsize(0){}
    Queue & operator=(const Queue & q) {return *this;}
public:
    Queue(int qs = Q_SIZE);
    ~Queue();
    bool isempty() const;
    bool isfull() const;
    int queueCount() const;
    bool enqueue(const Item &item);
    bool dequeue(Item &item);
};
#endif
