#include "utils.h"
#include "parser.h"
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


/*********************************************************/
struct t_port_values
{
    float hypercyple;
    unsigned long period;
    int gate_states;
    size_t size;
    struct t_port_values* next;
};

struct t_port
{
    int number;
    struct t_port_values * values;
};

struct t_port_list
{
    struct t_port port;
    size_t size;
    struct t_port_list* next;
};

struct t_switch
{
    char name[MAX_SWITCH_NAME_LENGTH];
    struct t_port_list * port_list;
};

struct t_switch_list
{
    struct t_switch sw;
    size_t size;
    struct t_switch_list* next;
};

static struct t_port_values* createPortValues(void)
{
    struct t_port_values* port_values = NULL;
    port_values = (struct t_port_values * )malloc(sizeof(struct t_port_values));

    if(port_values == NULL)
    {
        printf("[ERROR]Could not create port values...\n");
        exit(EXIT_FAILURE);
    }

    port_values->size = 0;
    port_values->next = NULL;

    return port_values;
}

static struct t_port_list* createPortList(void)
{
    struct t_port_list* port_list = NULL;
    port_list = (struct t_port_list * )malloc(sizeof(struct t_port_list));

    if(port_list == NULL)
    {
        printf("[ERROR]Could not create port list...\n");
        exit(EXIT_FAILURE);
    }

    port_list->size = 0;
    port_list->next = NULL;

    port_list->port.values = createPortValues();

    return port_list;
}

static struct t_switch_list* createSwitchList(void)
{
    struct t_switch_list* switch_list = NULL;
    switch_list = (struct t_switch_list * )malloc(sizeof(struct t_switch_list));

    if(switch_list == NULL)
    {
        printf("[ERROR]Could not create switch list...\n");
        exit(EXIT_FAILURE);
    }

    switch_list->size = 0;
    switch_list->next = NULL;

    switch_list->sw.port_list = createPortList();

    return switch_list;
}
/*********************************************************/


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

//static void read_array(const json *node, void* data)
//{
//    (void)data;

//    if(json_type(node) == JSON_NUMBER)
//    {
//        printf("PERIOD ---> %s\n", json_string(node));
//        node = json_next(node);
//        printf("GATESTATE ---> %s\n", json_string(node));
//    }
//}

static void read_values(const json *node, struct t_port_values *port_values)
{
    node = json_item(node, 0);

    struct t_port_values *current_port_values = createPortValues();



    while (node != NULL)
    {
        json *data = json_item(node, 0);
        if(json_is_real(data))
        {
            printf("Period: %lu\n", json_real(data));
            current_port_values->period = json_real(data);
        }

        data = json_item(node, 1);
        if(json_is_real(data))
        {
            printf("Gate_states: %lu\n", json_real(data));
            current_port_values->gate_states = (int)json_real(data); //TODO: bit
        }

        node = json_next(node);

        (current_port_values->size)++;
    }

}

static void read_json(const json *node, void* switch_data)
{
    (void)switch_data;
    /*struct t_switch_list* switch_list = NULL;
    switch_list = (struct t_switch_list*)switch_data;*/

    struct t_switch* current_switch = NULL;
    current_switch = (struct t_switch *) malloc(sizeof(struct t_switch));
    current_switch->port_list = createPortList();

    json *data_switch = json_node(node, "switch");
    if(json_is_string(data_switch))
    {
        printf("[SWITCH]---vvvvvv\n");
        printf("%s -> %s\n", json_name(data_switch), json_string(data_switch));
        strcpy(current_switch->name, json_string(data_switch));

        json *data_port_list = json_node(node, "port_list");
        if(json_is_array(data_port_list))
        {
            printf("[PORT LIST, size = %zu]---vvvvvv\n", json_items(data_port_list));
            current_switch->port_list->size = json_items(data_port_list);
            for(size_t i = 0; i < json_items(data_port_list); i++)
            {
                json *n_port = json_item(data_port_list, i);
                printf("==== N-PORT ====\n");

                printf("port number: %d\n", (int)json_real(json_node(n_port, "port_number")));
                current_switch->port_list->port.number = (int)json_real(json_node(n_port, "port_number"));

                read_values(json_node(n_port, "values"), current_switch->port_list->port.values);
            }
            printf("[PORT LIST]---^^^^^^^^\n");

        }
        else
        {
            printf("Missing port list in switch [%s]...\n", json_string(data_switch)); // TODO: test
        }

        printf("[SWITCH]---^^^^^^^^\n");
    }

    free(current_switch);
}



int main(void)
{
    struct t_switch_list* switch_list = createSwitchList();

    json *node = parse("../config.json");

    json_foreach(node, (void*)switch_list, read_json);

    return 0;
}

