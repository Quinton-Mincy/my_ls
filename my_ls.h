#ifndef MY_LS_H
#define MY_LS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdbool.h>
#define INC_DOT_FILES 0b1
#define TIME_SORT 0b10
#define UNKNOWN_FILES_EXIST 0b100

typedef struct s_file_list{
    struct s_file_list* prev;
    struct s_file_list* next;
    char* pathname;
    size_t size;
    struct stat* sb;
}file_list;


void sorter(file_list** sorted_head, file_list* new_node,bool (*cmp)(file_list*,file_list*));
void insertionSort(file_list** head_ptr, bool (*cmp)(file_list*, file_list*));
int getFlags(int argc, char** argv, int *index);
file_list* create_node(char* pathname, struct stat* sb, size_t size, file_list* prev);
bool lex_cmp(file_list* curr_node, file_list* next_node);
bool time_cmp(file_list* curr_node, file_list* next_node);
file_list* make_lists(int argc, char** argv, int index, int* flags);
void free_node(file_list* node);
void writeFiles(file_list* head, bool dir);
bool ends_with(char *str, char c);
void formatDirPrint(char* dir_path, int length, int index,  bool are_files, int flags);
void printDirectories(file_list* directories, int flags, bool are_files);
void printFiles(file_list* files, int flags);
file_list* make_curr_direc();

#endif
