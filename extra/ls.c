# include <stdio.h>
# include <dirent.h> 
# include <sys/stat.h>
# include <unistd.h>

int ls(char *name){

    DIR *d;
    d = opendir(name);
    if (! d){
        perror("opendir");
        return -1;
    } 
    
    while (1){
        struct dirent *de;
        de = readdir(d);
        if (! de){
            break;
        }
        else {
            printf("%s\n", de->d_name);
        }
    }

    closedir(d);
    return 0;
};



int main(int argc, char* argv[]){
int i= 0; 
if (argc == 1){
    ls(".");
} else {
    for (i = 0; i < argc; i++){
        ls(argv[i]);
    }
}
}
