#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h> 
#include <string.h>
int processor(FILE *fp, char *dest, FILE *processed) {
    char str[50];
    char include[] = "#include \"\\";
    while(fgets(str, 50, fp)) {
        if(strncmp(include, str, strlen(include)-1) == 0) {
            char *temp = strchr(str, '"') + 1;
            char filename[strlen(temp)-1];
            strncpy(filename, temp, strlen(temp)-2);
            filename[strlen(temp)-2]='\0';
            FILE *af = fopen(filename, "r");
            if(af == NULL) {
                fprintf(stdout, "Error: %s does not exist.\n", filename);
                return 0;
            }
            int check = processor(af, dest, processed);
            if(!check) {
                return 0;
            }
            fclose(af);
        }
        else{
            fputs(str, processed);
        }
    }
    return 1;
}
int main(int argc, char** argv) {
    if(argc == 1) {
        fprintf(stderr, "Error: no file inputted. \n");
        return EXIT_FAILURE;
    }
    for(int i = 1; i < argc; i++){
        FILE *fp;
        char *file = *(argv+i);
        char *cchecker = strchr(file, '.');
        if(strncmp(cchecker,".c", 2) !=0) {
            fprintf(stderr, "Error: invalid file %s.\n", file);
            return EXIT_FAILURE;
        }
        fp = fopen(file, "r");
        if(fp == NULL) {
            fclose(fp);
            fprintf(stderr, "Error: %s does not exist.\n", file);
            return EXIT_FAILURE;
        }
        file[strlen(file)-1] = 'i';
        FILE *processed = fopen(file, "w");
        int yay = processor(fp, file, processed);
        if(!yay) {
            return EXIT_FAILURE;
        }
        fclose(fp);
        fclose(processed);
    }
}
