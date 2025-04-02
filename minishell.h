/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eljamaaouyayman <eljamaaouyayman@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 00:23:34 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/04/01 13:21:01 by eljamaaouya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdbool.h>
# include <stdio.h>
# include <limits.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
#include <readline/readline.h>
#include <readline/history.h>


int	    ft_strcmp(const char *s1, const char *s2);
char	**ft_split(char const *s, char c);
char	*ft_strdup(const char *s1);
char	*ft_strchr(const char *s, int c);
int	    ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(const char *s);
char	*ft_strrchr(const char *s, int c);
char *ft_strndup(const char *s, size_t n);
void ft_unset(char **args, char ***env);


#endif