#include <zmq.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <iostream>
#include <windows.h>
#include <cstdlib>

using namespace std;

int main()
{
    void* context;
    void* publisher;
    void* collector;

    context = zmq_ctx_new();
    publisher = zmq_socket(context, ZMQ_PUB);
    zmq_bind(publisher, "tcp://*:5557");
    collector = zmq_socket(context, ZMQ_PULL);
    zmq_bind(collector, "tcp://*:5558");

    while (1) {
        char update[10];
        zmq_recv(collector, update, 10, 0);
        cout << "I: publishing update " << update << endl;
        zmq_send(publisher, update, 10, 0);
    }

    zmq_close(publisher);
    zmq_close(collector);
    zmq_ctx_destroy(context);

    return 0;
}