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
    // Socket to talk to clients
    void* context;
    void* socket;
    int ret;
    int zipcode;
    int temperature;
    int relhumidity;

    cout << "Publishing updates at weather server..." << endl;

    context = zmq_ctx_new();
    socket = zmq_socket(context, ZMQ_PUB);
    ret = zmq_bind(socket, "tcp://*:5556");

    while (1) {
        zipcode = (rand() % 1000000) + 1;
        temperature = (rand() % 215) - 80;
        relhumidity = (rand() % 50) + 10;

        char update[20];
        sprintf(update, "%05d %d %d", zipcode, temperature, relhumidity);
        zmq_send(socket, update, 20, 0);
    }

    zmq_close(socket);
    zmq_ctx_destroy(context);

    return 0;
}