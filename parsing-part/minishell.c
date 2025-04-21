/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:21:12 by obarais           #+#    #+#             */
/*   Updated: 2025/04/21 11:02:39 by obarais          ###   ########.fr       */
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

void    typ_of_input(t_input **new)
{
    if (ft_strncmp((*new)->value, "|", ft_strlen((*new)->value)) == 0)
        (*new)->type = PIPE;
    else if (ft_strncmp((*new)->value, "<", ft_strlen((*new)->value)) == 0)
        (*new)->type = REDIRECT_IN;
    else if (ft_strncmp((*new)->value, ">", ft_strlen((*new)->value)) == 0)
        (*new)->type = REDIRECT_OUT;
    else if (ft_strncmp((*new)->value, ">>", ft_strlen((*new)->value)) == 0)
        (*new)->type = APPEND;
    else if (ft_strncmp((*new)->value, "<<", ft_strlen((*new)->value)) == 0)
        (*new)->type = HEREDOC;
    else if ((*new)->value[0] == '$')
        (*new)->type = VARIABLE;
    else
        (*new)->type = WORD;
}

void    list_input(char **input, t_input **list)
{
    int i = 0;
    int j = 0;
    t_input *new;
    t_input *tmp;

    while (input[i] != NULL)
        i++;
    while (j < i)
    {
        new = (t_input *)malloc(sizeof(t_input));
        new->value = ft_strdup(input[j]);
        typ_of_input(&new);
        new->next = NULL;
        if (*list == NULL)
            *list = new;
        else
        {
            tmp = *list;
            while (tmp->next != NULL)
                tmp = tmp->next;
            tmp->next = new;
        }
        j++;
    }
}

char **put_the_args(t_input *list, char *cmd)
{
    char **args;
    int i = 0;
    int j = 0;
    t_input *tmp;
    t_input *tmp2;
    
    tmp = list;
    while (strcmp(tmp->value, cmd) != 0)
        tmp = tmp->next;
    tmp = tmp->next;
    tmp2 = tmp;
    while(tmp2 != NULL && tmp2->type != PIPE)
    {
        i++;
        tmp2 = tmp2->next;
    }
    args = (char **)malloc(sizeof(char *) * (i + 1));
    if (args == NULL)
        return (NULL);
    while (tmp != NULL && tmp->type != PIPE)
    {
        args[j] = ft_strdup(tmp->value);
        j++;
        tmp = tmp->next;
    }
    args[j] = NULL;
    return (args);
}

int what_direction(char *str)
{
    if (strcmp(str, "<") == 0)
        return 0; // input
    else if (strcmp(str, ">") == 0)
        return 1; // output
    else if (strcmp(str, ">>") == 0)
        return 2; // append
    else if (strcmp(str, "<<") == 0)
        return 3; // heredoc
    return -1; // unknown
}

t_redir *check_derctions(char **args)
{
    int i = 0;
    t_redir *redir = NULL;
    
    while (args[i] != NULL)
    {
        if (strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0 || strcmp(args[i], "<<") == 0)
        {
            t_redir *new_redir = (t_redir *)malloc(sizeof(t_redir));
            new_redir->filename = ft_strdup(args[i + 1]);
            new_redir->type = what_direction(args[i]);
            if (redir == NULL)
            {
                redir = new_redir;
                redir->next = NULL;
            }
            else
            {
                t_redir *tmp = redir;
                while (tmp->next != NULL)
                    tmp = tmp->next;
                tmp->next = new_redir;
                new_redir->next = NULL;
            }
            // i++;
        }
        i++;
    }
    return redir;
}


void    list_command(t_input **list, t_command **cmd_list)
{    
    while(*list != NULL)
    {
        t_command *new_cmd = (t_command *)malloc(sizeof(t_command));
        new_cmd->cmd = ft_strdup((*list)->value);
        new_cmd->args = put_the_args(*list, (*list)->value);
        new_cmd->inoutfile = check_derctions(new_cmd->args);
        new_cmd->next = NULL;
        if (*cmd_list == NULL)
            *cmd_list = new_cmd;
        else
        {
            t_command *tmp = *cmd_list;
            while (tmp->next != NULL)
                tmp = tmp->next;
            tmp->next = new_cmd;
        }
        t_input *tmp = *list;
        while (*list != NULL && (*list)->type != PIPE)
        {
            tmp = *list;
            *list = (*list)->next;
            free(tmp->value);
            free(tmp);
        }
        if (*list && (*list)->type == PIPE)
        {
            tmp = *list;
            *list = (*list)->next;
            free(tmp->value);
            free(tmp);
        }
    }
}

int main(int ac, char **av, char **env)
{
    char *line;
    t_command *cmd_list = NULL;
    (void)ac;
    (void)av;

    printbanner();
    while(1)
    {
        line = readline("minishell$ ");  // Read input from user
        if (line == NULL)
        {
            printf("Exiting...\n");
            exit(0);
        }
        if (strlen(line) > 0)
        {
            add_history(line);  // Add input to history

            // Split line into arguments (space-separated)
            char **input = ft_split(line);
            t_input *list = NULL;
            list_input(input, &list);
            list_command(&list, &cmd_list);
            exection(cmd_list, env);
        }
        cmd_list = NULL;
        free(line);  // Free the allocated memory for the line
    }
    return 0;
}
