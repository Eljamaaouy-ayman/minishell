/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:21:12 by obarais           #+#    #+#             */
/*   Updated: 2025/04/18 15:36:38 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int main()
{
    pid_t pid;
    const char *prompt = "minishell$ ";
    char *line;

    while (1)
    {
        line = readline(prompt);  // Read input from user
        if (line == NULL)
        {
            printf("Exiting...\n");
            break;
        }

        if (strlen(line) > 0)
        {
            add_history(line);  // Add input to history

            // Split line into arguments (space-separated)
            char **input = ft_split(line);
            t_input *list = NULL;
            list_input(input, &list);
            // Execute the command
            pid = fork();  // Create a child process
            if (pid == 0)
            {
                // In child process: Execute the command
                execute_commind_line(list);
                exit(1);  // Exit child process if exec fails
            }
            else
            {
                // In parent process: Wait for the child to finish
                wait(NULL);
            }
        }

        free(line);  // Free the allocated memory for the line
    }

    return 0;
}
