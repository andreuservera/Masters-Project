#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "json.h"

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

static void read_values(const json *node)
{
    node = json_item(node, 0);
    puts("values: [");
    while (node != NULL)
    {
        printf("\t[");

        for (size_t i = 0; i < 2; i++)
        {
            json *data = json_item(node, i);
            if (json_is_real(data))
            {
                printf(" %lu ", json_real(data));
            }
        }
        node = json_next(node);
        printf("]\n");
    }
    puts("]");
}

static void read_switch(const json *node)
{
    node = json_item(node, 0);

    while (node != NULL)
    {
        json *data;

        data = json_node(node, "switch");
        if (json_is_string(data))
        {
            printf("%s\n", json_string(data));
        }
        data = json_node(node, "port_number");
        if (json_is_real(data))
        {
            printf("%lu\n", json_real(data));
        }
        read_values(json_node(node, "values"));
        node = json_next(node);
        puts("--------------");
    }
}

int main(void)
{
    json *node = parse("../config.json");

    read_switch(node);
    json_free(node);
    return 0;
}
