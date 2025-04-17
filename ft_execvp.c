/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eljamaaouyayman <eljamaaouyayman@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 08:43:07 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/04/09 01:36:40 by eljamaaouya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_executable(const char *path)
{
    struct stat st;
    return (stat(path, &st) == 0 && (st.st_mode & S_IXUSR));
}

char* search_path(const char *file)
{
    int i = 0;
    char *path = getenv("PATH");
    if (!path) return NULL;
    char *path_copy = strdup(path);
    char *full_path = NULL;
    char **dir = ft_split(path_copy, ':');
    while(dir[i]){
        full_path = malloc(ft_strlen(dir[i]) + ft_strlen(file) + 2);
        full_path = ft_strjoin(dir[i], "/");
        full_path = ft_strjoin(full_path, file);
        if (is_executable(full_path))
        {
            free(path_copy);
            return full_path;
        }
        free(full_path);
    }
    free(path_copy);
    return NULL;
}


// execve wrapper that works like execvp
int execve_like_execvp(const char *file, char *const argv[])
{
    char *full_path;
    char *path;

    path = getenv("PATH");
    full_path = NULL;
    if (!path)
    {
        errno = ENOENT;
        return -1;
    }
    char *path_copy = strdup(path);
    char **environ = ft_split(path_copy, ':');
    if (strchr(file, '/'))
        return execve(file, argv, environ);
    full_path = search_path(file);
    if (!full_path)
    {
        errno = ENOENT;
        return -1;
    }
    int result = execve(full_path, argv, environ);
    free(full_path);
    return result;
}
