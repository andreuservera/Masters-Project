#ifndef PARSER_H
#define PARSER_H

#include "utils.h"

#define MAX_SWITCH_LENGTH 20
#define SIZE_POINTER 8
#define MAX_NUMBER_OF_PORTS 6
#define MAX_NUMBER_OF_SWITCHES 8
#define NUMBER_OF_LISTS 100

//Structure for a port
typedef struct {
    int port_number;
    int admin_control_list_length;
    float hypercycle;
    //Each port can have more than one period and gates_state
    unsigned long period[8];
    int gates_state[8];

}port;

//Structure for a single switch
typedef struct {
    char switx_number[MAX_SWITCH_LENGTH];
    //int length_switx_number;
    struct port *t_ports[MAX_NUMBER_OF_PORTS];
    //struct port *(*p_ports)[] = &t_ports;
    int ports_quantity;

}switx;

void writePort(FILE * file_pointer, int port_number, int admin_control_list_length);
void writeList(FILE * file_pointer, unsigned long *period, int *gates_state, float hypercycle, int admin_control_list_length);
void ReadConfigFile(int* p_count_switches, switx *(*p_switches)[]);
//void ReadConfigFileOld();
void WriteXmlInstance(int* p_count_switches, switx **p_switches);

#endif //PARSER_H
