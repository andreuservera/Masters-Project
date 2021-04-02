#include "utils.h"
#include "parser.h"
#include "switch.h"
#include "json.h"
#include <mysql/mysql.h>
#include <ctype.h>

/****************************** M A C R O S ******************************/

#define SIZE_POINTER 8
#define NUMBER_OF_PORTS 6
#define NUMBER_OF_SWITCHS 8
#define NUMBER_OF_LISTS 100


/*void InsertConfigInDB(switx **ptr_switches, int *ptr_count_switches, MYSQL* conn);
void Show(MYSQL* conn);
void Select_args(MYSQL *conn, char *buffer);
void ReadArgumentValue(char* arg, char* ptr, size_t bufferLength, char* dest);*/

#define MAX_SWITCH_NAME_LENGTH 20



static json *parse(const char *path)
{
    char *text = file_read(path);

    if (text == NULL)
    {
        fprintf(stderr, "Can't read %s\n", path);
        exit(EXIT_FAILURE);
    }

    json *node = json_parse(text);

    free(text);
    if (node == NULL)
    {
        fprintf(stderr, "Can't parse %s\n", path);
        exit(EXIT_FAILURE);
    }
    return node;
}


static void read_values(const json *node, struct t_port_values_list *port_values_list)
{
    node = json_item(node, 0);

    struct t_port_values current_port_values;

    while (node != NULL)
    {
        json *data = json_item(node, 0);
        if(json_is_real(data))
        {
            printf("Period: %lu\n", json_real(data));
            current_port_values.period = json_real(data);
        }

        data = json_item(node, 1);
        if(json_is_real(data))
        {
            printf("Gate_states: %lu\n", json_real(data));
            current_port_values.gate_states = (int)json_real(data); //TODO: bit
        }

        node = json_next(node);

        port_values_push(port_values_list, current_port_values);
    }
}

static void read_json(const json *node, void* list)
{
    struct t_switch_list *switch_list = (struct t_switch_list*)list;

//    struct t_switch *current_switch = NULL;
//    current_switch = (struct t_switch *) malloc(sizeof(struct t_switch));
//    current_switch->port_list = switch_create_port_list();


    json *data_switch = json_node(node, "switch");
    if (json_is_string(data_switch))
    {
        printf("[SWITCH]---vvvvvv\n");

        struct t_switch current_switch;
        current_switch.port_list = (struct t_port_list *)malloc(sizeof(struct t_port_list));

        printf("%s -> %s\n", json_name(data_switch), json_string(data_switch));
        //strcpy(current_switch->name, json_string(data_switch));
        strcpy(current_switch.name, json_string(data_switch));

        json *data_port_list = json_node(node, "port_list");
        if (json_is_array(data_port_list))
        {
            for (size_t i = 0; i < json_items(data_port_list); i++)
            {
                json *n_port = json_item(data_port_list, i);
                printf("==== PORT %zu ====\n", i);

                struct t_port current_port;
                current_port.values = (struct t_port_values_list *) malloc(sizeof(struct t_port_values_list));

                printf("port number: %d\n", (int)json_real(json_node(n_port, "port_number")));
                //current_switch->port_list->port.number = (int)json_real(json_node(n_port, "port_number"));
                current_port.number = (int)json_real(json_node(n_port, "port_number"));
                read_values(json_node(n_port, "values"), current_port.values);

                port_push(current_switch.port_list, current_port);
            }
            printf("[PORT LIST]---^^^^^^^^\n");

        }
        else
        {
            printf("Missing port list in switch [%s]...\n", json_string(data_switch)); // TODO: test
        }

        switch_push(switch_list, current_switch);
        printf("[SWITCH]---^^^^^^^^\n");
    }


}



int main(void)
{
    struct t_switch_list* switch_list = switch_create_list();

    json *node = parse("../config.json");

    json_foreach(node, (void*)switch_list, read_json);

    switch_print_list(switch_list);

    return 0;
}

