#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

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

void free_linked_list(linked_list* head) {
    linked_list* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->bid);
        free(temp);
    }
}

int my_strlen(char* string){
    int len=0;
    while(string[len]) len++;
    return len;
}

int my_strcmp(char* p1, char* p2){
    for(int i  = 0; p1[i] || p2[i]; i++){
        if(p1[i] != p2[i]){
            return (int)(p1[i] - p2[i]);
        }
    }
    return 0;
}

void fill_null(char* string){
    int len = my_strlen(string);
    for(int i=0; i<len; i++){
        string[i]='\0';
    }
}

void my_itoa(int num, char* str) {
    if (!num){
        str[0]='0';
    }
    else{
        int i = 0, sign = 0;
        if (num < 0) {
            sign = 1;
            num = -num;
        }
        while (num != 0) {
            int rem = num % 10;
            str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
            num = num / 10;
        }

        if (sign) {
            str[i++] = '-';
        }

        str[i] = '\0';

        int len = i;
        for (int j = 0; j < len / 2; j++) {
            char temp = str[j];
            str[j] = str[len - j - 1];
            str[len - j - 1] = temp;
        }
    }
}

void my_strcpy(char* p1, char* p2){
    for(int i=0; i<my_strlen(p2); i++){
        p1[i]=p2[i];
    }
}

void print_error(int type){
    if (type==1){
        printf("nok: no more resources available on the computer\n");
    }
    else if (type==2){
        printf("nok: this node already exists\n");
    }
    else if (type==3){
        printf("nok: this block already exists\n");
    }
    else if (type==4){
        printf("nok: node doesn't exist\n");
    }
    else if (type==5){
        printf("nok: block doesn't exist\n");
    }
    else if (type==6){
        printf("nok: command not found\n");
    }
}

char** string_to_S_A(char* string, int* len){
    // strip the ending
    int space_f = 0;
    for(int i=my_strlen(string)-1; i>=0; i--){
        if(string[i]!=' '){
            break;
        }
        string[i]='\0';
    }

    if (my_strlen(string)){
        *len+=1;
    }
    int spc = 0;
    for(int i=0; i<my_strlen(string); i++){
        if (!spc){
            if(string[i]!=' ') spc=1;
        }
        else if (spc){
            if(string[i]==' '){
                if(!space_f){
                    *len+=1;
                    space_f=1;
                }
            }
            else{
                space_f=0;
            }
        }
    }
    space_f=0;
    char** result = (char**)calloc(*len, sizeof(char*));
    int indexw=0;
    int indexx=0;
    result[indexx] = (char*)calloc(my_strlen(string)+1,sizeof(char));
    spc = 0;
    for(int i=0; i<my_strlen(string); i++){
        if (!spc){
            if(string[i]!=' ') spc=1;
        }
        if (spc){
            if (string[i]!=' '){
                result[indexx][indexw]=string[i];
                indexw++;
                space_f=0;
            }
            else{
                if(!space_f){
                    indexw=0;
                    indexx++;
                    if (indexx<*len){
                        result[indexx] = (char*)calloc(my_strlen(string)+1,sizeof(char));
                    }
                    space_f=1;
                }
            }
        }
    }
    return result;
}

void free_string_array(char** string_array, int len){
    for(int i=0; i<len; i++){
        free(string_array[i]);
    }
}

int string_to_int(char* string){
    int increaser=1;
    int integer = 0;
    for(int i=my_strlen(string)-1; i>=0; i--){
        integer+=increaser*(string[i]-48);
        increaser*=10;
    }
    return integer;
}

void string_array_sort(char** p1, int len){
    if (len>1){
        for (int i=0; i<len; i++){
            for (int j=0; j<len-1; j++){
                if (my_strcmp(p1[j], p1[j+1])>0){
                    char* temp = p1[j];
                    p1[j]=p1[j+1];
                    p1[j+1]=temp;
                }
            }
        }
    }
}

int string_array_checker(char** p1, char** p2, int len){
    for (int i=0; i<len; i++){
        if (my_strcmp(p1[i], p2[i])){
            return 0;
        }
    }
    return 1;
}

void ll_add_last(linked_list* blocks, char* bid, node* dest){
    linked_list* head = blocks;
    if(dest->b_quantity){
        while(head->next){
            head = head->next;
        }
        head->next = (linked_list*)malloc(sizeof(linked_list));
        head->next->bid = (char*)calloc(my_strlen(bid)+1,sizeof(char));
        my_strcpy(head->next->bid, bid);
        head->next->next=NULL;
    }
    else{
        dest->block = (linked_list*)malloc(sizeof(linked_list));
        dest->block->bid = (char*)calloc(my_strlen(bid)+1,sizeof(char));
        my_strcpy(dest->block->bid, bid);
        dest->block->next=NULL;
    }
}

int check_ll(node* node_, char* bid){

    if(!node_->b_quantity) return 0;
    linked_list* head = node_->block;
    while(head){
        if(!my_strcmp(head->bid,bid)){
            return 1;
        }
        head = head->next;
    }
    return 0;

}

void missing_add_ll(node* dest,node* node_){
    if(node_->b_quantity){
        linked_list* head_ll = node_->block;
        while(head_ll){
            if(!check_ll(dest, head_ll->bid)){
                ll_add_last(dest->block, head_ll->bid, dest);
                dest->b_quantity++;
            }
            head_ll = head_ll->next;
        }
    }
}

void sync_blockchain(blockchain* blckchn){
    if(blckchn->N_nodes>1){
        // Idealizing the first node
        node* node_next = blckchn->nodes->next;
        if(blckchn->nodes->next){
        while(node_next){
            missing_add_ll(blckchn->nodes, node_next);
            node_next = node_next->next;
        }
        }
        // Idealizing the rest ones
        node_next = blckchn->nodes->next;
        while(node_next){
            missing_add_ll(node_next,blckchn->nodes);
            node_next = node_next->next;
        }
    }
    printf("OK\n");
    fflush(stdout);
}

void node_add_last(blockchain* blckchn, char* node_name, int restored){
    int nid = string_to_int(node_name);
    if (!blckchn->N_nodes){
        blckchn->nodes = (node*)malloc(sizeof(node));
        blckchn->nodes->nid = nid;
        blckchn->nodes->next=NULL;
        blckchn->nodes->b_quantity=0;
        blckchn->N_nodes=1;

    }
    else if (blckchn->N_nodes==1){
        if(blckchn->nodes->nid==nid){
            print_error(2);
            return;
        }
        blckchn->nodes->next=(node*)malloc(sizeof(node));
        blckchn->nodes->next->nid = nid;
        blckchn->nodes->next->next = NULL;
        blckchn->nodes->next->b_quantity=0;
        blckchn->N_nodes=2;
    }
    else{
        node* blckchn_nodes = blckchn->nodes;
        if (blckchn_nodes->nid==nid){
            print_error(2);
            return;
        }
        while(blckchn_nodes->next){
            if (blckchn_nodes->next->nid==nid){
                print_error(2);
                return;
            }
            blckchn_nodes=blckchn_nodes->next;
        }
        blckchn_nodes->next = (node*)malloc(sizeof(node));
        blckchn_nodes->next->nid = nid;
        blckchn_nodes->next->next = NULL;
        blckchn_nodes->next->b_quantity=0;
        blckchn->N_nodes+=1;
    }
    if(!restored){
        printf("OK\n");
        
    }
}

void block_add_last(blockchain* blckchn, char* bid, char* nid, int restored){
    if (nid[0]=='*'){
        if (!blckchn->N_nodes){
            print_error(4);
            return;
        }
        node* blckchn_nodes = blckchn->nodes;
        while(blckchn_nodes){
            if (!blckchn_nodes->b_quantity){
                blckchn_nodes->block = (linked_list*)malloc(sizeof(linked_list));
                blckchn_nodes->block->bid = (char*)calloc(my_strlen(bid)+1,sizeof(char));
                my_strcpy(blckchn_nodes->block->bid, bid);
                blckchn_nodes->b_quantity=1;
                blckchn_nodes->block->next = NULL;
            }
            else if (blckchn_nodes->b_quantity==1){
                if(my_strcmp(blckchn_nodes->block->bid,bid)){
                    blckchn_nodes->block->next = (linked_list*)malloc(sizeof(linked_list));
                    blckchn_nodes->block->next->bid = (char*)calloc(my_strlen(bid)+1,sizeof(char));
                    my_strcpy(blckchn_nodes->block->next->bid, bid);
                    blckchn_nodes->b_quantity=2;
                    blckchn_nodes->block->next->next = NULL;
                }
            }
            else{
                int add=1;
                linked_list* blocks = blckchn_nodes->block;
                if(!my_strcmp(blocks->bid,bid)){
                    add=0;
                }
                while (blocks->next) {
                    if(!my_strcmp(blocks->next->bid,bid)){
                        add=0;
                        break;
                    }
                    blocks = blocks->next;
                }
                if (add){
                    blocks->next = (linked_list*)malloc(sizeof(linked_list));
                    blocks->next->bid = (char*)calloc(my_strlen(bid)+1,sizeof(char));
                    my_strcpy(blocks->next->bid, bid);
                    blckchn_nodes->b_quantity+=1;
                    blocks->next->next = NULL;
                }
            }
            blckchn_nodes=blckchn_nodes->next;
        }
    }
    else{
        int node_id = string_to_int(nid);
        node* nodes = blckchn->nodes;
        int run=1;
        while(run){
            if (!nodes || !blckchn->N_nodes){
                print_error(4);
                run=0;
                return;
            }
            if(nodes->nid==node_id){
                if (!nodes->b_quantity){
                    nodes->block = (linked_list*)malloc(sizeof(linked_list));
                    nodes->block->bid = (char*)calloc(my_strlen(bid)+1,sizeof(char));
                    my_strcpy(nodes->block->bid, bid);
                    nodes->b_quantity=1;
                    nodes->block->next = NULL;
                    run=0;
                }
                else if (nodes->b_quantity==1){
                    if (!my_strcmp(nodes->block->bid, bid)){
                        print_error(3);
                        run=0;
                        return;
                    }
                    nodes->block->next = (linked_list*)malloc(sizeof(linked_list));
                    nodes->block->next->bid = (char*)calloc(my_strlen(bid)+1,sizeof(char));
                    my_strcpy(nodes->block->next->bid, bid);
                    nodes->b_quantity=2;
                    nodes->block->next->next = NULL;
                    run=0;
                }
                else{
                    linked_list* blocks = nodes->block;
                    if (!my_strcmp(blocks->bid, bid)){
                        print_error(3);
                        run=0;
                        return;
                    }
                    while (blocks->next) {
                        if (!my_strcmp(blocks->next->bid, bid)){
                            print_error(3);
                            run=0;
                            return;
                        }
                        blocks = blocks->next;
                    }
                    blocks->next = (linked_list*)malloc(sizeof(linked_list));
                    blocks->next->bid = (char*)calloc(my_strlen(bid)+1,sizeof(char));
                    my_strcpy(blocks->next->bid, bid);
                    nodes->b_quantity+=1;
                    blocks->next->next = NULL;
                }
                break;
            }
            nodes = nodes->next;
        }
    }
    if(!restored){
        printf("OK\n");
        
    }
}

void remove_node(blockchain* blckchn, char* node_name){
    int nid = string_to_int(node_name);
    if (!blckchn->N_nodes){
        print_error(4);
        return;
    }
    else if (node_name[0]=='*') {
        node* temp;
        while(blckchn->nodes){
            temp = blckchn->nodes;
            blckchn->nodes = blckchn->nodes->next;
            free_linked_list(temp->block);
            free(temp);
            blckchn->N_nodes--;
        }
        blckchn->nodes=NULL;
        printf("OK\n");
        
        return;
    }
    else if (blckchn->N_nodes==1){
        if (blckchn->nodes->nid==nid){
            free_linked_list(blckchn->nodes->block);
            free(blckchn->nodes);
            blckchn->N_nodes=0;
            printf("OK\n");
            
            return;
        }
        else{
            print_error(4);
            return;
        }
    }
    else{
        node* blckchn_nodes = blckchn->nodes;
        if (blckchn_nodes->nid==nid){
                node* temp = blckchn_nodes->next;
                free_linked_list(blckchn->nodes->block);
                free(blckchn->nodes);
                blckchn->N_nodes--;
                blckchn->nodes=temp;
                printf("OK\n");
                
                return;
            }
        else{
            node* prev;
            node* next;
            while (blckchn_nodes) {
                if (blckchn_nodes->nid==nid){
                    next = blckchn_nodes->next;
                    free_linked_list(blckchn_nodes->block);
                    free(blckchn_nodes);
                    blckchn->N_nodes--;
                    if(next)
                        prev->next=next;
                    else{
                        prev->next=NULL;
                    }
                    printf("OK\n");
                    
                    return;
                }
                prev = blckchn_nodes;
                blckchn_nodes = blckchn_nodes->next;
            }
        }
    }
    print_error(4);
}

void remove_block(blockchain* blckchn, char* bid, char* node_name){
    int nid = string_to_int(node_name);
    int found=0;
    if (!blckchn->N_nodes){
        print_error(4);
        return;
    }
    else if (node_name[0]=='*'){
        node* blckchn_nodes = blckchn->nodes;
        while(blckchn_nodes){
            if(blckchn_nodes->b_quantity){
                linked_list* temp=blckchn_nodes->block;
                if (!my_strcmp(blckchn_nodes->block->bid, bid)){
                    if (blckchn_nodes->block->next){
                        blckchn_nodes->block = blckchn_nodes->block->next;
                        free(temp->bid);
                        free(temp);
                        found++;
                    }
                    else{
                        free_linked_list(temp);
                        blckchn_nodes->block = NULL;
                        found++;
                    }
                    blckchn_nodes->b_quantity--;
                }
                else{
                    linked_list* blckchn_nodes_block = blckchn_nodes->block;
                    linked_list* prev;
                    linked_list* next;
                    while (blckchn_nodes_block) {
                        if (!my_strcmp(blckchn_nodes_block->bid,bid)){
                            next=blckchn_nodes_block->next;
                            if (next){
                                prev->next=next;
                            }
                            else{
                                prev->next=NULL;
                            }
                            blckchn_nodes->b_quantity--;
                            free(blckchn_nodes_block->bid);
                            free(blckchn_nodes_block);
                            found++;
                            break;
                        }
                        prev = blckchn_nodes_block;
                        blckchn_nodes_block=blckchn_nodes_block->next;
                    }
                }
            }
            blckchn_nodes = blckchn_nodes->next;
        }
    }
    else {
        node* blckchn_nodes=blckchn->nodes;
        while (blckchn_nodes) {
            if (blckchn_nodes->nid==nid){
                if(!blckchn_nodes->b_quantity){
                    print_error(5);
                    return;
                }
                if (!my_strcmp(blckchn_nodes->block->bid,bid)){
                    if (blckchn_nodes->block->next){
                        linked_list* temp = blckchn_nodes->block;
                        blckchn_nodes->block=blckchn_nodes->block->next;
                        free(temp->bid);
                        free(temp);
                        blckchn_nodes->b_quantity--;
                        
                        printf("OK\n");
                        return;
                    }
                    else{
                        free(blckchn_nodes->block->bid);
                        free(blckchn_nodes->block);
                        blckchn_nodes->block=NULL;
                        blckchn_nodes->b_quantity--;
                        
                        printf("OK\n");
                        return;
                    }
                }
                else{
                    linked_list* blckchn_nodes_block = blckchn_nodes->block;
                    linked_list* prev;
                    while(blckchn_nodes_block){
                        if (!my_strcmp(blckchn_nodes_block->bid, bid)){
                            if (blckchn_nodes_block->next){
                                prev->next=blckchn_nodes_block->next;
                                free(blckchn_nodes_block->bid);
                                free(blckchn_nodes_block);
                                blckchn_nodes->b_quantity--;
                                
                                printf("OK\n");
                                return;
                            }
                            else{
                                free(blckchn_nodes_block->bid);
                                free(blckchn_nodes_block);
                                prev->next=NULL;
                                blckchn_nodes->b_quantity--;
                                
                                printf("OK\n");
                                return;
                            }
                        }
                        prev = blckchn_nodes_block;
                        blckchn_nodes_block = blckchn_nodes_block->next;
                    }
                    print_error(5);
                    return;
                }
            }

            blckchn_nodes=blckchn_nodes->next;
        }
        print_error(4);
        return;
    }
    if (found){
        printf("OK\n");
        
    }
    else{
        print_error(5);
    }
}

char sync_checker(blockchain* blckchn){
    if (blckchn->N_nodes==0 || blckchn->N_nodes==1){
        return 's';
    }
    node* blckchn_nodes = blckchn->nodes;
    while (blckchn_nodes->next) {
        char** blocks1 = (char**)calloc(blckchn_nodes->b_quantity,sizeof(char*));
        char** blocks2 = (char**)calloc(blckchn_nodes->next->b_quantity,sizeof(char*));
        if(blckchn_nodes->b_quantity!=blckchn_nodes->next->b_quantity){
            free_string_array(blocks1, blckchn_nodes->b_quantity);
            free_string_array(blocks2, blckchn_nodes->next->b_quantity);
            free(blocks1);
            free(blocks2);
            return '-';
        }
        if(!blckchn_nodes->b_quantity && !blckchn_nodes->next->b_quantity){
            blckchn_nodes = blckchn_nodes->next;
            free(blocks1);
            free(blocks2);
            continue;
        }
        linked_list* blckchn_nodes_block = blckchn_nodes->block;
        for(int i=0; blckchn_nodes_block; i++){
            int len = my_strlen(blckchn_nodes_block->bid);
            blocks1[i] = (char*)calloc(len+1,sizeof(char));
            my_strcpy(blocks1[i],blckchn_nodes_block->bid);
            blckchn_nodes_block = blckchn_nodes_block->next;
        }
        linked_list* blckchn_nodes_next_block = blckchn_nodes->next->block;
        for(int i=0; blckchn_nodes_next_block; i++){
            blocks2[i] = (char*)calloc(my_strlen(blckchn_nodes_next_block->bid)+1,sizeof(char));
            my_strcpy(blocks2[i],blckchn_nodes_next_block->bid);
            blckchn_nodes_next_block = blckchn_nodes_next_block->next;
        }

        string_array_sort(blocks1, blckchn_nodes->b_quantity);
        string_array_sort(blocks2, blckchn_nodes->next->b_quantity);

        if (!string_array_checker(blocks1, blocks2, blckchn_nodes->b_quantity)){
            free_string_array(blocks1, blckchn_nodes->b_quantity);
            free_string_array(blocks2, blckchn_nodes->next->b_quantity);
            free(blocks1);
            free(blocks2);
            return '-';
        }

        free_string_array(blocks1, blckchn_nodes->b_quantity);
        free_string_array(blocks2, blckchn_nodes->next->b_quantity);
        free(blocks1);
        free(blocks2);
        blckchn_nodes = blckchn_nodes->next;
    }
    return 's';
}

void blockchain_ls(blockchain* blckchn, char* option){
    char l[] = "-l";
    char spc[] = " ";
    if (!my_strcmp(option,l)){
        linked_list* blockchain_nodes_block;
        node* blockchain_nodes = blckchn->nodes;
        while (blockchain_nodes) {
            blockchain_nodes_block = blockchain_nodes->block;
            printf("%d:",blockchain_nodes->nid);
            while (blockchain_nodes_block) {
                printf(" %s",blockchain_nodes_block->bid);
                if(blockchain_nodes_block->next){
                    printf(",");
                }
                blockchain_nodes_block = blockchain_nodes_block->next;
            }
            blockchain_nodes=blockchain_nodes->next;
            printf("\n");
        }
    }
    else if (!my_strcmp(option, spc)){
        node* blockchain_nodes = blckchn->nodes;
        while (blockchain_nodes) {
            printf("%d",blockchain_nodes->nid);
            fflush(stdout);
            if(blockchain_nodes->next){
                printf(" ");
            }
            blockchain_nodes=blockchain_nodes->next;
        }
        printf("\n");
    }
    else{
        print_error(6);
    }
}

void save_to_file(blockchain* blckchn, int trunc){
    if (trunc)
        close(open("data.blckchn",O_TRUNC));
                    
    int fd = open("data.blckchn", O_WRONLY);
    char space = ' ';
    write(fd, &space, 1);

    char* n_of_nodes = (char*)calloc(6,sizeof(char));
    my_itoa(blckchn->N_nodes, n_of_nodes);
    write(fd, n_of_nodes, 5);
    free(n_of_nodes);
    node* node_cpy = blckchn->nodes;
    while(node_cpy){
        char* node_id = (char*)calloc(21,sizeof(char));
        my_itoa(node_cpy->nid, node_id);
        write(fd, node_id, 20);
        free(node_id);
        
        char* n_of_blocks = (char*)calloc(21,sizeof(char));
        my_itoa(node_cpy->b_quantity, n_of_blocks);
        write(fd, n_of_blocks, 20);
        free(n_of_blocks);
        linked_list* block_copy = node_cpy->block;
        if(node_cpy->b_quantity){
            while(block_copy){
                char* block_id = (char*)calloc(21,sizeof(char));
                my_strcpy(block_id,block_copy->bid);
                write(fd, block_id, 20);
                free(block_id);
                block_copy = block_copy->next;
            }
        }
        node_cpy = node_cpy->next;
    }
}

void handle_input(blockchain* blckchn){
    char* input = (char*)calloc(1024,sizeof(char));
    while(1){
            printf("[%c%d]>",sync_checker(blckchn),blckchn->N_nodes);
            fflush(stdout);

            int num_read=0, c=0;
            char character;
            while(1){
                c=read(STDIN_FILENO, &character, 1);
                if(!c) break;
                else if(character=='\n'){
                    num_read++;
                    break;
                }
                else{
                    input[num_read]=character;
                    num_read++;
                }
            }
            
            if (num_read>0){
                if(num_read==1) continue;
                char quit[] = "quit", add[] = "add", rm[] = "rm", ls[] = "ls", sync[] = "sync", node[] = "node", block[] = "block";
                if (!my_strcmp(input, quit)){
                    // printf("Backing up blockchain...\n");
                    save_to_file(blckchn, 0);
                    free(input);
                    break;
                }
                int input_len = 0;
                char** clean_input = string_to_S_A(input, &input_len);
                if (!my_strcmp(clean_input[0], add)){
                    if(input_len<3){
                        print_error(6);
                    }
                    else{
                        if (!my_strcmp(clean_input[1], node)){
                            if(input_len!=3){
                                print_error(6);
                            }
                            else
                            node_add_last(blckchn, clean_input[2], 0);
                        }
                        else if (!my_strcmp(clean_input[1], block)){
                            if(input_len!=4){
                                print_error(6);
                            }
                            else
                            block_add_last(blckchn, clean_input[2], clean_input[3], 0);
                        }
                        else
                            print_error(6);
                    }
                }
                else if (!my_strcmp(clean_input[0], rm)){
                    if(input_len<3){
                        print_error(6);
                    }
                    else{
                        if (!my_strcmp(clean_input[1], node)){
                            if(input_len!=3){
                                print_error(6);
                            }
                            else
                            remove_node(blckchn, clean_input[2]);
                        }
                        else if (!my_strcmp(clean_input[1], block)){
                            if(input_len!=4){
                                print_error(6);
                            }
                            else
                            remove_block(blckchn, clean_input[2], clean_input[3]);
                        }
                        else
                            print_error(6);
                    }
                }
                else if (!my_strcmp(clean_input[0], ls)){
                    if(input_len>3)
                        print_error(6);
                    else if(input_len==1){
                        char spc[] = " ";
                        blockchain_ls(blckchn, spc);
                    }
                    else if(input_len==2){
                        blockchain_ls(blckchn, clean_input[1]);
                    }
                }
                else if (!my_strcmp(clean_input[0], sync)){
                    if (input_len>1)
                        print_error(6);
                    else{
                        sync_blockchain(blckchn);
                    }
                }
                else
                    print_error(6);
                for(int i=0; i<input_len; i++){
                    free(clean_input[i]);
                }
                free(clean_input);
                fill_null(input);
            }
            else{
                // printf("Backing up blockchain...\n");
                save_to_file(blckchn, 0);
                free(input);
                break;
            }
        }
        // free
        if(blckchn->N_nodes){
            while(blckchn->nodes){
                node* temp = blckchn->nodes;
                if (blckchn->nodes->b_quantity){
                    free_linked_list(blckchn->nodes->block);
                }
                blckchn->nodes=blckchn->nodes->next;
                free(temp);
            }
        }
        free(blckchn);
}