#include "general.h"
#include "ModbusAP.h"

int main() {
    uint16_t registers[1000];
    uint16_t *val=NULL, st, n, transaction_identifier;
    uint8_t op;


    int fd, comm_socket;
    struct sockaddr_in remote;
    socklen_t remote_addlen = sizeof(remote);
    server_connect(&fd);
        
    transaction_identifier = Get_request(fd, &op, &n, &st, &val, &remote, &comm_socket);
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
    
    
    

    server_close(&fd, &comm_socket);

    return 0;
}