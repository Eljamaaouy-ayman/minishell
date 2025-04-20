/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_and_unset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eljamaaouyayman <eljamaaouyayman@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:20:21 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/04/03 20:48:52 by eljamaaouya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

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

static char *find_env_var(char **env, const char *name) {
    size_t name_len = ft_strlen(name);
    for (int i = 0; env[i]; i++) {
        if (ft_strncmp(env[i], name, name_len) == 0 && env[i][name_len] == '=')
            return env[i];
    }
    return NULL;
}

// Helper to add/replace an env var
static void set_env_var(char ***env, const char *name_value)
{
    char (**new_env), (*name), (*eq);
    int (count), (i);
    count = 0;
    eq = strchr(name_value, '=');
    if (eq)
        name = ft_strndup(name_value, eq - name_value);
    char *existing = find_env_var(*env, name);
    if (existing)
    {
        printf("existing: %s\n", existing);
        free(existing);
        existing = ft_strdup(name_value);
        return;
    }
    while ((*env)[count])
        count++;
    new_env = malloc((count + 2) * sizeof(char *));
    if (!new_env)
        return;
    i = -1;
    while(++i < count)
        new_env[i] = (*env)[i];
    new_env[count] = ft_strdup(name_value);
    new_env[count + 1] = NULL;
    // free(*env);
    *env = new_env;
}

// Main export function
void ft_export(char **args, char ***env)
{
    if (!args[1]) {
        for (int i = 0; (*env)[i]; i++) {
            write(STDOUT_FILENO, (*env)[i], strlen((*env)[i]));
            write(STDOUT_FILENO, "\n", 1);
        }
        return;
    }

    for (int i = 1; args[i]; i++) {
        if (strchr(args[i], '=') && strchr(args[i], ' ') == NULL)
            set_env_var(env, args[i]);
        else
        {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "export: `%s': not a valid identifier\n", args[i]);
            write(STDERR_FILENO, error_msg, strlen(error_msg));
        }
    }
}
