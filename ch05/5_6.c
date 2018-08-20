#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <err.h>

int main(int argc, char *argv[])
{
    int rc = fork();
    if (rc == 0) {
        printf("child\n");
    }
    else {
        waitpid(rc, NULL, WNOHANG);
    }
    return 0;
}
