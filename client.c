#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#define THREAD_MAX 1000

void *cns(void *msg)
{
    int fd = 0;
    char rcvBuf[128] = "";
    struct sockaddr_in info;
    struct timespec tstart, tend;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        printf("Socket create failed.");
        exit(0);
    }

    bzero(&info, sizeof(info));
    info.sin_family = PF_INET;
    info.sin_addr.s_addr = inet_addr("127.0.0.1");
    info.sin_port = htons(12345);

    int err = connect(fd, (struct sockaddr *) &info, sizeof(info));
    if (err == -1) {
        printf("Connect error");
        exit(0);
    }

    clock_gettime(CLOCK_REALTIME, &tstart);
    send(fd, (char *) msg, strlen((char *) msg), 0);
    recv(fd, rcvBuf, sizeof(rcvBuf), 0);
    clock_gettime(CLOCK_REALTIME, &tend);

    close(fd);
    // printf("recv: %s\n", rcvBuf);
    return (void *) ((tend.tv_sec - tstart.tv_sec) * 1000000000 +
                     (tend.tv_nsec - tstart.tv_nsec));
}

int main()
{
    FILE *avg = fopen("output/Average", "w");
    FILE *worst = fopen("output/Worst", "w");
    const char msg[32] = "Message";
    for (int th = 1; th <= THREAD_MAX; ++th) {
        printf("Now progress: %d\n", th);
        long long total = 0;

        pthread_t t[th];
        for (int i = 0; i < th; ++i) {
            pthread_create(&t[i], NULL, cns, (void *) msg);
        }

        long long ret;
        long long wst = -1;
        for (int i = 0; i < th; ++i) {
            pthread_join(t[i], (void *) &ret);
            total += ret;
            wst = wst > ret ? wst : ret;
        }
        fprintf(avg, "%d %lf\n", th, total / (double) th / 1000000000);
        fprintf(worst, "%d %lf\n", th, wst / 1000000000.0);
    }
    fclose(avg);
    fclose(worst);

    return 0;
}
