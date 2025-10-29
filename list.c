#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

List* list_alloc() {
    List *list = (List*)malloc(sizeof(List));
    list->head = NULL;
    list->size = 0;
    return list;
}

void list_free(List *list) {
    Node *curr = list->head;
    while (curr) {
        Node *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    free(list);
}

void list_add_to_front(List *list, int value) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->value = value;
    new_node->next = list->head;
    list->head = new_node;
    list->size++;
}

void list_add_to_back(List *list, int value) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->value = value;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node *curr = list->head;
        while (curr->next) curr = curr->next;
        curr->next = new_node;
    }
    list->size++;
}

int list_remove_at_index(List *list, int index) {
    if (index < 0 || index >= list->size || !list->head) return -1;

    Node *curr = list->head;
    Node *prev = NULL;
    for (int i = 0; i < index; i++) {
        prev = curr;
        curr = curr->next;
    }

    int val = curr->value;
    if (prev) prev->next = curr->next;
    else list->head = curr->next;

    free(curr);
    list->size--;
    return val;
}

int list_length(List *list) {
    return list->size;
}

char* listToString(List *list) {
    static char buf[512];
    buf[0] = '\0';
    strcat(buf, "[");

    Node *curr = list->head;
    while (curr) {
        char temp[32];
        sprintf(temp, "%d", curr->value);
        strcat(buf, temp);
        if (curr->next) strcat(buf, ", ");
        curr = curr->next;
    }
    strcat(buf, "]");
    return buf;
}

void list_add_at_index(List *list, int index, int value) {
    if (index <= 0) {
        list_add_to_front(list, value);
        return;
    }
    if (index >= list->size) {
        list_add_to_back(list, value);
        return;
    }

    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->value = value;

    Node *curr = list->head;
    for (int i = 0; i < index - 1; i++) curr = curr->next;

    new_node->next = curr->next;
    curr->next = new_node;
    list->size++;
}

int list_remove_from_back(List *list) {
    if (!list->head) return -1;
    if (!list->head->next) {
        int val = list->head->value;
        free(list->head);
        list->head = NULL;
        list->size--;
        return val;
    }

    Node *curr = list->head;
    while (curr->next->next) curr = curr->next;
    int val = curr->next->value;
    free(curr->next);
    curr->next = NULL;
    list->size--;
    return val;
}

int list_remove_from_front(List *list) {
    if (!list->head) return -1;

    Node *temp = list->head;
    int val = temp->value;
    list->head = temp->next;
    free(temp);
    list->size--;
    return val;
}

int list_get_elem_at(List *list, int index) {
    if (index < 0 || index >= list->size) return -1;

    Node *curr = list->head;
    for (int i = 0; i < index; i++) curr = curr->next;
    return curr->value;
}
