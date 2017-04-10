#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define BUFFSIZE (64*1024)
typedef unsigned long long jiff;

void getstat(jiff *__restrict cuse, jiff *__restrict cice, jiff *__restrict csys, jiff *__restrict cide, jiff *__restrict ciow, jiff *__restrict cxxx, jiff *__restrict cyyy, jiff *__restrict czzz);


int main(){

    while(1){
	    jiff  cuse;
	    jiff  cice;
	    jiff  csys;
	    jiff  cide;
	    jiff  ciow;
	    jiff  cxxx;
	    jiff  cyyy;
	    jiff  czzz;

	    getstat(&cuse,
	                &cice,
	                &csys,
	                &cide,
	                &ciow,
	                &cxxx,
	                &cyyy,
	                &czzz);
    }
}


void getstat(jiff *restrict cuse, jiff *restrict cice, jiff *restrict csys, jiff *restrict cide, jiff *restrict ciow, jiff *restrict cxxx, jiff *restrict cyyy, jiff *restrict czzz)
{

    static int fd;
    static char buff[BUFFSIZE];
    buff[BUFFSIZE-1] = 0;
    char* b;

  	if(fd){
    	lseek(fd, 0L, SEEK_SET);
  	}else{
    	fd = open("/proc/stat", O_RDONLY, 0);
    	if(fd == -1) printf("/proc/stat");
  	}

    read(fd,buff,BUFFSIZE-1);
    b = strstr(buff, "cpu ");
    *ciow = 0;  /* not separated out until the 2.5.41 kernel */
  	*cxxx = 0;  /* not separated out until the 2.6.0-test4 kernel */
  	*cyyy = 0;  /* not separated out until the 2.6.0-test4 kernel */
  	*czzz = 0;  /* not separated out until the 2.6.11 kernel */
    if(b) 
    	sscanf(b,  "cpu  %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu", cuse, cice, csys, cide, ciow, cxxx, cyyy, czzz);

    fprintf(stdout, "us: %Lu", *cuse);
    usleep(1000 * 30);
}

