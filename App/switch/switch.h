#ifndef SWITCH_H
#define SWITCH_H

#define MAX_SWITCH_NAME_LENGTH 20


struct t_port_values
{
    unsigned long period;
    int gate_states;
};

struct t_port_values_list
{
    struct t_port_values values;
    size_t size;
    struct t_port_values_list *next;
};

struct t_port
{
    int number;
    struct t_port_values_list * values;
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


struct t_port_values_list* switch_create_port_values_list(void);
struct t_port_list* switch_create_port_list(void);
struct t_switch_list* switch_create_list(void);
void switch_push(struct t_switch_list *, struct t_switch);
void port_push(struct t_port_list *, struct t_port);
void port_values_push(struct t_port_values_list *, struct t_port_values);
void switch_print_list(struct t_switch_list *);
void free_switch_list(struct t_switch_list *);


#endif /* SWITCH_H */

