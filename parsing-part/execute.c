/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eljamaaouyayman <eljamaaouyayman@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 12:13:56 by obarais           #+#    #+#             */
/*   Updated: 2025/04/20 14:48:50 by eljamaaouya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    return (
        strcmp(cmd, "cd") == 0 ||
        strcmp(cmd, "echo") == 0 ||
        strcmp(cmd, "pwd") == 0 ||
        strcmp(cmd, "export") == 0 ||
        strcmp(cmd, "unset") == 0 ||
        strcmp(cmd, "env") == 0 ||
        strcmp(cmd, "exit") == 0
    );
}


void    execute_commind_line(t_command **list)
{
    t_command *temp;

    temp = *list;
    (void)temp;
    
    while(list)
    {
        printf("Command: %s\n", (*list)->cmd);
        printf("Arguments: ");
        for (int i = 0; (*list)->args[i] != NULL; i++)
            printf("%s ", (*list)->args[i]);
        printf("\n");
        printf("Input redirection: ");
        t_redir *redir = (*list)->inoutfile;
        while (redir)
        {
            printf("%s    ", redir->filename);
            printf("Type: %d    ", redir->type);
            redir = redir->next;
        }
        printf("\n");
        *list = (*list)->next;
    }
    // int i = 0;
    // while (*list != NULL)
    // {
    //     free((*list)->cmd);
    //     while ((*list)->args[i] != NULL)
    //     {
    //         free((*list)->args[i]);
    //         i++;
    //     }
    //     free((*list)->inoutfile->filename);
    //     *list = (*list)->next;
    // }
    // if (is_builtin(temp->value) == 1)
    //     printf("khsny ncode lamr\n");
    // else
    //     printf("ghady nsft l execve\n");
}
