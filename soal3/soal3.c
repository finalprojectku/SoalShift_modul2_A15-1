#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <glob.h>
#include <fcntl.h>

// #include "zip.h"
int isFileExistsAccess(const char *path)
{
    // Check for file existence
    if (access(path, F_OK) == -1)
        return 0;

    return 1;
}

int main() {
    pid_t child_id;
    int status;
    int fd[2];

    pipe(fd);
    child_id = fork();
    
    if (child_id != 0) {
        //parent
        while(wait(&status)>0);
        child_id = fork();
        if(child_id != 0){
            while(wait(&status)>0);
            child_id = fork();
            if(child_id != 0){
                while(wait(&status)>0);
                int filefd = open("./daftar.txt", O_CREAT | O_WRONLY | O_APPEND);
                dup2(filefd, 1);
                close(fd[1]);
                close(filefd);
                
                char  test;
                while(read(fd[0],&test,1)){
                    printf("%c",test);
                }
                printf("test");
                close(fd[0]);
                // char *argv[4] = {"chmod", "777", "daftar.txt", NULL};
                // execv("/bin/chmod", argv);

            } else {
                //child 3 remove old daftar
                close(fd[1]);
                close(fd[0]);
                if(isFileExistsAccess("daftar.txt")){
                    char *argv[3] = {"rm","daftar.txt", NULL};
                    execv("/bin/rm", argv);
                }
            }

        } else {
            //child 2
            //list txt files
            close(fd[0]);
            chdir("./campur2");
            dup2(fd[1],1);
            close(fd[1]);
            glob_t globbuf;
            globbuf.gl_offs = 1;
            glob("*.txt", GLOB_DOOFFS, NULL, &globbuf);
            globbuf.gl_pathv[0] = "ls";
            execvp("ls", &globbuf.gl_pathv[0]);         
        }


    } else {
        //child 1
        //unzip
        close(fd[0]);
        close(fd[1]);
        char *argv[3] = {"unzip","campur2.zip", NULL};
        execv("/usr/bin/unzip", argv);
    }

    return 0;
}


