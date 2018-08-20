#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);
    int write_child = fork();
    if (write_child == 0) {
        close(fd[0]);
        char *str = "child 1 is speaking\n";
        write(fd[1], str, strlen(str));
        exit(0);
    }
    else {
        int read_child = fork();
        if (read_child == 0) {
            close(fd[1]);
            char buf[1024];
            read(fd[0], buf, sizeof(buf));
            printf("child 2 received: %s\n", buf);
        }
    }
    wait(NULL);
    return 0;
}
