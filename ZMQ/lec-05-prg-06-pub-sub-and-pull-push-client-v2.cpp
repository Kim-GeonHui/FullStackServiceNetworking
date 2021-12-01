#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <io.h>
#include <iostream>
#include <Windows.h>

using namespace std;

int main(int argc, char** argv)
{
    void* context;
    void* publisher;
    void* subscriber;
    char* clientID;

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
    
    clientID = argv[1];
    srand(time(NULL));
    while (1) {
        zmq_poll(items, 2, 100);
        if (items[0].revents && ZMQ_POLLIN) {
            char string[20];
            zmq_recv(subscriber, string, 20, 0);
            cout << clientID << ": received status => " << string << endl;
        }
        else {
            int randNumber = rand() % 100 + 1; 

            char msg[20];
            if (randNumber < 10) {
                Sleep(1);
                sprintf(msg, "%s%s%s", "(", clientID, ":ON)");
                zmq_send(publisher, msg, sizeof(msg), 0);
                cout << clientID << ": send status - activated" << endl;
            }
            else if (randNumber > 90) {
                Sleep(1);
                sprintf(msg, "%s%s%s", "(", clientID, ":OFF)");
                zmq_send(publisher, msg, sizeof(msg), 0);
                cout << clientID << ": send status - deactivated" << endl;
            }
        }
    }
   
    zmq_close(publisher);
    zmq_close(subscriber);
    zmq_ctx_destroy(context);

    return 0;
}