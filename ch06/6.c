// test system call time cost and context switch

#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/sysctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// port sched_getaffinity
#define SYSCTL_CORE_COUNT   "machdep.cpu.core_count"

typedef struct cpu_set {
  uint32_t    count;
} cpu_set_t;

static inline void
CPU_ZERO(cpu_set_t *cs) { cs->count = 0; }

static inline void
CPU_SET(int num, cpu_set_t *cs) { cs->count |= (1 << num); }

static inline int
CPU_ISSET(int num, cpu_set_t *cs) { return (cs->count & (1 << num)); }

int sched_getaffinity(pid_t pid, size_t cpu_size, cpu_set_t *cpu_set)
{
  int32_t core_count = 0;
  size_t  len = sizeof(core_count);
  int ret = sysctlbyname(SYSCTL_CORE_COUNT, &core_count, &len, 0, 0);
  if (ret) {
    printf("error while get core count %d\n", ret);
    return -1;
  }
  cpu_set->count = 0;
  for (int i = 0; i < core_count; i++) {
    cpu_set->count |= (1 << i);
  }

  return 0;
}


void test_sys_call(int n) {
    int i;
    struct timeval start, end;
    int f = open("6.c", O_RDONLY);
    gettimeofday(&start, NULL);
    for (i = 0;i < n; ++i) {
        read(f, NULL, 0); // This is a system call without additional time cost
    }
    gettimeofday(&end, NULL);
    close(f);
    float delta_us = (end.tv_sec - start.tv_sec) * 1e6
        + (end.tv_usec - start.tv_usec);
    printf("system call:\n");
    printf("%d time\ntotal: %fus\navg cost: %fus\n", n, delta_us, delta_us / n);
}

void test_ctx_swx(int n) {
    int ab_fd[2], ba_fd[2];
    pipe(ab_fd);
    pipe(ba_fd);
    char *str = "123";
    cpu_set_t mask;
    CPU_SET(7, &mask);
    int rc = fork();
    sched_getaffinity(rc, sizeof(mask), &mask);
    if (rc == 0) {
        close(ab_fd[1]);
        close(ba_fd[0]);
        int i;
        char buf[1024];
        for (i = 0;i < n; ++i) {
            read(ab_fd[0], buf, sizeof(buf));
            write(ba_fd[1], buf, strlen(buf));
        }
    }
    else {
        close(ab_fd[0]);
        close(ba_fd[1]);
        int i;
        char buf[1024];
        struct timeval start, end;
        float delta_us = 0;
        for (i = 0;i < n; ++i) {
            write(ab_fd[1], str, strlen(str));
            gettimeofday(&start, NULL);
            read(ba_fd[0], buf, sizeof(buf));
            gettimeofday(&end, NULL);
            delta_us += (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec);
        }
        wait(NULL);
        printf("context switch:\n");
        printf("%d times\ntotal: %fus\navg cost: %fus\n", n, delta_us, delta_us / n);
    }
}

int main(int argc, char *argv[])
{
    int N = 100000;
    test_sys_call(N);
    test_ctx_swx(N);
    return 0;
}
