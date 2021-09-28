// A C program to demonstrate linked list based implementation of queue
#include <stdio.h>
#include <stdlib.h>

// A linked list (LL) node to store a queue entry
struct QNode {
    unsigned long ccBuf;
    void* pvBuf;
    struct QNode* next;
};

// The queue, front stores the front node of LL and rear stores the
// last node of LL
struct Queue {
    struct QNode* front, * rear;
};

// A utility function to create a new linked list node.
struct QNode* NewNode(unsigned long s, void* p) {
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode));
    temp->ccBuf = s;
    temp->pvBuf = p;
    temp->next = NULL;
    return temp;
}

// A utility function to create an empty queue
struct Queue* CreateQueue() {
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

// The function to add a key k to q
void Enqueue(struct Queue* q, unsigned long s, void* p)
{
    // Create a new LL node
    struct QNode* temp = NewNode(s, p);

    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }

    // Add the new node at the end of queue and change rear
    q->rear->next = temp;
    q->rear = temp;
}

// Function to remove a key from given queue q
void Dequeue(struct Queue* q)
{
    // If queue is empty, return NULL.
    if (q->front == NULL)
        return;

    // Store previous front and move front one node ahead
    struct QNode* temp = q->front;

    q->front = q->front->next;

    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL)
        q->rear = NULL;

    free(temp);
}

int main()
{
    struct Queue* q = CreateQueue();
    
    for (int i = 0; i < 10; i++)
    {
        unsigned long ccBuf = 512;
        unsigned char* pbBuf = malloc(ccBuf);
        Enqueue(q, ccBuf, (void*)pbBuf);
    }

    for (int i = 0; i < 10; i++)
    {
        free(q->front->pvBuf);
        Dequeue(q);
    }
}
