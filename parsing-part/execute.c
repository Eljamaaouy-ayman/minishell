/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 12:13:56 by obarais           #+#    #+#             */
/*   Updated: 2025/04/17 11:13:26 by obarais          ###   ########.fr       */
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


void    execute_commind_line(t_input *list)
{
    t_input *temp;

    temp = list;
    while (list)
    {
        printf("value: %s\n", list->value);
        printf("type: %d\n", list->type);
        list = list->next;
    }
    if (is_builtin(temp->value) == 1)
        printf("khsny ncode lamr\n");
    else
        printf("ghady nsft l execve\n");
}