#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define BUFFSIZE (1024)
typedef unsigned long long jiff;

void getstat(char* core, 
	         jiff *__restrict cuse, 
	         jiff *__restrict cice, 
	         jiff *__restrict csys, 
	         jiff *__restrict cide, 
	         jiff *__restrict ciow, 
	         jiff *__restrict cxxx, 
	         jiff *__restrict cyyy, 
	         jiff *__restrict czzz);


int main(int argc, char **argv){
	jiff  cuse;
    jiff  cice;
    jiff  csys;
    jiff  cide;
    jiff  ciow;
    jiff  cxxx;
    jiff  cyyy;
    jiff  czzz;
    char *core = argv[1];

    if(core == NULL)
    	core = "0";

    getstat(core,
		&cuse,
        &cice,
        &csys,
        &cide,
        &ciow,
        &cxxx,
        &cyyy,
        &czzz);
}


void getstat(char* core, 
			 jiff *restrict cuse, 
			 jiff *restrict cice, 
			 jiff *restrict csys, 
			 jiff *restrict cide, 
			 jiff *restrict ciow, 
			 jiff *restrict cxxx, 
			 jiff *restrict cyyy, 
			 jiff *restrict czzz)
{
    static int fd;
    static char buff[BUFFSIZE];
    buff[BUFFSIZE-1] = 0;
    char* b;
    int core_num = 0;
    char prefix[6] = "cpu";
    jiff temp, diff;

    strcat(prefix, core);

    while(1){
	    if(fd){
	    	lseek(fd, 0L, SEEK_SET);
	  	}else{
	    	fd = open("/proc/stat", O_RDONLY, 0);
	    	if(fd == -1) printf("/proc/stat");
	  	}

	    read(fd, buff, BUFFSIZE - 1);

	    b = strstr(buff, prefix);
	    *ciow = 0;  /* not separated out until the 2.5.41 kernel */
	  	*cxxx = 0;  /* not separated out until the 2.6.0-test4 kernel */
	  	*cyyy = 0;  /* not separated out until the 2.6.0-test4 kernel */
	  	*czzz = 0;  /* not separated out until the 2.6.11 kernel */
	    if(b){
	    	sscanf(b,  "cpu%s  %Lu", core, cuse);
	    	// sscanf(b,  "cpu  %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu", cuse, cice, csys, cide, ciow, cxxx, cyyy, czzz);
	    }
	    	
	    diff = *cuse - temp;
	    fprintf(stdout, "%s - diff: %Lu us: %Lu\n", core, diff, *cuse);
	    temp = *cuse;
	    usleep(1000 * 10);
    }
}
