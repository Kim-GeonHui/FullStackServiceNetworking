#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <io.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    void* context;
    void* publisher;
    void* subscriber;

    context = zmq_ctx_new();
    subscriber = zmq_socket(context, ZMQ_SUB);
    zmq_connect(subscriber, "tcp://localhost:5557");
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);
    publisher = zmq_socket(context, ZMQ_PUSH);
    zmq_connect(publisher, "tcp://localhost:5558");
    
    zmq_pollitem_t items[2];

    items[0].socket = subscriber;
    items[0].fd = 0;
    items[0].events = ZMQ_POLLIN;
    items[0].revents = 0;

    items[1].socket = publisher;
    items[1].fd = 0;
    items[1].events = ZMQ_POLLIN;
    items[1].revents = 0;
    
    srand(time(NULL));
    while (1) {
        zmq_poll(items, 2, 100);
        if (items[0].revents && ZMQ_POLLIN) {
            char string[10];
            zmq_recv(subscriber, string, 10, 0);
            cout << "I: received massage " << string << endl;
        }
        else {
            int randNumber = rand() % 100 + 1;
            char charNumber[10];
            sprintf(charNumber, "%d", randNumber);

            if (randNumber < 10) {
                zmq_send(publisher, charNumber, sizeof(charNumber), 0);
                cout << "I: sending message " << randNumber << endl;
            }
        }
    }
   
    zmq_close(publisher);
    zmq_close(subscriber);
    zmq_ctx_destroy(context);

    return 0;
}