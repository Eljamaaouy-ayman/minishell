/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eljamaaouyayman <eljamaaouyayman@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 00:24:19 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/04/01 13:39:19 by eljamaaouya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
void printbanner(void)
{
    printf("███╗   ███╗██╗   ██╗    ███████╗██╗  ██╗███████╗██╗     ██╗\n"
"████╗ ████║╚██╗ ██╔╝    ██╔════╝██║  ██║██╔════╝██║     ██║\n"  
"██╔████╔██║ ╚████╔╝     ███████╗███████║█████╗  ██║     ██║\n"
"██║╚██╔╝██║  ╚██╔╝      ╚════██║██╔══██║██╔══╝  ██║     ██║\n"
"██║ ╚═╝ ██║   ██║       ███████║██║  ██║███████╗███████╗███████╗\n"
"╚═╝     ╚═╝   ╚═╝       ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n");
}


void getworkingdir()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n", cwd);
    else
        perror("getcwd() error");
}

void getenvfunc(char **env)
{
    int i = 0;
    while(env[i])
    {
        printf("%s\n", env[i]);
        i++;
    }
}

void shell_luncher(char **cmdlist)
{
    pid_t pid;
    int status;
    pid = fork();
    if(pid == 0)
    {
        if(execvp(cmdlist[0], cmdlist) == -1)
            perror("Error");
        exit(EXIT_FAILURE);
    }
    else if(pid < 0)
    {
        perror("Error");
    }
    else
    {
        waitpid(pid, &status, 0);
    }
}

void ft_echo(char **cmdlist)
{
    bool flag = true;
    int i = 1;

    if(cmdlist[1] && ft_strcmp(cmdlist[1], "-n") == 0)
    {
        flag = false;
        i++;
    }
    while(cmdlist[i])
    {
        printf("%s ", cmdlist[i]);
        i++;
    }
    if(flag)
        printf("\n");
}

void ft_cd(char **cmdlist)
{
    // char cwd[PATH_MAX];

    if (!cmdlist[1]) {
        printf("cd: missing argument\n");
        return;
    }
    if (cmdlist[2]) {
        printf("cd: too many arguments\n");
        return;
    }
    if(ft_strcmp(cmdlist[1], "~") == 0)
        chdir(getenv("HOME"));
    else if(chdir(cmdlist[1]) != 0)
    {
        perror("Error");
    }
    
}

//================================================EXPORT================================================

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
        // Print all exported variables (alphabetically sorted)
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


int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;
    char *line;
    char **cmdlist;
    printbanner();
    while((line = readline("$>")) != NULL && ft_strcmp(line, "exit") != 0)
    {
        if(*line)
            add_history(line);
        // printf("%s\n", line);
        cmdlist = ft_split(line, ' ');
        if(ft_strcmp(line, "pwd") == 0)
            getworkingdir();
        else if(ft_strcmp(line, "env") == 0)
            getenvfunc(env);
        else if(ft_strcmp(cmdlist[0], "echo") == 0)
            ft_echo(cmdlist);
        else if(ft_strcmp(cmdlist[0], "cd") == 0)
            ft_cd(cmdlist);
        else if(ft_strcmp(cmdlist[0], "export") == 0)
            ft_export(cmdlist, &env);
        else if(ft_strcmp(cmdlist[0], "unset") == 0)
            ft_unset(cmdlist, &env);
        else{
            shell_luncher(cmdlist);
        }
        free(line);
    }



        // for (int i = 0; i < history_length; i++) {
        //     HIST_ENTRY *entrie = history_get(i + 1);  // Get the history list
        //     printf("%d: %s\n", i + 1, entrie->line);  // Print each history entry
        // }
    return (0);
}
