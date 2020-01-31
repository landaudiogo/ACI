#include "general.h"

int Send_Modbus_request(char *server_add, uint16_t port, uint8_t *apdu, uint8_t apdu_len, uint8_t **apdu_r) {
    uint8_t *pdu = (uint8_t *)calloc(apdu_len +7, sizeof(uint8_t));
    uint8_t *r_pdu = (uint8_t *)calloc(5+7, sizeof(uint8_t));

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
    //checking values
    printf("client sending message:\n");
    for(int i=0; i<apdu_len+7; i++) {
        if(i==7) printf("SDU: ");
        printf(" %x ", pdu[i]);
    }
    printf("\n"); 

    int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //creates a local socket
    
    struct sockaddr_in server; 
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_aton(server_add, &server.sin_addr);

    if(connect(s, (struct sockaddr *)&server, apdu_len+7) ==-1) {
        printf("unable to connect\n");
        return -1;
    }
    
    //sending message to server
    int len_send = send(s, pdu, apdu_len+7, 0);
    if(len_send != apdu_len+7) {
        if(len_send <0) printf("Error has occurred\n");
        else printf("sent %d bytes\n", len_send);
    }

    //receiving response
    int len_recv = recv(s, r_pdu, 5+7, 0);
    if(len_recv >= 0) printf("received %d octets\n", len_recv);
    else printf("Error has occurred\n");
    
    for(int i=0; i<len_recv; i++) {
        printf(" %x ", r_pdu[i]);
    }
    printf("\n");

    uint8_t *aux_apdu_r = (uint8_t *)calloc(5, sizeof(uint8_t));

    //Remove header
    for(int i=0; i<5; i++) {
        aux_apdu_r[i] = r_pdu[i+7];
    }
    *apdu_r = aux_apdu_r; 

    return 0;
    
}

uint16_t Receive_Modbus_request(int fd, uint8_t **apdu, uint8_t *apdu_len, struct sockaddr_in *remote, int *comm_socket) {
    uint8_t buffer[256];

    socklen_t remote_addlen = sizeof(remote);   

    *comm_socket = accept(fd, (struct sockaddr *)remote, &remote_addlen);

    int len_recv = recv(*comm_socket, buffer, 256, 0);
    printf("Received %d octets\n", len_recv);

    printf("Received message:\n");
    for(int i=0; i<len_recv; i++) {
        printf(" %x ", buffer[i]);
    }
    printf("\n");

    uint16_t transaction_identifier, protocol, n_bytes;
    uint8_t unit_id;
    
    octet_to_register(buffer[0], buffer[1], transaction_identifier);
    octet_to_register(buffer[2], buffer[3], protocol);
    octet_to_register(buffer[4], buffer[5], n_bytes);
    unit_id = buffer[6];
    
    printf("apdu length: %x\n", n_bytes-1);

    uint8_t *aux = (uint8_t *)calloc(n_bytes-1, sizeof(uint8_t));
    for(int i=0; i<n_bytes-1; i++) {
        aux[i] = buffer[i+7];
        printf("%d: %x\n", i, aux[i]);
    }
    printf("\n");

    *apdu_len = n_bytes-1;
    *apdu = aux;

    return transaction_identifier;
}

void Send_Modbus_response(uint16_t t_id, uint8_t *r_apdu, uint8_t r_apdu_len, struct sockaddr_in remote, int comm_socket) {
    uint8_t *pdu = (uint8_t*)calloc(r_apdu_len+7, sizeof(uint8_t));

    register_to_octet(pdu[0], pdu[1], t_id);
    register_to_octet(pdu[2], pdu[3], 0x0000);
    register_to_octet(pdu[4], pdu[5], r_apdu_len+1);
    pdu[6] = 0x00;

    for(int i=0; i<r_apdu_len; i++) {
        pdu[i+7] = r_apdu[i];
    }

    for(int i=0; i<5+7; i++) {
        printf(" %x ", pdu[i]);
    }
    printf("\n");

    int len_send = send(comm_socket, pdu, r_apdu_len+7, 0);
    if(len_send<0) printf("Error has occurred\n");
    else printf("sent %d bytes\n", len_send);
}


