#include <stdio.h>
#include <stdlib.h>
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

int main(void)
{
    json *node = parse("test.json");

    json_print(node);
    json_free(node);
    return 0;
}
