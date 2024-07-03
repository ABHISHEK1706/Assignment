#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/resource.h>
#include <bpf/libbpf.h>
#include <bpf/bpf.h>

#define MAP_PATH "/sys/fs/bpf/port_map"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }

    __u32 key = 0;
    __u16 port = atoi(argv[1]);

    int map_fd = bpf_obj_get(MAP_PATH);
    if (map_fd < 0) {
        perror("bpf_obj_get");
        return 1;
    }

    if (bpf_map_update_elem(map_fd, &key, &port, BPF_ANY) < 0) {
        perror("bpf_map_update_elem");
        close(map_fd);
        return 1;
    }

    close(map_fd);
    return 0;
}
