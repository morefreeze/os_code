#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <err.h>
#include <string.h>

void e(int exe_ret) {
    if (exe_ret < 0) {
        printf("exec error return %d errno %s\n", exe_ret, strerror(errno));
        err(errno, NULL);
    }
}

int main(int argc, char *argv[])
{
    char *myargs[2];
    myargs[0] = "/bin/ls";
    myargs[1] = NULL;
    typedef int (*f)(const char *, const char *, ...);
    f func[] = {execl, execlp};
    int i;
    for (i = 0; i < sizeof(func) / sizeof(f); ++i) {
        int rc = fork();
        if (rc == 0) {
            printf("exec[%d]:\n", i);
            e(func[i](myargs[0], myargs[0], myargs[1]));
            exit(0);
        }
        else {
            int rc_wait = wait(NULL);
            printf("rc_wait is %d\n", rc_wait);
        }
    }
    typedef int (*fv)(const char *, char *const[]);
    fv funcv[] = {execv, execvp};
    for (i = 0; i < sizeof(funcv) / sizeof(fv); ++i) {
        int rc = fork();
        if (rc == 0) {
            printf("execv[%d]:\n", i);
            e(funcv[i](myargs[0], myargs));
            exit(0);
        }
        else {
            int rc_wait = wait(NULL);
            printf("rc_wait is %d\n", rc_wait);
        }
    }

    {
        int rc = fork();
        if (rc == 0) {
            printf("execle:");
            e(execle(myargs[0], myargs[0], myargs[1], NULL));
            exit(0);
        }
        else {
            int rc_wait = wait(NULL);
            printf("rc_wait is %d\n", rc_wait);
        }
    }

    return 0;
}
