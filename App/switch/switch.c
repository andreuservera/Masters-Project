#include <stdlib.h>
#include <stdio.h>
#include "switch.h"


struct t_port_values_list* switch_create_port_values_list(void)
{
    struct t_port_values_list* port_values_list = NULL;
    port_values_list = (struct t_port_values_list * )malloc(sizeof(struct t_port_values_list));

    if(port_values_list == NULL)
    {
        return NULL;
    }

    port_values_list->size = 0;
    port_values_list->next = NULL;

    return port_values_list;
}

struct t_port_list* switch_create_port_list(void)
{
    struct t_port_list* port_list = NULL;
    port_list = (struct t_port_list * )malloc(sizeof(struct t_port_list));

    if(port_list == NULL)
    {
        return NULL;
    }

    port_list->size = 0;
    port_list->next = NULL;

    port_list->port.values = switch_create_port_values_list();

    return port_list;
}

struct t_switch_list* switch_create_list(void)
{
    struct t_switch_list* switch_list = NULL;
    switch_list = (struct t_switch_list * )malloc(sizeof(struct t_switch_list));

    if(switch_list == NULL)
    {
        return NULL;
    }

    switch_list->size = 0;
    switch_list->next = NULL;

    switch_list->sw.port_list = switch_create_port_list();

    return switch_list;
}

void switch_push(struct t_switch_list * head, struct t_switch data)
{
    struct t_switch_list *current = head;

    while(current->next != NULL)
    {
        current = current->next;
    }

    current->next = (struct t_switch_list *)malloc(sizeof(struct t_switch_list));
    current->next->sw = data;
    current->next->next = NULL;
}

void port_push(struct t_port_list * head, struct t_port data)
{
    struct t_port_list *current = head;

    while(current->next != NULL)
    {
        current = current->next;
    }

    current->next = (struct t_port_list *)malloc(sizeof(struct t_port_list));
    current->next->port = data;
    current->next->next = NULL;
}

void port_values_push(struct t_port_values_list * head, struct t_port_values data)
{
    struct t_port_values_list *current = head;

    while(current->next != NULL)
    {
        current = current->next;
    }

    current->next = (struct t_port_values_list *)malloc(sizeof(struct t_port_values_list));
    current->next->values = data;
    current->next->next = NULL;
}

void switch_print_list(struct t_switch_list *head)
{
    struct t_switch_list *current_sw_list = head;

    while (current_sw_list != NULL)
    {
        printf("Switch:%s\n", current_sw_list->sw.name);

        struct t_port_list *current_port_list = current_sw_list->sw.port_list;
        while (current_port_list != NULL)
        {
            printf("\tPort Number: %d\n", current_port_list->port.number);
            current_port_list = current_port_list->next;
        }

        current_sw_list = current_sw_list->next;
    }
}



