#include "apue.h"
#include "sys/times.h"
#include "sys/stat.h"
#include "fcntl.h"
#define MAX_BUFFSIZE 131072
#define INIT_BUFFSIZE 256

int
main(int argc, char *argv[])
{
    struct tms t_start,t_end;
    long long CLOCKS_PER_SEC =  sysconf(_SC_CLK_TCK);
    int    outfile,i;    
    long   cur_buffsize, input_length,  n;
    char *tmp_buf = NULL;//temp buff
    
    if(argv[2] == "SYNC")     outfile = open(argv[1], O_RDWR|O_CREAT|O_SYNC, S_IRUSR|S_IWUSR);
       else                   outfile = open(argv[1], O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
     
    input_length = lseek(STDIN_FILENO, 0, SEEK_END);
    tmp_buf = (char*)malloc(input_length);
    
    lseek(STDIN_FILENO, 0, SEEK_SET);
    if(read(STDIN_FILENO, tmp_buf, input_length) != input_length) //read once into temp buff
                 err_sys("read error");
    
    printf("BUFFSIZE\tUSER CPU\tSYSTEM CPU\tCLOCK TIME\tLOOPS\n");
    
    for(cur_buffsize = INIT_BUFFSIZE; cur_buffsize <= MAX_BUFFSIZE; cur_buffsize *= 2)
    {
    	clock_t start = times(&t_start);
        n = input_length/cur_buffsize;
        
        for(i=0;i<n;i++){
            if(write(outfile, tmp_buf+i*cur_buffsize, cur_buffsize)==cur_buffsize) 
               lseek(outfile, 0, SEEK_SET);
        }
        if(write(outfile, tmp_buf+n*cur_buffsize, input_length%cur_buffsize)>0)
             n++;

    	clock_t end = times(&t_end);
   	printf("%ld\t\t",cur_buffsize);
    	printf("%.2lf\t\t",(double)(t_end.tms_utime - t_start.tms_utime)/CLOCKS_PER_SEC ); 
    	printf("%.2lf\t\t",(double)(t_end.tms_stime - t_start.tms_stime)/CLOCKS_PER_SEC );
    	printf("%.2lf\t\t",(double)(end - start)/CLOCKS_PER_SEC );
    	printf("%ld\n",n);
    }
    exit(0);
}
