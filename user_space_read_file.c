#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>


#define EP_MAX_BUF  64


int main(int argc, char *argv[] ) {

 int fd;
 int nb;
 uint8_t *buffer ;
 int eno ;
 uint8_t colr, colb, colg;

 if ( argc == 4 ) {
   sscanf(argv[1], "%hhu", &colr);
   printf("colr %u\n", colr);
   sscanf(argv[2], "%hhu", &colg);
   printf("colg %u\n", colg);
   sscanf(argv[3], "%hhu", &colb);
   printf("colb %u\n", colb);
 } else {
  printf ("give correct args\n");
  return -1 ;
 }

 buffer = (uint8_t *) malloc(EP_MAX_BUF);

 fd = open("/dev/mydev0", O_RDWR);
 eno = errno;

 if ( fd < 0 ) {
   printf("Error: fd=%d  error:%d  mesg: %s \n", fd, eno, strerror(eno) );
   exit(0);
 }

 if ( 1 == 1 ) {
    memset(buffer, 0, EP_MAX_BUF );
    lseek(fd, 0, 0);
    nb  = read(fd, buffer, 64);
    printf("Bytes read: %d\n", nb);
    for ( int i=0; i < nb; i++ ) printf("%02X ", buffer[i] );
    printf("\n");
 }

 if ( 1 == 1 ) {
    memset(buffer, 0, EP_MAX_BUF );
    buffer[0] = colr; buffer[1] = colg; buffer[2] = colb; buffer[3] = 0; 
    nb  = write(fd, buffer, 4);
    //nb = fwrite(buffer, 1, 2, fdopen(fd, "rw") );

    printf("Bytes written: %d\nBytes: %s\n", nb, buffer);
 }


 close(fd);


}
