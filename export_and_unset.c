/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_and_unset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eljamaaouyayman <eljamaaouyayman@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:20:21 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/04/01 13:36:02 by eljamaaouya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void remove_env_var(char ***env, const char *name)
{
    char **new_env;
    int (i), (j), (name_len), (count), (found_index);
    name_len = strlen(name);
    count = 0;
    found_index = -1;
    while ((*env)[count])
    {
        if (strncmp((*env)[count], name, name_len) == 0 && 
        (*env)[count][name_len] == '=')
            found_index = count;
        count++;
    }
    if (found_index == -1)
        return;
    new_env = malloc(count * sizeof(char *));
    if (!new_env)
        return;
    i = 0;
    j = 0;
    while (i < count)
    {
        if (i != found_index)
            new_env[j++] = (*env)[i];
        else
            free((*env)[i]);
        i++;
    }
    new_env[j] = NULL;
    free(*env);
    *env = new_env;
}

void ft_unset(char **args, char ***env)
{
    if (!args[1])
    {
        write(2, "unset: not enough arguments\n", 27);
        return;
    }
    for (int i = 1; args[i]; i++)
    {
        if (strchr(args[i], '='))
        {
            printf("unset: `%s': not a valid identifier\n", args[i]);
            continue;
        }
        remove_env_var(env, args[i]);
    }
}
