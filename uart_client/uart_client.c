#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>

int ifd;
int ofd;

struct termios oldt;
struct termios newt;

void cleanup()
{
    pthread_exit(0);

    printf("==============================\n");
    printf("enable console echo\n");
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    close(ifd);
    close(ofd);
}

void* input_thread(void* nothing)
{
    char c;

    while(1)
    {
        c = (char)getchar();
        write(ofd, &c, 1);
    }
}

int main(int argc, char* argv[])
{
    char c;
    int count;
    pthread_t my_thread;
    char fifo_name_in[128];
    char fifo_name_out[128];

    strcpy(fifo_name_in, argv[1]);
    strcat(fifo_name_in, ".out.fifo");
    strcpy(fifo_name_out, argv[1]);
    strcat(fifo_name_out, ".in.fifo");

    mknod(fifo_name_out, S_IFIFO | 0666, 0);

    printf("open fifo for write: %s\n", fifo_name_out);
    ofd = open(fifo_name_out, O_WRONLY);
    if(ofd < 0)
    {
        printf("open %s error", fifo_name_out);
    }

    mknod(fifo_name_in, S_IFIFO | 0666, 0);

    printf("open fifo for read: %s\n", fifo_name_in);
    ifd = open(fifo_name_in, O_RDONLY);
    if(ifd < 0)
    {
        printf("open %s error", fifo_name_in);
    }

    printf("disable console echo\n");
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSADRAIN, &newt);

    pthread_create(&my_thread, NULL, input_thread, (void*)0);

    atexit(cleanup);

    printf("==============================\n");
    while(1)
    {
        count = read(ifd, &c, 1);
        if(count > 0)
        {
            fputc(c, stderr);
        }
        else
        {
            close(ifd);
            ifd = open(fifo_name_in, O_RDONLY);
        }
    }

    return 0;
}

