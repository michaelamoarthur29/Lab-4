#ifndef LIST_H
#define LIST_H

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    int size;
} List;

List* list_alloc();
void list_free(List *list);
void list_add_to_front(List *list, int value);
void list_add_to_back(List *list, int value);
int list_remove_at_index(List *list, int index);
int list_length(List *list);
char* listToString(List *list);

void list_add_at_index(List *list, int index, int value);
int list_remove_from_back(List *list);
int list_remove_from_front(List *list);
int list_get_elem_at(List *list, int index);

#endif
