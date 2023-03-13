#include "my_ls.h"

file_list* make_curr_direc(){
    struct stat* sb = malloc(sizeof(struct stat));
    file_list* prev = NULL;
    lstat(".",sb);
    file_list* curr_dir = create_node(".",sb,1, prev);
    return curr_dir;
}


file_list* make_lists(int argc, char** argv, int index, int* flags){
    file_list* head = NULL;
    file_list* files = NULL;
    file_list* prev = NULL;
    size_t size = 0;
    while(index < argc){
        struct stat* sb = malloc(sizeof(struct stat));
        char* pathname = argv[index];
        if((lstat(pathname,sb)) == -1){
            *flags |= UNKNOWN_FILES_EXIST;

            // printf("\033[0;35m%s\n\033[0m",files->pathname);
            printf("\033[0;31mls: %s No such file or directory\n\033[0m", pathname);
        }
        else if(S_ISREG(sb->st_mode) || S_ISDIR(sb->st_mode)){
            size++;
            if(head == NULL){
                files = create_node(pathname, sb, size, prev);
                head = files;
            }
            else{
                files->next = create_node(pathname, sb, size, prev);
                prev = files;
                files = files->next;
                head->size++;
            }
        }
        index++;
    }
    return head;
}

file_list* create_node(char* pathname, struct stat* sb, size_t size, file_list* prev){
    file_list* new_node = malloc(sizeof(file_list));
    new_node->pathname = pathname;
    new_node->sb = sb;
    new_node->size = size;
    new_node->next = NULL;
    new_node->prev = prev;
    return new_node;
}

void free_node(file_list* node){
    node->next = NULL;
    node->pathname = NULL;
    node->sb = NULL;
    node->prev = NULL;
    node->size = 0;
    free(node);
}
