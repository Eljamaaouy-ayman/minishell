/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eljamaaouyayman <eljamaaouyayman@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:21:36 by obarais           #+#    #+#             */
/*   Updated: 2025/04/20 20:23:03 by eljamaaouya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
// #include "../exection_part/minishell_exec.h"

typedef enum e_input_type
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC,
	VARIABLE,
}					t_input_type;

typedef struct s_input
{
	char			*value;
	t_input_type	type;
	struct s_input	*next;
}					t_input;

typedef struct s_redir
{
    char *filename;
    int   type; // 0 = input, 1 = output, 2 = append, 3 = heredoc
    struct s_redir *next;
} t_redir;

typedef struct s_command
{
	char    *cmd;       // "cat"
    char    **args;      // {"file.txt", NULL}
    t_redir *inoutfile;     // لائحة ديال ملفات input (input أو heredoc)
    struct s_command *next;
} t_command;

void				execute_commind_line(t_command **list);
int	ft_strcmp(const char *s1, const char *s2);
void    exection(t_command *cmd_list);

#endif