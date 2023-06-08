#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct linked_list{
    char* bid;
    struct linked_list* next;
} linked_list;

typedef struct node{
    int nid, b_quantity;
    linked_list* block;
    struct node* next;
} node;

typedef struct blockchain{
    int N_nodes;
    node* nodes;
} blockchain;


void free_linked_list(linked_list* head);

int my_strlen(char* string);

int my_strcmp(char* p1, char* p2);

void fill_null(char* string);

void my_itoa(int num, char* str);

void my_strcpy(char* p1, char* p2);

void print_error(int type);

char** string_to_S_A(char* string, int* len);

void free_string_array(char** string_array, int len);

int string_to_int(char* string);

void string_array_sort(char** p1, int len);

int string_array_checker(char** p1, char** p2, int len);

void ll_add_last(linked_list* blocks, char* bid, node* dest);

int check_ll(node* node_, char* bid);

void missing_add_ll(node* dest,node* node_);

void sync_blockchain(blockchain* blckchn);

void node_add_last(blockchain* blckchn, char* node_name, int restored);

void block_add_last(blockchain* blckchn, char* bid, char* nid, int restored);

void remove_node(blockchain* blckchn, char* node_name);

void remove_block(blockchain* blckchn, char* bid, char* node_name);

char sync_checker(blockchain* blckchn);

void blockchain_ls(blockchain* blckchn, char* option);

void save_to_file(blockchain* blckchn, int trunc);

void handle_input(blockchain* blckchn);

#endif