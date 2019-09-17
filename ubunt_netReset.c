#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 1024

typedef struct real_Time{
	FILE *real_time;
	char r_time[100];
}real_Time;

void linux_time(real_Time *tm){
	int n;
	tm->real_time = popen("date", "r");

	while(fgets(tm->r_time, 100, tm->real_time) != NULL);
	n = strlen(tm->r_time);
	tm->r_time[n - 1] = '\0';
	strcat(tm->r_time, ". network loss...\n");
}

int main(){
    char buff[BUFF_SIZE];
    char ip_addr[100] = {'x', };
	char *ptr;
    FILE *fp, *txt;
	real_Time tm;
	

	if(NULL == (txt = fopen("log.txt", "a+"))){
		printf("fopen error!!\n");
		return -1;
	}
	

    while(1){
        if(NULL == (fp = popen("ip route", "r"))){
            perror("popen error!!");
            return -1;
        }


        while(fgets(buff, BUFF_SIZE, fp)){		//gateway address get it
            ptr = strtok(buff, " ");
    
            if(ip_addr[0] != 'x')
                break;
    
            while(1){
                ptr = strtok(NULL, " ");
    
                if(0 == (strcmp("via", ptr))){
                    ptr = strtok(NULL, " ");
                    strcpy(ip_addr, ptr);
                    break;
                }
            }//while() 
        }//while()
    
        if(ip_addr[0] == 'x'){
            printf("network restart....\n");
	    	system("service network-manager restart");
			linux_time(&tm);
			fprintf(txt,"%s", tm.r_time);
			fflush(txt);
			memset(tm.r_time, 0, 100);
			pclose(tm.real_time);
        }

        sleep(60);
        memset(ip_addr, 0, 100);
        ip_addr[0] = 'x';
        pclose(fp);
    }
	fclose(txt);

    return 0;
}

