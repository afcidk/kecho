#ifndef ECHO_SERVER_H
#define ECHO_SERVER_H

#include <linux/types.h>
#include <net/sock.h>

struct echo_server_param {
    struct socket *listen_sock;
};
struct data_struct {
    struct work_struct work;
    void *data;
};

extern int echo_server_daemon(void *arg);

#endif
