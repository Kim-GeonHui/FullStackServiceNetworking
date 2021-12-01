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
    int ret;

    cout << "Collecting updates from weather server..." << endl;

    context = zmq_ctx_new();
    socket = zmq_socket(context, ZMQ_SUB);
    zmq_connect(socket, "tcp://localhost:5556");

    char* zip_filter = (argc > 1) ? argv[1] : "10001";
    ret = zmq_setsockopt(socket, ZMQ_SUBSCRIBE, zip_filter, strlen(zip_filter));

    int total_temp = 0;
    int update_number;
    for (update_number = 0; update_number < 100; update_number++) {
        char string[20];
        zmq_recv(socket, string, 20, 0);

        int zipcode, temperature, relhumidity;
        sscanf(string, "%d %d %d", &zipcode, &temperature, &relhumidity);
        total_temp += temperature;

        cout << "Receive temperature for zip code '" << zip_filter << "' was " << temperature << " F" << endl;
    }
    cout << "Average temperature for zipcode is '" << zip_filter << "' was " << total_temp/update_number << " F" << endl;

    zmq_close(socket);
    zmq_ctx_destroy(context);

    return 0;
}