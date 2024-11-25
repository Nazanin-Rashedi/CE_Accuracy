#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
// #include <stropts.h>
#include <sys/ioctl.h>
#include <linux/hdreg.h>  // struct hd_driveid
#include <ctype.h>        // isprint
#include <errno.h>
#include <string.h>

// device must be /dev/xxx

int
print_device_model(char* aDevice) {
  int lRc = 0;
  struct hd_driveid lHd;

  int lFd = open(aDevice, O_RDONLY | O_NONBLOCK);

  if(0 <= lFd) {
    if(!ioctl(lFd, HDIO_GET_IDENTITY, &lHd)) {
      printf("device: ");
      char* s = (char*) lHd.model;
      int   lCnt = 0;
      while(isprint(*s) && (lCnt < 40)) {
        printf("%c", *s++);
        ++lCnt;
      }
      printf("\n");
    } else {
      printf("ERROR: ioctl call failed for '%s'.\n", aDevice);
      printf("errno = %d (%s).\n", errno, strerror(errno));
      lRc = -2;
    }
    close(lFd);
  } else {
    printf("ERROR: open failed for '%s'.\n", aDevice);
    printf("errno = %d (%s).\n", errno, strerror(errno));
    lRc = -1;
  }
  return lRc;
}


int
main(int argc, char* argv[]) {
  if(2 != argc) {
    printf("usage: get_device_model <device>\n");
    exit(-1);
  }

  int lRc = print_device_model(argv[1]);
  return lRc;
}

