#include "apue.h"
#include "sys/times.h"
#include "sys/stat.h"
#include "fcntl.h"
#define MAX_BUFFSIZE 131072
#define INIT_BUFFSIZE 256

int
main(int argc, char *argv[])
{
    struct tms tmp;
    long long CLOCKS_PER_SEC =  sysconf(_SC_CLK_TCK);
    int    outfile,infile;    
    long   cur_buffsize, input_length,  n, loops;
    char   buf[MAX_BUFFSIZE];
    char *tmp_buf = NULL;//temp buff
    
    if(argv[2] == "SYNC")     outfile = open(argv[1], O_RDWR|O_CREAT|O_SYNC, S_IRUSR|S_IWUSR);
       else                   outfile = open(argv[1], O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
     
    input_length = lseek(STDIN_FILENO, 0, SEEK_END);
    printf("input_length = %ld\n",input_length);
    
    tmp_buf = (char*)malloc(input_length);
    
    lseek(STDIN_FILENO, 0, SEEK_SET);
    if(read(STDIN_FILENO, tmp_buf, input_length) != input_length) 
                 err_sys("read error~~");//can be sure it is read into tmp_buf
    
    infile = open(tmp_buf, O_RDONLY);//try to open tmp_buf like a file, ok?? //can be sure it is not ok.


    printf("BUFFSIZE\tUSER CPU\tSYSTEM CPU\tCLOCK TIME\tLOOPS\n");
    for(cur_buffsize = INIT_BUFFSIZE; cur_buffsize <= MAX_BUFFSIZE; cur_buffsize *= 2)
    {
    	clock_t begin = times(&tmp);
    	clock_t begin_utime = tmp.tms_utime;
    	clock_t begin_stime = tmp.tms_stime;
        loops = 0;

        printf("infile_length = %ld\n",lseek(infile, 0, SEEK_END));


    	while ((n = read(infile, buf, cur_buffsize)) > 0) 
    	{
                lseek(outfile, 0, SEEK_SET);//set head position in outfile to write
                if(write(outfile, buf, n) != n) err_sys("write error");       
        	loops++;
    	}
        printf("n= %d\n",n);
    	if (n < 0)
        	err_sys("read error!!!");
    
    	clock_t end = times(&tmp);
    	clock_t end_utime = tmp.tms_utime; 
    	clock_t end_stime = tmp.tms_stime;
   	printf("%ld\t\t",cur_buffsize);
    	printf("%.2lf\t\t",(double)(end_utime - begin_utime)/CLOCKS_PER_SEC ); 
    	printf("%.2lf\t\t",(double)(end_stime - begin_stime)/CLOCKS_PER_SEC );
    	printf("%.2lf\t\t",(double)(end - begin)/CLOCKS_PER_SEC );
    	printf("%ld\n",loops);
    }
    exit(0);
}
