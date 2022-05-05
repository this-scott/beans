#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <cstring>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>


#include <openssl/md5.h>

using namespace std;

unsigned char result[MD5_DIGEST_LENGTH];

string exec(string command)
{
    char buffer[128];
    string result = "";

    // Open pipe to file
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe)
    {
        return "popen failed!";
    }

    // read till end of process:
    while (!feof(pipe))
    {
        // use buffer to read and add to result
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    pclose(pipe);
    return result;
}

string getSum(string theFile) {
    return exec("md5sum " + theFile);
}

void print_md5_sum(unsigned char* md) {
    int i;
    for(i=0; i <MD5_DIGEST_LENGTH; i++) {
            printf("%02x",md[i]);
    }
}

void write_md5_sum(unsigned char* md, char* writeFile, int recursionCounter) {
    int i;
    char awriteFile[] = "awritefile";
    FILE *fptr = fopen("awritefile.txt", "w");
    for(i=0; i <MD5_DIGEST_LENGTH; i++) {
            //TODO: FIX MD5 SUMS FROM WRITING OVER EACH OTHER
            //TODO: ADD FILE NAMES ON SAME LINE AS SUMS
            fprintf(fptr, "\n%02x",md[i]);
    }
}

unsigned long get_size_by_fd(int fd) {
    struct stat statbuf;
    if(fstat(fd, &statbuf) < 0) exit(-1);
    return statbuf.st_size;
}

/*unsigned char* getSum2(char* file) {

    int file_descript;
    unsigned long file_size;
    char* file_buffer;

    file_descript = open(file, O_RDONLY);
    if(file_descript<0) exit(-1);

    file_size = get_size_by_fd(file_descript);

    file_buffer = (char*)mmap(0, file_size, PROT_READ, MAP_SHARED, file_descript, 0);
    MD5((unsigned char*) file_buffer, file_size, result);
    munmap(file_buffer, file_size);

    return result;
}*/

void getSum2(char* file, char* writeFile) {

    //gets md5 sum
    int file_descript;
    unsigned long file_size;
    char* file_buffer;

    file_descript = open(file, O_RDONLY);
    if(file_descript<0) exit(-1);

    file_size = get_size_by_fd(file_descript);

    file_buffer = (char*)mmap(0, file_size, PROT_READ, MAP_SHARED, file_descript, 0);
    MD5((unsigned char*) file_buffer, file_size, result);
    munmap(file_buffer, file_size);

    //writes md5 sum to writefile
    int i;
    FILE *fptr = fopen(writeFile, "w");
    for(i=0; i <MD5_DIGEST_LENGTH; i++) {
            fprintf(fptr, "%02x",result[i]);
    }
    fclose(fptr);
}

void printSums(char* workingDir, int depth, char* writeFile) {
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(workingDir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", workingDir);
        return;
    }
    chdir(workingDir);
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            /* Found a directory, but ignore . and .. */
            if(strcmp(".",entry->d_name) == 0 ||
                strcmp("..",entry->d_name) == 0)
                continue;
            printf("%*s%s/\n",depth,"",entry->d_name);
            /* Recurse at a new indent level */
            printSums(entry->d_name,depth+4, writeFile);
        } 
        else {
            printf("%*s%s\n",depth,"",entry->d_name);
            //get sum goes here
            getSum2(entry->d_name, writeFile);
        }
    }
    chdir("..");
    closedir(dp);
}

int main(int argc, char** argv)
{

    char cwd[256];
    char* workingDir;
    //TODO: fix multiple write files bug
    //checks for specified folder, in none use current directory
    char* writeFile = (char*)"writefile.txt";
    if(argv[1] == NULL) {
        workingDir = getcwd(cwd, sizeof(cwd));
    } else {
        workingDir = argv[1];
    }

    if(getcwd(cwd, sizeof(cwd)) == NULL)
        perror("getcwd error");
    else
        printf("current working directory is %s\n", workingDir);

    /*dirp = opendir(workingDir);
    if( dirp == NULL ) {
        perror( "can't open" );
    } else {
        for(;;) {

            direntp = readdir( dirp );
            if( direntp == NULL ) break;

            printf( "%s\n", direntp->d_name );
            cout << getSum(direntp->d_name);
        }
        
        closedir( dirp );
    }*/

    printSums(workingDir, 0, writeFile);

    return 0;
}