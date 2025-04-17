# include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>


char* search_path()
{
    int i = 0;

    char *path = getenv("PATH");
    if (!path) return NULL;
    
    char **split_path = ft_split(path, ':');
    while (split_path[i]){
        printf("%s \n", split_path[i++]);
    }
    return NULL;
}

int main(){
    search_path();
}