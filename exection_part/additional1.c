/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-jama <ael-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:23:38 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/04/19 10:15:26 by ael-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	char	b;

	i = ft_strlen(s);
	b = (unsigned char)c;
	if (b == '\0')
		return ((char *)(s + i));
	while (i > 0)
	{
		if (s[i - 1] == b)
			return ((char *)(s + i -1));
		i--;
	}
	return (NULL);
}

char *ft_strndup(const char *s, size_t n)
{
    size_t len = 0;
    char *dup;

    while (len < n && s[len])
        len++;
    dup = malloc(len + 1);
    if (!dup)
	{
        return NULL;
	}
	len = 0;
    while (len < n && s[len])
	{
		dup[len] = s[len];
		len++;
	}
    dup[len] = '\0';
    return dup;
}
