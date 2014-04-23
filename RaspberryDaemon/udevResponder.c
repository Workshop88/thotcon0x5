/* Udev responder
  exec'd by udev.rule: send a message to Thotcon Daemon with
  /dev/ttyACM number, and action add/remove
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <mqueue.h>

#define MSGSZ     128
/*
 * Declare the message structure.
 */

typedef struct msgbuf {
         long    mtype;
         char    mtext[MSGSZ];
    } message_buf;

main()
{
  message_buf sbuf;
  int buf_length;
  int sendResult;
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = MSGSZ;
  attr.mq_curmsgs = 0;
  mqd_t mq = mq_open( "/thotcon0x5", O_CREAT | O_WRONLY, 0666, &attr);

  sbuf.mtype = 1;
  buf_length = snprintf(sbuf.mtext, MSGSZ, "%s\n%s", getenv("DEVNAME"), getenv("ACTION")) + 1;

  sendResult = mq_send( mq, (char*)&sbuf, buf_length, 0);
  exit(sendResult);
}
