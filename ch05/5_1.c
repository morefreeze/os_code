#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int x = 100;
    int rc = fork();
    if (rc == 0) {
        printf("child x is %d\n", x);
        x = 42;
        printf("child after x is %d\n", x);
    }
    else {
        printf("parent x is %d\n", x);
        x = 24;
        printf("parent after x is %d\n", x);
    }
    return 0;
}
