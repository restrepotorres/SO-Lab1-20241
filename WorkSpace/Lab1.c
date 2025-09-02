#include <stdio.h>


int main(int argc, char *argv[]){
    int x = 1;
    //printf(argv[1]);

    FILE *fp;
    char buffer[100];
    fp = fopen(argv[1], "r");
    int i = 1;
    while ( fgets(buffer,i,fp) ) {
        printf("%s", buffer);
        i = i+1;
    }
    fclose(fp);
    return 0;
}
