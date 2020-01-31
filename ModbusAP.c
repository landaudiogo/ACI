#include "ModbusTCP.h"
#include "general.h"

void Write_multiple_regs(char *server_add, uint16_t port, uint16_t st_r, uint16_t n_r, uint16_t *val) {
    uint8_t *apdu = (uint8_t *)calloc(n_r*2 + 7, sizeof(uint8_t));
    uint8_t *response_apdu =NULL; //funtoin code(1) + starting address(2) + registers written(2) || Error code(1) + Exception code (1)

    //Write Header
    apdu[0] = 0x10; //function code
    register_to_octet(apdu[1], apdu[2], st_r); //starting address
    register_to_octet(apdu[3], apdu[4], n_r); //number of registers
    apdu[5] = n_r*2; //number of bytes

    //Write Data
    printf("These are the values to be written:\n");
    for(int i=0; i<n_r; i++) {
      register_to_octet(apdu[i*2+6], apdu[i*2+7], val[i]);
      printf("val: %x\th_r: %x\tl_r: %x\n", val[i], apdu[i*2+6], apdu[i*2+7]);
    }

    Send_Modbus_request(server_ip, SERVER_PORT, apdu, n_r*2+6, &response_apdu);
    for(int i=0; i<5; i++){
        printf(" %x ", response_apdu[i]);
    }
    printf("\n");
}

void Read_multiple_regs(char *server_add, uint16_t port, uint16_t st_r, uint16_t n_r, uint16_t *val) {

}

uint16_t Get_request(int fd, uint8_t *op, uint16_t *n, uint16_t *st, uint16_t **val, struct sockaddr_in *remote, int *comm_socket) {
    uint8_t *apdu;
    uint8_t apdu_len;

    uint16_t transaction_identifier = Receive_Modbus_request(fd, &apdu, &apdu_len, remote, comm_socket);

    printf("Message received:\n");
    for(int i=0; i<apdu_len; i++) {
        printf(" %x ", apdu[i]);
    }
    printf("\n");

    *op = apdu[0];
    octet_to_register(apdu[1], apdu[2], *st);
    octet_to_register(apdu[3], apdu[4], *n)

    if(*op == 0x10) {
        uint16_t *aux_val = calloc(*n, sizeof(uint16_t));
        for(int i=0; i<*n; i++) {
            octet_to_register(apdu[i*2+6], apdu[i*2+1+6], aux_val[i]); 
        }

        *val = aux_val;
    }
    return transaction_identifier;
}

void Send_response(uint16_t t_id, uint8_t op, uint16_t st, uint16_t n, uint16_t *val, struct sockaddr_in remote, int comm_socket) {
    if(val==NULL) {
        if(op == 0x10) {
            printf("Enterred\n");
            uint8_t *apdu = (uint8_t *)calloc(5, sizeof(uint8_t));
            apdu[0] = op;
            register_to_octet(apdu[1], apdu[2], st);
            register_to_octet(apdu[3], apdu[4], n);
            for(int i=0; i<5; i++){
                printf(" %x ", apdu[i]);
            }
            printf("\n");

            Send_Modbus_response(t_id, apdu, 5, remote, comm_socket);
        }
    }
    else {
        printf("this side\n");
    }
}

int server_connect(int *fd) {
    *fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //creates a local socket
    if(*fd < 0) {
        printf("failed to create socket\n");
    }
    
    struct sockaddr_in local;
    socklen_t local_addlen = sizeof(local);
    local.sin_family = AF_INET;
    local.sin_port = htons(SERVER_PORT);
    inet_aton(server_ip, &local.sin_addr);

    if(bind(*fd, (struct sockaddr *)&local, local_addlen) == -1) {
        printf("failed to bind\n");
        return -1;
    }
    
    if (listen(*fd, 10) == -1) {
        printf("error listening\n");
        return -1;
    }

    return 0; //successful connection
}
