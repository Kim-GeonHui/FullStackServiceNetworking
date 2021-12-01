#include <zmq.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <iostream>
#include <windows.h>

using namespace std;

int main()
{
    // Socket to talk to clients
    void* context;
    void* socket;
    int ret;

    context = zmq_ctx_new();
    socket = zmq_socket(context, ZMQ_REP);
    ret = zmq_bind(socket, "tcp://*:5555");

    while (1) {
        char buffer[10];

        memset(buffer, 0x00, sizeof(buffer));
        zmq_recv(socket, buffer, sizeof(buffer) - 1, 0);
        cout << "Received request: " << buffer << endl;

        Sleep(1);

        zmq_send(socket, "World", 5, 0);
    }

    return 0;
}