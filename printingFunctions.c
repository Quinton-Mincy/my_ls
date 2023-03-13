#include "my_ls.h"

void formatDirPrint(char* dir_path, int length, int index,  bool are_files, int flags){
        if(are_files){
            // printf("\n%s:\n",dir_path);
            printf("\033[0;33m\n%s:\n\033[0m\n",dir_path);
        }
        else if(length == 1 && flags & UNKNOWN_FILES_EXIST){
            // printf("%s:\n",dir_path);
            printf("\033[0;33m%s:\n\033[0m\n",dir_path);
        }
        else if(length > 1){
            if(index == 0){
                // printf("%s:\n",dir_path);
                printf("\033[0;33m%s:\n\033[0m\n",dir_path);
            }
            else{
                // printf("\n%s:\n",dir_path);
                printf("\033[0;33m\n%s:\n\033[0m\n",dir_path);
            }
        }
}
void printDirectories(file_list* directories, int flags, bool are_files){
    file_list* head = NULL;
    file_list* files = NULL;
    file_list* prev = NULL;
    DIR* dir;
    struct dirent* entry;
    int length = directories->size;
    int index = 0;
    while(index < length){
        int size = 0;
        char* dir_path = directories->pathname;
        formatDirPrint(dir_path, length, index, are_files, flags);
        dir = opendir(dir_path);
        head = NULL;
        files = NULL;
        prev= NULL;
        while((entry = readdir(dir))){
            char fullpath[255] = { '\0' };
            if (strcmp(".", dir_path) != 0) {
                strcpy(fullpath, dir_path);
            }
            if (ends_with(dir_path, '/') == false) {
                strcat(fullpath, "/");
            }
            strcat(fullpath, entry->d_name);
            struct stat* sb = malloc(sizeof(struct stat));
            if(!(flags & INC_DOT_FILES) && entry->d_name[0] == '.'){
                continue;
            }
            else{
                size++;
                char* pathname = entry->d_name;
                if((strcmp(".", dir_path) == 0)){
                    lstat(entry->d_name,sb);
                }else{
                    lstat(fullpath,sb);
                }
                if(files == NULL){
                    files = create_node(pathname,sb, size, prev);
                    head = files;
                }else{
                    files->next = create_node(pathname, sb, size, prev);
                    prev = files;
                    files = files->next;
                    head->size++;
                }
            }
        }
        
        if(flags & TIME_SORT){
            insertionSort(&head, &time_cmp);
        }
        else{
            insertionSort(&head, &lex_cmp);
        }
        bool dir = false;
        if(length>1){
            dir = true;
        }
        writeFiles(head,dir);
        file_list* temp = directories;
        directories = directories->next;
        free_node(temp);
        index++;
    }
}

void printFiles(file_list* files, int flags){
    file_list* directories = NULL;
    file_list* prev = NULL;
    bool are_files = false;
    size_t size = 0;
    file_list* head = NULL;
    // bool is_dir = false;
    int index = 0;
    int length = files->size;
    while(index < length ){
        if(S_ISDIR(files->sb->st_mode)){
            size++;
            if(head == NULL){
                directories = create_node(files->pathname, files->sb, size, prev);
                prev = directories;
                head = directories;
            }else{
                directories->next = create_node(files->pathname, files->sb, size, prev);
                prev = directories;
                directories = directories->next;
                head->size++;
            }
            files = files->next;
        }
        else if(!(flags & INC_DOT_FILES) && files->pathname[0] == '.'){
            are_files = true;
            files = files->next;
        }
        else{
            are_files = true;
            file_list* temp = files;
            if(files->sb->st_mode & S_IXUSR || files->sb->st_mode &S_IXGRP){
                // file has execute permission for owner
                printf("\033[1;32m%s\n\033[0m",files->pathname);
            }else{    
                printf("\033[0;35m%s\n\033[0m",files->pathname);
            }   
            // printf("%s\n",files->pathname);
            files = files->next;
            free_node(temp);
        }
        index++;
    }
    if(directories != NULL){
        printDirectories(head, flags, are_files);
    }
}
//need to fix bug. if come across subdirectory, will treat it as a regualr directory and print things after it as if it in a reg directory. need to find a way to recirsively print contents of subdirectories
//if a direcotry, in the file struct have an fd member?
void writeFiles(file_list* head, bool dir){
    while(head != NULL){
        if(S_ISDIR(head->sb->st_mode)){//if only printing out cwd
            if(dir == true){
            }
            printf("\033[0;33m%s\n\033[0m",head->pathname);//print files in sub directory
        }else if (head->sb->st_mode & S_IXUSR || head->sb->st_mode &S_IXGRP) {
            // file has execute permission for owner
            printf("\033[1;32m%s\n\033[0m",head->pathname);
        }else{
            if(dir == true){
                printf("\t");
                printf("\033[0;32m%s\n\033[0m",head->pathname);//print files in sub directory
            }else{
                printf("\033[0;35m%s\n\033[0m",head->pathname);//print files in current directory
            }
        }
        // printf("%s\n",head->pathname);
        file_list* temp = head;
        head = head->next;
        free_node(temp);
    }
}

bool ends_with(char *str, char c) {
    int endex = strlen(str) - 1;

    if (endex >= 0) {
        return str[endex] == c;
    }
    return false;
}

