/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Variable_Expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 08:06:45 by obarais           #+#    #+#             */
/*   Updated: 2025/05/18 12:05:20 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_shell.h"

char	*get_value(char *str, t_list_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, str) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char	*ft_strjoin_c(char *s1, char c)
{
	char	*str;
	int		i;

	if (!s1)
	{
		str = ft_malloc(2, 0);
		str[0] = c;
		str[1] = '\0';
		return (str);
	}
	i = 0;
	while (s1[i])
		i++;
	str = ft_malloc(i + 2, 0);
	if (!str)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = c;
	str[i + 1] = '\0';
	return (str);
}

char	*split_to_tokens(char *tokn, t_input **temp, char *str, char h)
{
	t_input	*new;
	t_input	*temp1;
	char	*k;
	char	*temp2;
	int		start;
	int		i;

	if (!str)
		return (tokn);
	str = ft_strjoin(tokn, str);
	i = 0;
	temp2 = (*temp)->value;
	temp1 = (*temp)->next;
	(*temp)->next = NULL;
	while (tokn && tokn[i])
		i++;
	while (str[i])
	{
		while (!tokn && str[i] && str[i] <= 32)
			i++;
		start = i;
		while (str[i] && str[i] > 32)
			i++;
		if (!(*temp)->next)
		{
			if (!tokn)
				(*temp)->value = ft_substr(str, start, i - start);
			else
				(*temp)->value = ft_substr(str, 0, i);
			(*temp)->next = *temp;
		}
		else
		{
			new = ft_malloc(sizeof(t_input), 0);
			new->value = ft_substr(str, start, i - start);
			new->type = WORD;
			new->next = new;
			(*temp)->next = new;
			(*temp) = (*temp)->next;
		}
		while (str[i] && str[i] <= 32)
			i++;
	}
	if (str[i - 1] <= 32)
	{
		if (h)
		{
			new = ft_malloc(sizeof(t_input), 0);
			new->value = temp2;
			new->type = WORD;
			(*temp)->next = new;
			*temp = (*temp)->next;
			(*temp)->next = temp1;
			return (NULL);
		}
		k = (*temp)->value;
		(*temp)->value = temp2;
		(*temp)->next = temp1;
		return (k);
	}
	else
	{
		k = (*temp)->value;
		(*temp)->value = temp2;
		(*temp)->next = temp1;
		return (k);
	}
	return (NULL);
}

char	*ft_check_quote(char *str, t_list_env *env, char q)
{
	int		i;
	int		start;
	char	*tokn;

	i = 0;
	tokn = NULL;
	while (str[i] && str[ft_strlen(str) - 1] == q && str[1] != '\0')
	{
		if (str[i] == '"')
		{
			i++;
			while (str[i] && str[i] != '"')
			{
				if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i
							+ 1] == '_' || str[i + 1] == '?'))
				{
					i++;
					if (str[i] >= 48 && str[i] <= 57 && i++)
						continue ;
					start = i;
					while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'
							|| str[i] == '?'))
					{
						i++;
						if (str[i - 1] == '?' && start--)
							break ;
					}
					tokn = ft_strjoin(tokn, get_value(ft_substr(str, start, i
									- start), env));
					if (str[i] && str[i] == '"')
						i++;
				}
				if (str[i])
					tokn = ft_strjoin_c(tokn, str[i]);
				i++;
			}
		}
		else if (str[i] && str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
			{
				tokn = ft_strjoin_c(tokn, str[i]);
				i++;
			}
			i++;
		}
		else
		{
			tokn = ft_strjoin_c(tokn, str[i]);
			i++;
		}
	}
	if (str[ft_strlen(str) - 1] != q || str[1] == '\0')
	{
		printf("Error: Unmatched quotes\n");
		exit(1);
	}
	return (tokn);
}

char	*help_expand_variables(char *str, t_list_env *env)
{
	int		i;
	int		start;
	char	*tokn;

	i = 0;
	tokn = NULL;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) || str[i
					+ 1] == '_'))
		{
			i++;
			if (str[i] >= 48 && str[i] <= 57 && i++)
				continue ;
			start = i;
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				i++;
			tokn = ft_strjoin(tokn, get_value(ft_substr(str, start, i - start),
						env));
		}
		else if (str[i] == '"')
		{
			tokn = ft_strjoin_c(tokn, '\'');
			start = i;
			i++;
			while (str[i] && str[i] != '"')
				i++;
			if (str[i])
				i++;
			tokn = ft_strjoin(tokn, ft_check_quote(ft_substr(str, start, i
							- start), env, '"'));
			tokn = ft_strjoin_c(tokn, '\'');
		}
		else if (str[i] == '\'')
		{
			tokn = ft_strjoin_c(tokn, str[i]);
			start = i;
			i++;
			while (str[i] && str[i] != '\'')
				i++;
			if (str[i])
				i++;
			tokn = ft_strjoin(tokn, ft_check_quote(ft_substr(str, start, i
							- start), env, '\''));
			tokn = ft_strjoin_c(tokn, str[i - 1]);
		}
		else
		{
			tokn = ft_strjoin_c(tokn, str[i]);
			i++;
		}
	}
	return (tokn);
}

void	expand_variables(t_input **tok, t_list_env *env)
{
	int		i;
	int		start;
	char	*tokn;
	t_input	*temp;

	temp = *tok;
	tokn = NULL;
	while (temp)
	{
		i = 0;
		if (temp->value && temp->type == WORD)
		{
			while (temp->value[i])
			{
				if (temp->value[i] == '$' && temp->value[i + 1]
					&& (ft_isalnum(temp->value[i + 1]) || temp->value[i
							+ 1] == '_' || temp->value[i + 1] == '?'))
				{
					i++;
					if (temp->value[i] >= 48 && temp->value[i] <= 57 && i++)
						continue ;
					start = i;
					while (temp->value[i] && (ft_isalnum(temp->value[i])
							|| temp->value[i] == '_' || temp->value[i] == '?'))
					{
						i++;
						if (temp->value[i - 1] == '?' && start--)
							break ;
					}
					if (temp->value[i - 1] == '?')
						tokn = ft_strjoin(tokn, get_value(ft_substr(temp->value,
										start, i - start), env));
					else
						tokn = split_to_tokens(tokn, &temp,
								get_value(ft_substr(temp->value, start, i
										- start), env), temp->value[i]);
				}
				else if (temp->value[i] == '"')
				{
					start = i;
					i++;
					while (temp->value[i] && temp->value[i] != '"')
						i++;
					if (temp->value[i])
						i++;
					tokn = ft_strjoin(tokn,
							ft_check_quote(ft_substr(temp->value, start, i
									- start), env, '"'));
				}
				else if (temp->value[i] == '\'')
				{
					start = i;
					i++;
					while (temp->value[i] && temp->value[i] != '\'')
						i++;
					if (temp->value[i])
						i++;
					tokn = ft_strjoin(tokn,
							ft_check_quote(ft_substr(temp->value, start, i
									- start), env, '\''));
				}
				else
				{
					tokn = ft_strjoin_c(tokn, temp->value[i]);
					i++;
				}
			}
			temp->value = tokn;
			tokn = NULL;
		}
		else if ((temp->type == HEREDOC || temp->type == REDIRECT_OUT
				|| temp->type == APPEND) && temp->next)
			temp = temp->next;
		temp = temp->next;
	}
}
