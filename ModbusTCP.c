#include "general.h"

/****************************************************************************/
/****************************************************************************/
/***************************                 ********************************/
/***************************   CLIENT SIDE   ********************************/
/***************************                 ********************************/
/****************************************************************************/
/****************************************************************************/

int Send_Modbus_request(char *server_add, uint16_t port, uint8_t *apdu, uint8_t apdu_len, uint8_t **apdu_r) {
    //creating pdu arrays
    uint8_t *pdu = (uint8_t *)calloc(apdu_len+7, sizeof(uint8_t));
    uint8_t *r_pdu = (uint8_t *)calloc(5+7, sizeof(uint8_t));
    uint8_t buffer[256];

    //write pdu = MBAP + apdu
        //MBAP
    register_to_octet(pdu[0], pdu[1], 0x0001); // transaction identifier
    register_to_octet(pdu[2], pdu[3], 0x0000);
    register_to_octet(pdu[4], pdu[5], apdu_len +1);
    pdu[6] = 0x01;
        //SDU
    for(int i=0; i<apdu_len; i++) {
        pdu[i+7] = apdu[i];
    }
    //checking written values
    printf("client sending message:\n");
    for(int i=0; i<apdu_len+7; i++) {
        if(i==7) printf("SDU: ");
        printf(" %x ", pdu[i]);
    }
    printf("\n"); 

    //create a socket to communicate with server
    int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //creates a local socket
    if(s<0) printf("error creating socket\n");

    struct sockaddr_in server; 
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_aton(server_add, &server.sin_addr);

    if(connect(s, (struct sockaddr *)&server, sizeof(server)) == -1) {
        printf("unable to connect \ts: %d\tapdu_len: %d\n", s, apdu_len);
        return -1;
    }

    //sending message to server
    int len_send = send(s, pdu, apdu_len+7, 0);
    if(len_send != apdu_len+7) {
        if(len_send <0) printf("Error has occurred\n");
        else printf("sent %d bytes\n", len_send);
    }

    //receiving response
    int len_recv = recv(s, buffer, 256, 0);
    if(len_recv >= 0) printf("received %d octets\n", len_recv);
    else printf("Error has occurred\n");
    //printing received message
    for(int i=0; i<len_recv; i++) {
        printf(" %x ", buffer[i]);
    }
    printf("\n");

    uint8_t *aux_apdu_r = (uint8_t *)calloc(len_recv-7, sizeof(uint8_t));

    //Remove header
    for(int i=0; i<len_recv-7; i++) {
        aux_apdu_r[i] = buffer[i+7];
    }
    *apdu_r = aux_apdu_r; 

    return 0;
}




/****************************************************************************/
/****************************************************************************/
/***************************                 ********************************/
/***************************   SERVER SIDE   ********************************/
/***************************                 ********************************/
/****************************************************************************/
/****************************************************************************/


uint16_t Receive_Modbus_request(int fd, uint8_t **apdu, uint8_t *apdu_len, int *comm_socket) {
    
    //server buffer to read stream
    uint8_t buffer[256];

    //remote host
    struct sockaddr_in remote;
    socklen_t remote_addlen = sizeof(remote);   

    //communication socket used by server for the client
    *comm_socket = accept(fd, (struct sockaddr *)&remote, &remote_addlen);
    if(*comm_socket<0) printf("error creating socket to communicate between server and client\n");

    int len_recv = recv(*comm_socket, buffer, 256, 0);
    if(len_recv<0) {
        printf("error receiving message\n");
    }
    else {
        printf("Received %d octets\n", len_recv);
    }

    printf("Received message:\n");
    for(int i=0; i<len_recv; i++) {
        printf(" %x ", buffer[i]);
    }
    printf("\n");

    //writing relevant data from buffe 
    uint16_t transaction_identifier, protocol, n_bytes;
    uint8_t unit_id;
        //MBAP
    octet_to_register(buffer[0], buffer[1], transaction_identifier);
    octet_to_register(buffer[2], buffer[3], protocol);
    octet_to_register(buffer[4], buffer[5], n_bytes);
    unit_id = buffer[6];
        //APDU 
    uint8_t *aux = (uint8_t *)calloc(n_bytes-1, sizeof(uint8_t));
    for(int i=0; i<n_bytes-1; i++) {
        aux[i] = buffer[i+7];
    }
    printf("\n");

    *apdu_len = n_bytes-1;
    *apdu = aux;

    return transaction_identifier;
}

void Send_Modbus_response(uint16_t t_id, uint8_t *r_apdu, uint8_t r_apdu_len, int comm_socket) {
    //pdu to be sent back to client
    uint8_t *pdu = (uint8_t*)calloc(r_apdu_len+7, sizeof(uint8_t));
        //MBAP
    register_to_octet(pdu[0], pdu[1], t_id);
    register_to_octet(pdu[2], pdu[3], 0x0000);
    register_to_octet(pdu[4], pdu[5], r_apdu_len+1);
    pdu[6] = 0x00;
        //APDU
    for(int i=0; i<r_apdu_len; i++) {
        pdu[i+7] = r_apdu[i];
    }

    //sending message through comm_socket
    int len_send = send(comm_socket, pdu, r_apdu_len+7, 0);
    if(len_send<0) printf("Error has occurred\n");
    else printf("sent %d bytes\n", len_send);

    //Server response to request
    printf("Response to client request:");
    for(int i=0; i<5+7; i++) {
        printf(" %x ", pdu[i]);
    }
    printf("\n");
}


