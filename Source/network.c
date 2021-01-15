#include "network.h"

static char *ft_strdup(const char *s1) { // TODO
    char *str;
    size_t size = strlen(s1) + 1;

    str = malloc(size);
    if (str) {
        memcpy(str, s1, size);
    }
    return str;
}

void* context = NULL;
void* reqSocket = NULL;
char* serverAddress = {"tcp://localhost:5555"};

void initNetowrkContext() {
    if (context != NULL || reqSocket != NULL)
        return;
    context = zmq_ctx_new();
    reqSocket = zmq_socket (context, ZMQ_REQ);
    int val_zero = 0;
    zmq_setsockopt(reqSocket, ZMQ_LINGER, &val_zero, sizeof(val_zero));
    zmq_connect (reqSocket, serverAddress);
}

void closeNetwork() {
    zmq_close (reqSocket);
    zmq_ctx_destroy (context);
}

char* getCurrentServerAddres() {
    return ft_strdup(serverAddress);
}


bool checkServerConnection() {
    enum {cap = 32};
    char *msg = {"Baatle-client."};
    zmq_send (reqSocket, msg, strlen(msg), 0);


    char buffer [cap];
    int size = 0;

    int timeout = 1000;
    int milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
    int end = milliseconds_since + timeout;
    do {
        milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
        size = zmq_recv (reqSocket, buffer, 31, ZMQ_DONTWAIT);
        if (size == -1) {
            if (zmq_errno() == EAGAIN)
                continue;
            else 
                return false;
        } else 
            break;
    } while (milliseconds_since <= end);

    if (size == -1)
        return false;

    return true;
}
