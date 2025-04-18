/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 09:40:55 by obarais           #+#    #+#             */
/*   Updated: 2025/04/18 17:50:22 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_word(char const *p)
{
	int	i;
	int	j;
	int k;
	char d = '\0';

	i = 0;
	j = 0;
	k = 0;
	while (p[i] <= 32 && p[i] != '\0')
		i++;
	while (p[i] != '\0')
	{
		if ((p[i] == '"' || p[i] == '\'') && k == 0)
		{
			if (p[i] == '"')
				d = '"';
			else
				d = '\'';
			k = 1;
			j++;
		}
		if (p[i] == '|' && k == 0)
			j++;
		else if (p[i] > 32 && k == 0 && (i == 0 || p[i - 1] <= 32 || p[i - 1] == '|'))
			j++;
		i++;
		if (p[i] == d)
		{
			k = 0;
			i++;
		}
	}
	return (j);
}

static char	*ft_alloc_and_cpy(char const *s, size_t *i)
{
	size_t	start;
	size_t	len;
	char 	d;

	start = *i;
	len = 0;
	if (s[*i] == '|')
	{
		(*i)++;
		return ("|");
	}
	if (s[*i] == '"' || s[*i] == '\'')
	{
		if (s[*i] == '"')
			d = '"';
		else
			d = '\'';
		(*i)++;
		len++;
		while (s[*i] != '\0' && s[*i] != d)
		{
			len++;
			(*i)++;
		}
		len++;
		(*i)++;
	}
	else
	{
		while (s[*i] != '\0' && s[*i] > 32 && s[*i] != '|')
		{
			len++;
			(*i)++;
		}
	}
	return (ft_substr(s, start, len));
}

static char	**ft_free(char **array, size_t j)
{
	while (j > 0)
	{
		j--;
		free(array[j]);
	}
	free(array);
	return (NULL);
}

char	**ft_split(char const *s)
{
	size_t	i;
	size_t	j;
	size_t	words;
	char	**array;

	if (s == NULL)
		return (NULL);
	i = 0;
	j = 0;
	words = ft_count_word(s);
	array = (char **)malloc((words + 1) * sizeof(char *));
	if (array == NULL)
		return (NULL);
	while (j < words)
	{
		while (s[i] <= 32)
			i++;
		array[j] = ft_alloc_and_cpy(s, &i);
		if (!array[j])
			return (ft_free(array, j));
		j++;
	}
	array[j] = NULL;
	return (array);
}
