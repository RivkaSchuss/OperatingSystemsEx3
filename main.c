#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <memory.h>

#define SIZE 1024
#define ERROR "Error in system call\n"

int main(int argc, char *argv[] ){
    char buf1[SIZE];
    char buf2[SIZE];
    int read1 = 0, read2 = 0, ret = 1, i = 0, j = 0;
    memset(buf1, 0, SIZE);
    memset(buf2, 0, SIZE);
    if (argc < 3) {
        perror(ERROR);
        return 0;
    }
    int file1, file2;
    if ((file1 = open(argv[1], O_RDONLY)) == -1) {
        perror(ERROR);
        return 0;
    }
    if ((file2 = open(argv[2], O_RDONLY)) == -1) {
        perror(ERROR);
        close(file1);
        return 0;
    }
    //check the files for identity
    while (ret != 2) {
        i = 0, j = 0;
        read1 = (int)read(file1, buf1, SIZE);
        read2 = (int)read(file2, buf2, SIZE);
        //check for reading error
        if (read1 == -1 || read2 == -1){
            perror(ERROR);
            close(file1);
            close(file2);
            return 0;
        }
        //check for files size identity
        if (read1 != read2) {
            ret = 2;
            continue;
        }
            //check for end of files
        else if (read1 == 0 && read2 == 0) {
            close(file1);
            close(file2);
            return ret;
        }
            //if only one file reached end of file
        else if (read1 == 0 || read2 == 0) {
            ret = 2;
            continue;
        }
        while (i < read1 && j < read2) {
            if (buf1[i] == buf2[j]){
                ++i;
                ++j;
                // the files are not the same
            } else {
                ret = 2;
                break;
            }
        }
    }
    // check the file for similarity
    while (1){
        //finished comparing the first file buffer (contained white spaces)
        if (i == read1) {
            i = 0;
            read1 = (int) read(file1, buf1, SIZE);
        }
        // finished comparing the second file buffer (contained white spaces)
        if (j == read2) {
            j = 0;
            read2 = (int) read(file2, buf2, SIZE);
        }
        if (read1 == -1 || read2 == -1){
            perror(ERROR);
            close(file1);
            close(file2);
            return 0;
        }
        //check for end of file
        if (read1 == 0 && read2 == 0){
            close(file1);
            close(file2);
            return 2;
        }
            //check for end of file of the first, and the other file only has white spaces
        else if (read1 == 0) {
            while (j < read2) {
                //if there are spaces, continue checking
                if (isspace(buf2[j])) {
                    ++j;
                }
                    //otherwise, the files are equal
                else {
                    return 3;
                }
            }
        }
            //if we finished reading the second file
        else if (read2 == 0) {
            while (i < read1) {
                if (isspace(buf1[i])) {
                    ++i;
                }
                else {
                    return 3;
                }
            }
        }
        // check for similarity
        while(i < read1 && j < read2) {
            //equal chars
            if (buf1[i] == buf2[j]) {
                ++i;
                ++j;
            } else if (isalpha(buf1[i]) && isalpha(buf2[j])){
                if (toupper(buf1[i]) == buf2[j] || tolower(buf1[i]) == buf2[j]){
                    ++i;
                    ++j;
                } else {
                    close(file1);
                    close(file2);
                    return 3;
                }
                //white space in one of the buffers
            } else if(isspace(buf1[i])){
                ++i;
            } else if(isspace(buf2[j])){
                ++j;
            } else {
                close(file1);
                close(file2);
                return 3;
            }
        }
    }
}
