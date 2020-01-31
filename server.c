#include "general.h"
#include "ModbusAP.h"

int main() {
    uint16_t registers[1000];
    uint16_t *val=NULL, st, n, transaction_identifier;
    uint8_t op;

    int fd, comm_socket;
    struct sockaddr_in remote;
    socklen_t remote_addlen = sizeof(remote);

    //server listening socket
    if(server_connect(&fd) ==-1) {
        printf("error creating conneciton\n");
    }
    else {
        printf("successfully connected\n");
    }

    while(1) {
        //hold for client connection and read what is in stream
        transaction_identifier = Get_request(fd, &op, &n, &st, &val, &remote, &comm_socket);

        //handle functions
        if(op == 0x10) {
            printf("Writing registers\n");
            for(int i=0; i<n; i++) {
            registers[st+i] = val[i];
            }
            free(val);
            val=NULL;
            Send_response(transaction_identifier, op, st, n, val, remote, comm_socket);
        }
        if(op == 0x03) {
            for(int i=0; i<n; i++) {
                val[i] = registers[st+i];
            }
            Send_response(transaction_identifier, op, st, n, val, remote, comm_socket);
        }

        //close socket used for reception and transmission
        if(shutdown(comm_socket, SHUT_RDWR) == -1 ) printf("error with socket shutdown\n");
    }

    //close server listening socket
    if(shutdown(fd, SHUT_RDWR) == -1 ) printf("error with socket shutdown\n");
    

    return 0;
}