#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>

using namespace std;

#define PORT 8228
#define BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == 0) {
        cerr << "socket failed" << endl;
        exit(1);
    }

    cout << "SERVER: Socket created" << endl;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        cerr << "setsockopt";
        exit(1);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        cerr << "bind failed";
        exit(1);
    }

    cout << "SERVER: Success binding" << endl;

    listen(server_fd, 1);
    cout << "SERVER: listening clients..." << endl;

    new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen);

    if (new_socket < 0) {
        cerr << "accept";
        exit(1);
    }
    cout << "SERVER: success accepting!" << endl;

    char buffer[BUFFER_SIZE];

    while (new_socket > 0) {
        cout << "Connected to the client #1" << endl;

        while (true) {
            if (new_socket < 0) {
                exit(1);
            }

            cout << "Client" << " => ";
            recv(new_socket, buffer, BUFFER_SIZE, 0);
            
            for(int i = 0; (i < 1024 && buffer[i] != '\0'); i++)
                buffer[i] = buffer[i] - 2;
            cout << buffer << endl;

            if (new_socket < 0) {
                exit(1);
            }

            cout << "You   " << " => ";
            cin.getline(buffer, BUFFER_SIZE);
            
            for(int i = 0; (i < 1024 && buffer[i] != '\0'); i++)
            buffer[i] = buffer[i] + 2;
            
            send(new_socket, buffer, BUFFER_SIZE, 0);

            if (new_socket < 0) {
                exit(1);
            }
        }
    }
    return 0;
}
