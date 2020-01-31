#ifndef ModbusTCP_h
#define ModbusTCP_h

#include "general.h"

void Send_Modbus_request(char *server_add, uint16_t port, uint8_t *apdu, uint8_t apdu_len, uint8_t **apdu_r);

uint16_t Receive_Modbus_request(int fd, uint8_t **apdu, uint8_t *apdu_len, struct sockaddr_in *remote, int *comm_socket);

void Send_Modbus_response(uint16_t t_id, uint8_t *r_apdu, uint8_t r_apdu_len, struct sockaddr_in remote, int comm_socket);

#endif
