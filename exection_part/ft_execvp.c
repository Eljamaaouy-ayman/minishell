/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 08:43:07 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/04/19 18:01:20 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int is_executable(const char *path)
{
    struct stat st;
    return (stat(path, &st) == 0 && (st.st_mode & S_IXUSR));
}

char* search_path(const char *file)
{
    int i = 0;
    char *path = getenv("PATH");
    if (!path)
        return NULL;
    char *path_copy = ft_strdup(path);
    char *full_path = NULL;
    char **dir = ft_split1(path_copy, ':');
    while(dir[i])
    {
        full_path = malloc(ft_strlen(dir[i]) + ft_strlen(file) + 2);
        full_path = ft_strjoin(dir[i], "/");
        full_path = ft_strjoin(full_path, file);
        if (is_executable(full_path))
        {
            free(path_copy);
            return full_path;
        }
        free(full_path);
        i++;
    }
    free(path_copy);
    return NULL;
}


// execve wrapper that works like execvp
int execve_like_execvp(const char *file, char *const argv[])
{
    char *full_path;
    char *path;

    printf("hh");
    path = getenv("PATH");
    full_path = NULL;
    if (!path)
    {
        errno = ENOENT;
        return -1;
    }
    char *path_copy = ft_strdup(path);
    char **environ = ft_split1(path_copy, ':');
    if (ft_strchr(file, '/'))
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
