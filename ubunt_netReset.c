#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFF_SIZE 1024

int main(){
    char buff[BUFF_SIZE];
    char ip_addr[100] = {'x', };
    FILE *fp;
    char *ptr;

    while(1){
        if(NULL == (fp = popen("ip route", "r"))){
            perror("popen error!!");
            return -1;
        }


        while(fgets(buff, BUFF_SIZE, fp)){
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
        }
        sleep(60);
        memset(ip_addr, 0, 100);
        ip_addr[0] = 'x';
        pclose(fp);
    }

    return 0;
}

