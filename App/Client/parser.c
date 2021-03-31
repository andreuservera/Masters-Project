#include "parser.h"
#include "utils.h"
#include <stdio.h>

#define END_FILE 0
#define STR_SWITCH "switch:"
#define STR_SWITCH_LENGTH 7
#define STR_PORT "port_number:"
#define STR_PORT_LENGTH 12
#define SWITCH_ID_LENGTH 10

//TONI
#define NUMBER_OF_SWITCHS 8
#define NUMBER_OF_PORTS 6
#define NUMBER_OF_LISTS 100


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
        fprintf(file_pointer, "\t\t</sched:max-sdu-table>\n");
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
    denominator = 500.0f/hypercycle;
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



/**************************************************************************/
/** @name ReadFile  *******************************************************/
/**************************************************************************/
void ReadConfigFile(int* ptr_count_switches, switx *(*p_switches)[])
{
    //*****************+****** VARIABLES **********************

    //Variables for getting a word
    char string[150];                   //Max length of a word
    char *word = string;                 //Contains the last read word
    size_t  length_word = 1;                   //Length of last read word
    char *aux_word;                     //Used to copy the read words in other addresses



    //switx *ptr_switch;                  //Pointer to a switch
    //port *ptr_port;                    //Pointer a port

    unsigned long aux_hypercycle;


    int last_port;
    int aux_acll;
    int block = 1;

    //********* START READING THE FILE*********

    FILE * fpointer = fopen("../mytext.txt","r");

    while(1)
    {
        getWord(fpointer, word, &length_word, 0);

        if(length_word == END_FILE)
        {
            break;
        }


        //******* GET A SWITCH *************
        if(strcmp(word, "switch:") == 0)
        {
            (*ptr_count_switches)++;

            getWord(fpointer, word, &length_word, 0);
            //aux_word = copyCharArray(word,length_word);


//            switx* ptr_switch = malloc(sizeof(switx));
//            if(ptr_switch == NULL)
//            {
//                printf("Couldn't allocate memory.\n");
//                exit(1);
//            }

//            ptr_switch->switx_number = malloc(sizeof(char) * (length_word+1));
//            if(ptr_switch->switx_number == NULL)
//            {
//                printf("Couldn't allocate memory.\n");
//                exit(1);
//            }

            //ptr_switch->switx_number = aux_word;
            //memcpy(ptr_switch->switx_number, word, length_word); - before
            memcpy((*p_switches)[*ptr_count_switches]->switx_number, word, length_word);

            //ptr_switch->length_switx_number = (int)length_word; - before
            //(*p_switches)[*ptr_count_switches]->length_switx_number;

            //ptr_switch->ports_quantity = 0; - before
            (*p_switches)[*ptr_count_switches]->ports_quantity = 0;
            //ptr_switches[(*ptr_count_switches)-1] = ptr_switch; -before

            //Debug
//            for(int i = 0; i < *ptr_count_switches; i++)
//            {
//                printf("Switch id: %s\n", ptr_switches[i]->switx_number);
//            }


            //ptr_switches[*ptr_count_switches-1]->ports = malloc(sizeof(port)*NUMBER_OF_PORTS); - before

            //free(ptr_switch->switx_number); - before
            //free(ptr_switch); - before
            block = 1;

        }

        //******* GET A PORT **********
        if(strcmp(word, "port_number:") == 0)
        {
            // explicit pointer to array of pointers to structs.
            // aka, pointer to the array of pointers to port structs from the current switch
            port *(*p_ports)[] = &(*p_switches)[ptr_count_switches]->t_ports;

            getWord(fpointer, word, &length_word, 0);

            //port* ptr_port = malloc(sizeof(port)); - before
            //aux_word = copyCharArray(word, length_word); - before
            //ptr_port->port_number = atoi(aux_word); - before
            (*p_ports)[(*p_switches)[*ptr_count_switches]->ports_quantity]->port_number = atoi(word);

            //ptr_port->admin_control_list_length = 0; - before
            (*p_ports)[(*p_switches)[*ptr_count_switches]->ports_quantity]->admin_control_list_length = 0;

            //aux_hypercycle = 0; - before
            aux_hypercycle = 0;


            // before
            /*last_port = ptr_switches[*ptr_count_switches-1]->ports_quantity;
            ptr_switches[*ptr_count_switches-1]->ports[last_port] = ptr_port;
            ptr_switches[*ptr_count_switches-1]->ports_quantity++;                                          //Update the number of ports in the switch
            ptr_switches[*ptr_count_switches-1]->ports[last_port]->period = malloc(sizeof(unsigned long)*NUMBER_OF_LISTS);        //Allocate memory for the lists of the port
            ptr_switches[*ptr_count_switches-1]->ports[last_port]->gates_state = malloc(sizeof(unsigned long)*NUMBER_OF_LISTS);*/

            (*p_switches)[*ptr_count_switches]->ports_quantity++;

            //free(ptr_port); - before
            block = 0;                                                                                    //If the port have been read then we can proceed  to read the configuration
            getWord(fpointer, word, &length_word, 0);                                                     //Read the period
        }


        if(block == 0)
        {
        //printf("[INFO] ---- NEW LIST----\n");
            //aux_word = copyCharArray(word, length_word); - before
            //aux_acll = ptr_switches[*ptr_count_switches-1]->ports[last_port]->admin_control_list_length; - before
            //ptr_switches[*ptr_count_switches-1]->ports[last_port]->period[aux_acll] = (unsigned long)atol(aux_word); - before

            aux_hypercycle = aux_hypercycle + (unsigned long)atol(aux_word);                                                       //Calculate the new hypercycle
            ptr_switches[*ptr_count_switches-1]->ports[last_port]->hypercycle = (float)aux_hypercycle/1000000000;     //Convert the hypercyle to seconds and store it

            getWord(fpointer, word, &length_word, 0);                                                               //Get the gates state
            ptr_switches[*ptr_count_switches-1]->ports[last_port]->gates_state[aux_acll] = BinCharToInt(word, (int)length_word); //Store the gates state
            ptr_switches[*ptr_count_switches-1]->ports[last_port]->admin_control_list_length++;                       //Update the length of the list



            //printf("[INFO] Admin control list length: %d \n",ptr_switches[switches_quantity-1]->ports[last_port]->admin_control_list_length);
            //printf("[INFO] Hypercycle: %2f \n",ptr_switches[switches_quantity-1]->ports[last_port]->hypercycle);
            //printf("[INFO] Period: %li \n",ptr_switches[switches_quantity-1]->ports[last_port]->period[aux_acll]);
            //printf("[INFO] Gates state: %d \n",ptr_switches[switches_quantity-1]->ports[last_port]->gates_state[aux_acll]);
        }

    }
//    for(int i=0; i < *ptr_count_switches; i++)
//    {
//        printf("Switch id: %s \t ports: %d\n", ptr_switches[i]->switx_number, ptr_switches[i]->ports_quantity);
//    }
    //free(ptr_switch);
    fclose(fpointer);
    printf("Configuration file parsed correctly...\n");
}

/**************************************************************************/
/** @name WriteXmlInstance  ***********************************************/
/**************************************************************************/
void WriteXmlInstance(int *ptr_count_switches, switx **ptr_switches)
{

    //char s64[] = "64";
    //char s65[] = "65";
    char name_file[13];

    for (int k = 0; k < *ptr_count_switches; k++)
    {
        sprintf(name_file,"XMLdata%s.xml",ptr_switches[k]->switx_number);
        FILE * fpointer1 = fopen(name_file,"w");
        fprintf(fpointer1,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>>\n");
        fprintf(fpointer1,"<if:interfaces xmlns:if=\"urn:ietf:params:xml:ns:yang:ietf-interfaces\">\n");

        for(int i = 0; i < ptr_switches[k]->ports_quantity; i++)
        {
            writePort(fpointer1, ptr_switches[k]->ports[i]->port_number, ptr_switches[k]->ports[i]->admin_control_list_length);
            writeList(fpointer1, ptr_switches[k]->ports[i]->period, ptr_switches[k]->ports[i]->gates_state, ptr_switches[k]->ports[i]->hypercycle, ptr_switches[k]->ports[i]->admin_control_list_length);
        }
        fprintf(fpointer1,"</if:interfaces>\n");
        fclose(fpointer1);
            }
    printf("XML instance generated correctly...\n");
}

