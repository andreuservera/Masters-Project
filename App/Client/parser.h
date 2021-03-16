#ifndef PARSER_H
#define PARSER_H

#include "utils.h"
#include <string.h>

//Structure for a port
typedef struct {
    int port_number;
    int admin_control_list_length;
    float hypercycle;
    //Each port can have more than one period and gates_state
    unsigned long *period;
    int *gates_state;

}port;

//Structure for a single switch
typedef struct {
    char *switx_number;           //String
    int length_switx_number;      //Length of the switch number
    port **ports;                 // Ports of the switch
    int ports_quantity;

}switx;

void writePort(FILE * file_pointer, int port_number, int admin_control_list_length);
void writeList(FILE * file_pointer, unsigned long *period, int *gates_state, float hypercycle, int admin_control_list_length);
void ReadConfigFile(int* p_count_switches, switx **p_switches);
//void ReadConfigFileOld();
void WriteXmlInstance(int* p_count_switches, switx **p_switches);

#endif //PARSER_H
