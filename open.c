#include<stdio.h>
#include <stdlib.h>
#include<string.h>
int main(){
    char *fname = "Daaaki";
    char *sname = "Richard";
    char fullname[100];
    sprintf(fullname,"%s %s",fname,sname);
    puts(fullname);
}