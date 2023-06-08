#include "functions.h"

/*

    .blckchn file structure

    space for checking if it is empty [1]

    number of nodes [5]

    node
        nid [20]
        b_quantity [20]

    block
        bid [20]


*/

int main(){
    int fd;
    
    fd = open("data.blckchn",O_RDWR);
    if (fd==-1){
        close(fd);
        fd = open("data.blckchn", O_RDWR|O_CREAT);
        // printf("No Backup Found: Starting New Blockchain\n");
        
        blockchain* blckchn = malloc(sizeof(blockchain));
        blckchn->N_nodes = 0;
        handle_input(blckchn);
    }
    else{
        char space;
        int readed = read(fd, &space, 1);
        blockchain* blckchn;
        if(readed){
            // printf("Restoring From Backup\n");
            
            char* n_nodes = (char*)calloc(6,sizeof(char));
            read(fd, n_nodes, 5);
            int n_of_nodes = string_to_int(n_nodes);
            free(n_nodes);

            blckchn = (blockchain*)malloc(sizeof(blockchain));
            blckchn->N_nodes=0;
            for(int i=0; i<n_of_nodes; i++){
                char* nid_s = (char*)calloc(21, sizeof(char));
                read(fd, nid_s, 20);

                char* b_n = (char*)calloc(21, sizeof(char)); // block n
                read(fd, b_n, 20);
                int b_n_int = string_to_int(b_n);
                free(b_n);
                node_add_last(blckchn, nid_s, 1);
                for(int i=0; i<b_n_int; i++){
                    char* bid_s = (char*)calloc(21, sizeof(char));
                    read(fd, bid_s, 20);
                    block_add_last(blckchn, bid_s, nid_s , 1);
                    free(bid_s);
                }
                free(nid_s);
            }
            close(fd);
        }
        else{
            // printf("No Backup Found: Starting New Blockchain\n");
            blckchn = malloc(sizeof(blockchain));
            blckchn->N_nodes = 0;
        }
        handle_input(blckchn);
    }
    close(fd);
    return 0;
}