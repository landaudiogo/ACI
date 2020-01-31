#ifndef ModbusAP_h
#define ModbusAP_h

void Write_multiple_regs(char *server_add, int port, uint16_t st_r, uint16_t n_r, uint16_t *val);

void Read_multiple_regs(char *server_add, int port, uint16_t st_r, uint16_t n_r, uint16_t *val);

uint16_t Get_request(int fd, uint8_t *op, uint16_t *n, uint16_t *st, uint16_t **val, struct sockaddr_in *remote, int *comm_socket);

void Send_response(uint16_t t_id, uint8_t op, uint16_t st, uint16_t n, uint16_t *val, struct sockaddr_in remote, int comm_socket);

int server_close(int *fd, int *comm_socket);

void server_connect(int *fd);

#endif
