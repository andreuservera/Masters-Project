#include "utils.h"
#include "parser.h"
#include <mysql/mysql.h>
#include <ctype.h>

/****************************** M A C R O S ******************************/

//#define SIZE_POINTER 8
//#define MAX_NUMBER_OF_PORTS 6
//#define MAX_NUMBER_OF_SWITCHES 8
//#define NUMBER_OF_LISTS 100


void InsertConfigInDB(switx **ptr_switches, int *ptr_count_switches, MYSQL* conn);
void Show(MYSQL* conn);
void Select_args(MYSQL *conn, char *buffer);
void ReadArgumentValue(char* arg, char* ptr, size_t bufferLength, char* dest);



int main(void)
{
    //***************************************
    //********** Parse config file **********
    //***************************************
//    int count_switches = 0;
//    int* ptr_count_switches = &count_switches;

//    switx **ptr_switches;
//    ptr_switches = malloc(sizeof(switx)*NUMBER_OF_SWITCHS);

//    ReadConfigFile(ptr_count_switches, ptr_switches);
//    printf("Number of Switches: %u\n", count_switches);

//    WriteXmlInstance(ptr_count_switches, ptr_switches);


    //***************************************
    //********** Manage user input **********
    //***************************************
    MYSQL* conn;


    char* server = "localhost";
    char* user = "default";
    char* passwd = "password";
    char* db = "mydata";


    while(1)
    {

        conn = mysql_init(NULL);
        // Connect to database
        if (!mysql_real_connect(conn, server, user, passwd, db, 0, NULL, 0))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
        }

        printf("> ");
        char buffer[256];

        if(fgets(buffer, sizeof buffer, stdin) != NULL)
        {

            buffer[strcspn(buffer, "\n")] = 0;

            if(strcmp(buffer, "store") == 0 || strcmp(buffer, "store-upload") == 0)
            {
                int count_switches = 0;
                int* ptr_count_switches = &count_switches;

                //array of pointers to structs.
                switx *t_switches[MAX_NUMBER_OF_SWITCHES];

                // allocate memory for each switch
                for(size_t i = 0; i < MAX_NUMBER_OF_SWITCHES; i++)
                {
                    t_switches[i] = malloc(sizeof(switx));

                    // allocate memory for each port in every switch
                    for(size_t j = 0; j < MAX_NUMBER_OF_PORTS; j++)
                    {
                        t_switches[i]->t_ports[j] = malloc(sizeof(port));
                    }
                }

                //pointer to array of pointers to structs.
                switx *(*p_switches)[] = &t_switches;


                ReadConfigFile(ptr_count_switches, p_switches);
                printf("Number of Switches: %u\n", count_switches);

                WriteXmlInstance(ptr_count_switches, ptr_switches);
                printf("debug1\n");
                InsertConfigInDB(ptr_switches, ptr_count_switches, conn);

                free(p_switches);
                free(t_switches);
            }

            if(strcmp(buffer, "show") == 0)
            {
                Show(conn);
            }
            
            if(strstr(buffer, "select ") != NULL)
            {
                Select_args(conn, buffer);
            }

        }

        memset(buffer, 0, strlen(buffer));
    }

    //free(ptr_switches);
}

void InsertConfigInDB(switx **ptr_switches, int *ptr_count_switches, MYSQL *conn)
{
    int list_number = 1;

    for(int i=0; i<*ptr_count_switches; i++)
    {
        char* myquery = malloc(sizeof(char)*200);

        myquery = malloc(sizeof(char)*200);
        sprintf(myquery, "select list_number, switch_number from info group by switch_number, list_number HAVING switch_number=%s order by list_number DESC"
                ,ptr_switches[i]->switx_number);
        //Query to find the last list_number
        if (mysql_query(conn, myquery))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
        }

        MYSQL_RES *query_results = mysql_store_result(conn);
        if(query_results==NULL)
        {
            fprintf(stderr, "No data to fetch");
            exit(1);
        }

        MYSQL_ROW row = mysql_fetch_row(query_results);
        if (row!=NULL)
        {
            printf("ASDFASDF: %s\n", row[0]);
            list_number = row[0] ? atoi(row[0]) : 0;
            list_number++;
        }
        else
        {
            list_number = 1;
            printf("[DATABASE] Row is null\n");
        }

        printf("[DATABASE] LIST NUMBER: %d\n", list_number);


        //*************************************************************
        memset(myquery, 0, strlen(myquery));

        printf("[SWITCH: %d] \n", i);
        printf("\tID: %s \n", ptr_switches[i]->switx_number);
        printf("\tNumber of Ports: %d \n", ptr_switches[i]->ports_quantity);
        for(int j = 0; j < ptr_switches[i]->ports_quantity; j++)
        {
            printf("\t[PORT: %d ] \n", ptr_switches[i]->ports[j]->port_number);
            printf("\t\tadmin_control_list_length: %d \n", ptr_switches[i]->ports[j]->admin_control_list_length);
            printf("\t\tadmin_control_list_length: %f \n", ptr_switches[i]->ports[j]->hypercycle);

            for(int k=0; k < ptr_switches[i]->ports[j]->admin_control_list_length; k++)
            {
                printf("\t\tPeriod: ");
                printf("%lu", ptr_switches[i]->ports[j]->period[k]);
                printf("\t\t\tgate_states: ");
                printf("%d \n", ptr_switches[i]->ports[j]->gates_state[k]);

                sprintf(myquery, "INSERT INTO info(switch_number,port_number,list_number,index_,period,gates_state) "
                                 "VALUES(%s, %d, %d, %d, %li, %d)"
                        ,ptr_switches[i]->switx_number
                        ,ptr_switches[i]->ports[j]->port_number
                        ,list_number
                        ,k
                        ,ptr_switches[i]->ports[j]->period[k]
                        ,ptr_switches[i]->ports[j]->gates_state[k]);

                if (mysql_query(conn, myquery))
                {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                    exit(1);
                }
            }
        }

        printf("[DATABASE] Configuration added in database\n");
        /*int last_port = ptr_switches[(*ptr_count_switches)-1]->ports_quantity;
        printf("debug1: %d\n", last_port);
        int debug = ptr_switches[(*ptr_count_switches)-1]->ports[last_port]->port_number;
        printf("debug2\n");
        int aux_acll = ptr_switches[(*ptr_count_switches)-1]->ports[last_port]->admin_control_list_length;
        printf("debug3\n");
        unsigned long aux_period = ptr_switches[(*ptr_count_switches)-1]->ports[last_port]->period[aux_acll];
        int aux_gates_state = ptr_switches[(*ptr_count_switches)-1]->ports[last_port]->gates_state[aux_acll];
        char* aux_switx_number = copyCharArray(ptr_switches[(*ptr_count_switches)-1]->switx_number, ptr_switches[(*ptr_count_switches)-1]->length_switx_number);
        int aux1 = atoi(aux_switx_number);
        int aux_port_number = (int)ptr_switches[(*ptr_count_switches)-1]->ports[last_port]->port_number;
        //printf("aux port number : %d\n",aux_port_number);
        //list_number = 1;
        printf("debug4\n");
        sprintf(myquery, "INSERT INTO info(switch_number,port_number,list_number,index_,period,gates_state) VALUES(%d, %d, %d, %d, %li, %d)",aux1, aux_port_number,list_number, aux_acll, aux_period, aux_gates_state);

        //printf("[DATABASE] Query\n");
        if (mysql_query(conn, myquery))
        {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
        }
        //printf("[DATABASE] Configuration added in database\n");
        */

    }
}

void Show(MYSQL *conn)
{
    char* myquery = malloc(sizeof(char)*200);
    myquery = "select distinct list_number, switch_number from info order by switch_number";

    if (mysql_query(conn, myquery))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    MYSQL_RES *res = mysql_use_result(conn);
    MYSQL_ROW row;

    // output table name
    printf("   List ID    | Switch number\n");
    printf("--------------|-------------\n");
    while ((row = mysql_fetch_row(res)) != NULL)
    printf("       %s           %s\n", row[0],row[1]);
}

void Select_args(MYSQL *conn, char *buffer)
{
    char input_switch[10] = "";
    char input_list[10] = "";
    //char* p_input_switch = &input_switch[0];

    //const char* pos = &buffer[0];
    //int length_counter = 0;

    char arg[1] = "s";
    char* p_arg = &arg[0];

    ReadArgumentValue(p_arg, &buffer[0], strlen(buffer), &input_switch[0]);

    *p_arg = 'l';
    ReadArgumentValue(p_arg, &buffer[0], strlen(buffer), &input_list[0]);

    printf("S: %s\n", input_switch);
    printf("L: %s\n", input_list);

    char* myquery = malloc(sizeof(char)*200);
    sprintf(myquery,"SELECT  port_number, index_, period, gates_state from info where switch_number = %s and list_number = %s order by port_number, index_ asc", input_switch, input_list);

    if (mysql_query(conn, myquery))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    MYSQL_RES *res = mysql_use_result(conn);
    MYSQL_ROW row;

    // output table SELECTname
    printf("Port number | Index |    Period    | Gates state \n");
    printf("------------|-------|--------------|-------------\n");
    while ((row = mysql_fetch_row(res)) != NULL)
    printf("    %s         %s        %s           %s \n", row[0],row[1],row[2], row[3]);

}

void ReadArgumentValue(char* arg, char* pos, size_t bufferLength, char* dest)
{
    memset(dest, 0, strlen(dest));

    char hyphen[1] = "-";
    char* p_hyphen = &hyphen[0];

    size_t length_counter = 0;

    int exit = 0;
    while(length_counter < bufferLength && exit == 0)
    {
        if(strncmp(pos, p_hyphen, 1) == 0)
        {
            pos++;
            if(strncmp(pos, arg, 1) == 0)
            {
                char* aux_pos = pos+2;

                while(isalpha(*aux_pos) || isdigit(*aux_pos))
                {
                    strncat(dest, aux_pos, 1);
                    aux_pos++;
                }
                exit = 1;
            }
        }
        pos++;
        length_counter++;
    }
}



