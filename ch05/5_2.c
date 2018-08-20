#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int f = open("5_2.txt", O_WRONLY | O_CREAT);
    int rc = fork();
    if (rc == 0) {
        char *str = "this is child\n";
        write(f, str, strlen(str));
    }
    else {
        char *str = "this is parent\n";
        write(f, str, strlen(str));
    }
    return 0;
}
