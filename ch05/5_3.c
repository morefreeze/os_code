#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int rc = fork();
    if (rc == 0) {
        printf("hello\n");
    }
    else {
        int i;
        for (i = 0; i < 1000000; ++i);
        printf("goodbye\n");
    }
    return 0;
}
