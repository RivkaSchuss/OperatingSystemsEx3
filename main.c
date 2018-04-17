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
    int charsr1 = 0, charsr2 = 0, ret = 1, i = 0, j = 0;
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
        charsr1 = (int)read(file1, buf1, SIZE);
        charsr2 = (int)read(file2, buf2, SIZE);
        //check for reading error
        if (charsr1 == -1 || charsr2 == -1){
            perror(ERROR);
            close(file1);
            close(file2);
            return 0;
        }
        //check for files size identity
        if (charsr1 != charsr2) {
            ret = 2;
            continue;
        }
            //check for end of files
        else if (charsr1 == 0 && charsr2 == 0) {
            close(file1);
            close(file2);
            return ret;
        }
            //if only one file reached end of file
        else if (charsr1 == 0 || charsr2 == 0) {
            ret = 2;
            continue;
        }
        while (i < charsr1 && j < charsr2) {
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
    // check rhe file for similarity
    while (1){
        //finished comparing the first file buffer (contained white spaces)
        if (i == charsr1) {
            i = 0;
            charsr1 = (int) read(file1, buf1, SIZE);
        }
        // finished comparing the second file buffer (contained white spaces)
        if (j == charsr2) {
            j = 0;
            charsr2 = (int) read(file2, buf2, SIZE);
        }
        if (charsr1 == -1 || charsr2 == -1){
            perror(ERROR);
            close(file1);
            close(file2);
            return 0;
        }
        //check for end of file
        if (charsr1 == 0 && charsr2 == 0){
            close(file1);
            close(file2);
            return 2;
        }
            /*check for end of file of only one file
             * and the other file has only white spaces left*/
        else if (charsr1 == 0) {
            while(j < charsr2){
                if (isspace(buf2[j])) {
                    ++j;
                }
                else {
                    return 3;
                }
            }
        }
        else if(charsr2 == 0) {
            while(i < charsr1){
                if (isspace(buf1[i])) {
                    ++i;
                }
                else {
                    return 3;
                }
            }
        }
        // check for similarity
        while(i < charsr1 && j < charsr2) {
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
                //not similar at all
            } else {
                close(file1);
                close(file2);
                return 3;
            }
        }
    }
}
