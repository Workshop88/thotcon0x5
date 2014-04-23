/* Daemon for Raspberry Pi Thotcon
 *  - receive notification from udev rule of USB add/removes
 *  - for USB adds, forward to UUGear daemon to id the board
 *  - query central server for board id - if unknown, put up id screen
 *  - keep list of currently connected people
 *  - accumulate connection time for each board detected
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <time.h>
#include <mqueue.h>
#include <dirent.h>

#define MAX_MSG_SIZE 128

int main(int argc, char **argv)
{
  /* open mqueue for udev.rule program */
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = MAX_MSG_SIZE;
  attr.mq_curmsgs = 0;
  mqd_t udev_mq = mq_open("/thotcon0x5", O_CREAT | O_RDONLY, 0666, &attr);

  while(1) {
    // read 1 message from queue
    bytes_read = mq_receive (mq, buffer, MAX_MSG_SIZE, NULL);

    // print the messsage
    printf("Msg: %s", buffer);
  }
}
