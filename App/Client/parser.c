#include <string.h>
#include <mysql/mysql.h>
#include "parser.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define END_FILE 0
#define STR_SWITCH "switch:"
#define STR_SWITCH_LENGTH 7
#define STR_PORT "port_number:"
#define STR_PORT_LENGTH 12
#define SWITCH_ID_LENGTH 10

/**************************************************************************/
/** @name writePort ****************************************************/
/**************************************************************************/
void writePort(FILE * file_pointer, int port_number, int admin_control_list_length)
{
    fprintf(file_pointer, "\t<if:interface>\n");
    fprintf(file_pointer, "\t\t<if:name>%d</if:name>\n",port_number);
    fprintf(file_pointer, "\t\t<if:type xmlns:iftype=\"urn:ietf:params:xml:ns:yang:iana-if-type\">iftype:ethernetCsmacd</if:type>\n");
    for (int i=0; i<8; i++){
        fprintf(file_pointer, "\t\t<sched:max-sdu-table xmlns:sched=\"urn:ieee:std:802.1Q:yang:ieee802-dot1q-sched\">\n");
        fprintf(file_pointer, "\t\t\t<sched:traffic-class>%d</sched:traffic-class>\n", i);
        fprintf(file_pointer, "\t\t\t<sched:queue-max-sdu>0</sched:queue-max-sdu>\n");
        fprintf(file_pointer, "\t\t</sched:mysql.h: No such file or directorymax-sdu-table>\n");
    }
    fprintf(file_pointer, "\t\t<sched:gate-parameters xmlns:sched=\"urn:ieee:std:802.1Q:yang:ieee802-dot1q-sched\">\n");
    fprintf(file_pointer, "\t\t\t<sched:gate-enabled>true</sched:gate-enabled>\n");
    fprintf(file_pointer, "\t\t\t<sched:admin-gate-states>0</sched:admin-gate-states>\n");	//0..7
    fprintf(file_pointer, "\t\t\t<sched:admin-control-list-length>%d</sched:admin-control-list-length>\n", admin_control_list_length);

    return;
}



/**************************************************************************/
/** @name writeList ****************************************************/
/**************************************************************************/
void writeList(FILE * file_pointer, unsigned long *period, int *gates_state, float hypercycle, int admin_control_list_length)
{
    int index;
    float denominator;

    for( index = 0; index < admin_control_list_length; index++)
    {
        fprintf(file_pointer, "\t\t\t<sched:admin-control-list>\n");
        fprintf(file_pointer, "\t\t\t\t<sched:index>%d</sched:index>\n",index);
        fprintf(file_pointer, "\t\t\t\t<sched:operation-name>sched:set-gate-states</sched:operation-name>\n");
        fprintf(file_pointer, "\t\t\t\t<sched:sgs-params>\n");
        fprintf(file_pointer, "\t\t\t\t\t<sched:gate-states-value>%d</sched:gate-states-value>\n", gates_state[index]); // 0..7 TODO
        fprintf(file_pointer, "\t\t\t\t\t<sched:time-interval-value>%ld</sched:time-interval-value>\n", period[index]);
        fprintf(file_pointer, "\t\t\t\t</sched:sgs-params>\n");
        fprintf(file_pointer, "\t\t\t</sched:admin-control-list>\n");
    }
    fprintf(file_pointer, "\t\t\t<sched:admin-cycle-time>\n");
    fprintf(file_pointer, "\t\t\t\t<sched:numerator>500</sched:numerator>\n");
    denominator = 500.0/hypercycle;
    fprintf(file_pointer, "\t\t\t\t<sched:denominator>%d</sched:denominator>\n", (int)denominator);
    fprintf(file_pointer, "\t\t\t</sched:admin-cycle-time>\n");
    fprintf(file_pointer, "\t\t\t<sched:admin-base-time>\n");
    fprintf(file_pointer, "\t\t\t\t<sched:seconds>0</sched:seconds>\n");
    fprintf(file_pointer, "\t\t\t\t<sched:fractional-seconds>0</sched:fractional-seconds>\n");
    fprintf(file_pointer, "\t\t\t</sched:admin-base-time>\n");
    fprintf(file_pointer, "\t\t\t<sched:config-change>true</sched:config-change>\n");
    fprintf(file_pointer, "\t\t</sched:gate-parameters>\n");
    fprintf(file_pointer, "\t</if:interface>\n");

    return;
}

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
    char switx_id[SWITCH_ID_LENGTH];            //String
    int length_switx_number;                    //Length of the switch number
    port **ports;                               // Ports of the switch
    int ports_quantity;

}switx;

/**************************************************************************/
/** @name ReadFile  *******************************************************/
/**************************************************************************/
void ReadConfigFile()
{
    FILE * fpointer = fopen ("mytext.txt", "r");

    char buffer[150];
    int word_length = 1;           //Length of last read word
    char *ptr_buffer = buffer;

    switx** ptr_switches;

    int numOfSwitches = 0;

    while(word_length != END_FILE)
    {
        getWord(fpointer, ptr_buffer, &word_length, 0);
        printf("%s\n",ptr_buffer);

        int exit_switch = 0;

        //****** PARSE SWITCH ******
        if (strcmp(ptr_buffer, STR_SWITCH) == 0)
        {
            numOfSwitches++;
            switx* ptr_current_switch = (switx*)malloc(sizeof(switx));

            memset(buffer, 0, sizeof buffer);
            getWord(fpointer, ptr_buffer, &word_length, 0);
            if(word_length>10)
            {
                fprintf(stderr,"Switch_id is too long!");
                exit(1);
            }
            snprintf(ptr_current_switch->switx_id, SWITCH_ID_LENGTH, "%s", ptr_buffer);
            printf("switch id: %s\n", ptr_current_switch->switx_id);

        }
        //****** PARSE PORT ******
        else if (strcmp(ptr_buffer, STR_PORT))
        {

        }

        memset(buffer, 0, sizeof buffer);
        getWord(fpointer, ptr_buffer, &word_length, 0);

        /*if(compareWords(ptr_buffer, word_length, STR_PORT, STR_PORT_LENGTH, 0))
        {

        }

        }*/


        memset(buffer, 0, sizeof buffer);
    }
}


