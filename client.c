#include "general.h"
#include "ModbusAP.h"

void input_values_stream(uint16_t *input_registers, int n_regs) {
  for(int i=0; i<n_regs; i++) {
    printf("Register %d value: ", i);
    scanf("%" SCNu16, &input_registers[i]);
  }
}

int main() {
  uint8_t function_input, user_n_reg, user_n_coils;
  uint16_t *input_registers, st_address;

  printf("1 - Write Registers\n2 - Read Registers\n3 - Write Coils\n4 - Read Coils\n");
  printf("Select one of the functions mentioned above: ");
  scanf("%" SCNu8, &function_input);

  printf("What is the desired starting addresss? ");
  scanf("%"SCNu16, &st_address);

  if(function_input == 1) {
    printf("How many registers would you like to Write? ");
    scanf("%" SCNu8, &user_n_reg);
    uint8_t *input_octets;

    input_registers = (uint16_t*)calloc( ((int)user_n_reg), sizeof(uint16_t) );
    input_values_stream(input_registers, user_n_reg);

    Write_multiple_regs(server_ip, SERVER_PORT, st_address, user_n_reg, input_registers);
  }
  else if(function_input == 2) {
    printf("How many registers would you like to Read? ");
    scanf("%" SCNu8, &user_n_reg);
  }
  else if(function_input == 3) {
    printf("How many coils would you like to Write? ");
    scanf("%" SCNu8, &user_n_coils);
  }
  else if(function_input == 4) {
    printf("How many coils would you like to Read? ");
    scanf("%" SCNu8, &user_n_coils);
  }
}
