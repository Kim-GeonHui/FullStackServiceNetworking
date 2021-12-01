#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <io.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    void* context;
    void* socket;
    int req_cnt;
    int ret;

    cout << "Connecting to hello world server.." << endl;

    context = zmq_ctx_new();
    socket = zmq_socket(context, ZMQ_REQ);
    zmq_connect(socket, "tcp://localhost:5555");

    for (req_cnt = 0; req_cnt != 10; req_cnt++) {
        char buffer[100];

        cout << "sending request " << req_cnt << " ..." << endl;
        zmq_send(socket, "Hello", 5, 0);

        memset(buffer, 0x00, sizeof(buffer));
        ret = zmq_recv(socket, buffer, sizeof(buffer) - 1, 0);
        cout << "Received reply " << req_cnt << " [ " << buffer << " ]" << endl;
    }

    zmq_close(socket);
    zmq_ctx_destroy(context);

    return 0;
}