#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>

char buffer[2045];
int version=1;

void copy(old, new)
int old, new;
{
    int count;
    while((count=read(old,buffer,sizeof(buffer)))>0)
        write(new,buffer,count);
}

int main(argc,argv)
int argc; char * argv[];
{
    int fdold, fdnew;
    if(argc!=3){
        printf("Need 2 arguments for copy programm");
        exit(1);
    }

    fdold = open(argv[1],O_RDONLY);
    if(fdold==-1){
        printf("Cannot open file %s\n",argv[1]);
        exit(1);
    }

    fdnew = creat(argv[2],0666); // create a target file
    if(fdnew==-1){
        printf("Cannot create file %s \n",argv[2]);
        exit(1);
    }

    copy(fdold,fdnew);
    exit(0);
}

