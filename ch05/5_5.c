#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <err.h>

int main(int argc, char *argv[])
{
    printf("wait %d\n", wait(NULL));
    int rc = fork();
    if (rc == 0) {
        printf("wait %d\n", wait(NULL));
        err(errno, NULL);
    }
    else {
        wait(NULL);
    }
    return 0;
}
