/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tokns.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:34:00 by obarais           #+#    #+#             */
/*   Updated: 2025/05/19 13:36:09 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char	*random_str(void)
{
	static int	count = 0;
	char		*str;

	str = ft_strjoin("/tmp/heredoc+", ft_itoa(count));
	count++;
	return (str);
}

int	help_expand_heredoc(char **str, int *i, int *start)
{
	(*i)++;
	if ((*str)[*i] >= 48 && (*str)[*i] <= 57)
	{
		(*i)++;
		return (1);
	}
	*start = *i;
	while ((*str)[*i] && (ft_isalnum((*str)[*i]) || (*str)[*i] == '_'
			|| (*str)[*i] == '?'))
	{
		(*i)++;
		if ((*str)[*i - 1] == '?' && (*start)--)
			break ;
	}
	return (0);
}

void	expand_heredoc(char **str, t_list_env *env, int i)
{
	char	*tmp;
	int		start;

	start = 0;
	tmp = NULL;
	while ((*str)[i])
	{
		if ((*str)[i] == '$' && (*str)[i + 1] && (ft_isalnum((*str)[i + 1])
				|| (*str)[i + 1] == '_' || (*str)[i + 1] == '?'))
		{
			if (help_expand_heredoc(str, &i, &start) == 1)
				continue;
			if ((*str)[i - 1] == '?')
				tmp = ft_strjoin(tmp, get_value(ft_substr((*str), start, i
								- start), env));
			else
				tmp = ft_strjoin(tmp, get_value(ft_substr((*str), start, i
								- start), env));
		}
		else
			tmp = ft_strjoin_c(tmp, (*str)[i++]);
	}
	*str = "\0";
	if (tmp)
		*str = tmp;
}

void	help_move_quote(char **str, int *j, char **tmp)
{
	(*j)++;
	while ((*str)[*j] && (*str)[*j] != (*tmp)[0])
	{
		*tmp = ft_strjoin_c(*tmp, (*str)[*j]);
		(*j)++;
	}
	if ((*str)[*j] == (*tmp)[0])
	{
		(*j)++;
		while ((*str)[*j])
		{
			*tmp = ft_strjoin_c(*tmp, (*str)[*j]);
			(*j)++;
		}
		*tmp = ft_strjoin_c(*tmp, (*tmp)[0]);
	}
}

char *check_qout_closed(char *str)
{
	int i;
	char q;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			q = str[i];
			i++;
			while (str[i] && str[i] != q)
				i++;
			if (!str[i])
			{
				write(2, "minishell: unexpected EOF while looking for", 44);
				write(2, " matching \"\'\n", 14);
				write(2, "minishell:syntax error:unexpected end of file\n",
					47);
				return (NULL);
			}
		}
		i++;
	}
	return (str);
}

char	*move_quote(char *str, int i, int j)
{
	char	*tmp;

	tmp = NULL;
	while (str && str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			tmp = ft_strjoin_c(tmp, str[i]);
			while (str[j] && str[j] != tmp[0])
			{
				tmp = ft_strjoin_c(tmp, str[j]);
				j++;
			}
			if (str[j] == tmp[0])
			{
				help_move_quote(&str, &j, &tmp);
				return (tmp);
			}
		}
		i++;
	}
	return (str);
}

void	remove_quote(char **str, int i)
{
	char	*tmp;
	char	qoute;

	tmp = NULL;
	while (*str && (*str)[i])
	{
		if ((*str)[i] == '"' || (*str)[i] == '\'')
		{
			qoute = (*str)[i];
			i++;
			while ((*str)[i] && (*str)[i] != qoute)
			{
				tmp = ft_strjoin_c(tmp, (*str)[i]);
				i++;
			}
			if ((*str)[i])
				i++;
		}
		else
		{
			tmp = ft_strjoin_c(tmp, (*str)[i]);
			i++;
		}
	}
	*str = tmp;
}

void	handler_heredoc(t_input *tok, t_command **cmd_list, t_list_env *env)
{
	int			fd;
	char		*str;
	char		*tmp;
	t_input		*temp;
	static int	j = 0;
	int			status;
	int			b;
	pid_t		pid;
	int			sig;
	t_command *hh;

	b = 0;
	fd = -2;
	temp = tok;
	hh = *cmd_list;
	while (temp)
	{
		if (temp->next && temp->type == HEREDOC && temp->next->type == WORD)
			temp = temp->next;
		else if (temp->next && temp->type == HEREDOC
			&& temp->next->type != WORD)
		{
			printf("%s\n",
				"minishell: syntax error near unexpected token `<<'");
			break ;
		}
		else
			temp = temp->next;
	}
	while (tok)
	{
		if (tok->next && tok->type == HEREDOC && tok->next->type == WORD)
		{
			if (fd > 0)
				unlink(hh->heredoc);
			hh->heredoc = random_str();
			fd = open(hh->heredoc, O_CREAT | O_RDWR | O_TRUNC, 0644);
			if (fd == -1)
			{
				env->value = ft_strdup("1");
				write(2, "minishell: i can not open the file\n", 36);
				return ;
			}
			if (!check_qout_closed(ft_strdup(tok->next->value)))
				break;
			tmp = move_quote(ft_strdup(tok->next->value), 0 , 0);
			pid = fork();
			if (pid == 0)
			{
				signal(SIGINT, SIG_DFL);
				j++;
				str = readline("> ");
				if (tmp[0] != '"' && tmp[0] != '\'' && str)
				{
					b = 2;
					if (ft_strcmp(str, tmp) != 0)
						expand_heredoc(&str, env, 0);
				}
				tmp = tok->next->value;
				remove_quote(&tmp, 0);
				while (str && ft_strcmp(str, tmp) != 0)
				{
					write(fd, str, strlen(str));
					write(fd, "\n", 1);
					j++;
					str = readline("> ");
					if (b == 2 && str)
					{
						if (ft_strcmp(str, tmp) != 0)
							expand_heredoc(&str, env, 0);
					}
				}
				if (!str)
				{
					printf("minishell: warning: here-document at line");
					printf("%d delimited by end-of-file (wanted `%s')\n", j,
						tmp);
				}
				exit(0);
			}
			else
			{
				signal(SIGINT, SIG_IGN);
				waitpid(pid, &status, 0);
				signal(SIGINT, sigint_handler);
				if (WIFSIGNALED(status))
				{
					sig = WTERMSIG(status);
					if (sig == SIGINT)
					{
						hh->heredoc = ft_strdup("ctrlC");
						env->value = ft_strdup(ft_itoa(sig));
						printf("\n");
						break ;
					}
				}
			}
		}
		else if ((tok->type == HEREDOC && !tok->next) || (tok->type == HEREDOC
				&& tok->next->type != WORD))
		{
			if (!tok->next)
			{
				env->value = ft_strdup("1");
				write(2, "minishell: syntax error near ", 30);
				write(2, "unexpected token `newline'\n", 28);
			}
			return ;
		}
		if (tok->type == PIPE)
			hh = hh->next;
		tok = tok->next;
		close(fd);
	}
}

int	help_chek_ambiguous_redirectin(t_list_env *env, t_redir **redir, int *i)
{
	(*redir)->filename = help_expand_variables((*redir)->filename, env);
	(*redir)->filename = move_quote((*redir)->filename, 0, 0);
	if ((*redir)->filename && (*redir)->filename[0] != '\'')
	{
		while ((*redir)->filename[*i])
		{
			if ((*redir)->filename[*i] <= 32)
			{
				write(2, "minishell: ambiguous redirect\n", 31);
					return (1) ;
			}
			(*i)++;
		}
	}
	remove_quote(&(*redir)->filename, 0);
	return (0);
}

int	check_ambiguous_redirect(t_command **cmd_list, t_list_env *env)
{
	t_command	*tmp;
	t_redir		*redir;
	int			i;

	i = 0;
	tmp = *cmd_list;
	while (tmp)
	{
		redir = tmp->inoutfile;
		while (redir)
		{
			if (redir->type == 2 || redir->type == 1)
			{
				if (help_chek_ambiguous_redirectin(env, &redir, &i) == 1)
					return (1);
			}
			redir = redir->next;
		}
		tmp = tmp->next;
	}
	return (0);
}

void	max_heredoc(t_input *tmp)
{
	int i;

	i = 0;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
			i++;
		tmp = tmp->next;
	}
	if (i > 16)
	{
		write(2, "minishell: maximum here-document count exceeded\n", 49);
		ft_malloc(1, 1);
		rl_clear_history();
		exit(2);
	}
}

int	parsing_tokns(t_input *tok, t_command **cmd_list, t_list_env *env)
{
	t_input	*tmp;

	max_heredoc(tok);
	tmp = tok;
	if (tmp->type == PIPE)
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 50);
		env->value = ft_strdup("2");
		return (1);
	}
	tmp = tmp->next;
	while (tmp)
	{
		if (tmp->type == PIPE && (!tmp->next || tmp->next->type == PIPE))
		{
			write(2, "minishell: syntax error near unexpected token `|'\n", 50);
			env->value = ft_strdup("2");
			return (1);
		}
		tmp = tmp->next;
	}
	handler_heredoc(tok, cmd_list, env);
	if (check_ambiguous_redirect(cmd_list, env) == 1)
	{
		env->value = ft_strdup("1");
		return (1);
	}
	return (0);
}
