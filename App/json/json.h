/*!
 *  \brief     JSON parser
 *  \author    David Ranieri <davranfor@gmail.com>
 *  \copyright GNU Public License.
 */

#ifndef JSON_H
#define JSON_H

typedef struct json json;

enum json_type
{
    JSON_EMPTY,     /* Vacío (único elemento de un objeto o matriz) */
    JSON_OBJECT,    /* {Objeto} */
    JSON_ARRAY,     /* [Matriz] */
    JSON_STRING,    /* "Cadena" */
    JSON_NUMBER,    /* Número entero o con decimales */
    JSON_BOOLEAN,   /* true o false */
    JSON_NULL,      /* null */
    JSON_NONE,      /* Puntero nulo */
};

enum json_type json_type(const json *);
char *json_name(const json *);
char *json_string(const json *);
double json_number(const json *);
long json_integer(const json *);
unsigned long json_real(const json *);
int json_boolean(const json *);
int json_is_empty(const json *);
int json_is_object(const json *);
int json_is_array(const json *);
int json_is_string(const json *);
int json_is_number(const json *);
int json_is_integer(const json *);
int json_is_real(const json *);
int json_is_boolean(const json *);
int json_is_null(const json *);
int json_streq(const json *, const char *);
json *json_parse(const char *);
json *json_parent(const json *);
json *json_node(const json *, const char *);
json *json_child(const json *, const char *);
json *json_next(const json *);
json *json_item(const json *, size_t);
size_t json_items(const json *);
void json_foreach(const json *, void *, void (*)(const json *, void *));
void json_print(const json *);
void json_free(json *);

size_t json_encode(char *, const char *);

#endif /* JSON_H */
