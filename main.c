#include "my_ls.h"
int main(int argc, char** argv){
    if(argc == 1){//no arguments
        entryList* currDir = readCurrDir();
        entryList* lexSortedList_ = lexSortedList(currDir);
        writeCurrDir(lexSortedList_);
    }else{//arguments present, unclear whether flags, files, or both are present in the arguments, need to parse
        readInput(argc,argv);
    }
    return 0;
}
//tests:
    //./my_ls stuff testdir bnuwcbeuri testfile testdir2 main.c -breaks: trys to display unknown file like a directory
    //./my_ls testdir testdir2 main.c stuff wweeffewrwg testfile -breaks: displays unknown files like regular files
    //./my_ls stuff niuner hwberffyibewrig testdir testdir2 main.c testfile: works

    //      char* fileName;
    //      struct stat* sb = malloc(sizeof(struct stat));
    //      stat(fileName,sb);