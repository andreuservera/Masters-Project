#ifndef PARSER_H
#define PARSER_H

#include "utils.h"

void writePort(FILE * file_pointer, int port_number, int admin_control_list_length);
void writeList(FILE * file_pointer, unsigned long *period, int *gates_state, float hypercycle, int admin_control_list_length);
void ReadConfigFile();

#endif //PARSER_H
