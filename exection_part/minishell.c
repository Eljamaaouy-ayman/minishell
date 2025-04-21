/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 00:24:19 by eljamaaouya       #+#    #+#             */
/*   Updated: 2025/04/21 07:00:26 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

void	getworkingdir(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("getcwd() error");
}

void	getenvfunc(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

void	shell_luncher(char **cmdlist)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (execve_like_execvp(cmdlist[0], cmdlist) == -1)
			perror("Error");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("Error");
	}
	else
	{
		waitpid(pid, &status, 0);
	}
}

void	ft_echo(char **cmdlist)
{
	bool	flag;
	int		i;

	flag = true;
	i = 1;
	if (cmdlist[1] && ft_strcmp(cmdlist[1], "-n") == 0)
	{
		flag = false;
		i++;
	}
	while (cmdlist[i])
	{
		printf("%s ", cmdlist[i]);
		i++;
	}
	if (flag)
		printf("\n");
}

void	ft_cd(char **cmdlist)
{
	// char cwd[PATH_MAX];
	if (!cmdlist[1])
	{
		printf("cd: missing argument\n");
		return ;
	}
	if (cmdlist[2])
	{
		printf("cd: too many arguments\n");
		return ;
	}
	if (ft_strcmp(cmdlist[1], "~") == 0)
		chdir(getenv("HOME"));
	else if (chdir(cmdlist[1]) != 0)
	{
		perror("Error ");
	}
}

void	exection(t_command *cmd_list, char **env)
{
	if (ft_strcmp(cmd_list->cmd, "pwd") == 0)
		getworkingdir();
	else if (ft_strcmp(cmd_list->cmd, "env") == 0)
		getenvfunc(env);
	else if (ft_strcmp(cmd_list->cmd, "echo") == 0)
		ft_echo(cmdlist);
	else if (ft_strcmp(cmd_list->cmd, "cd") == 0)
		ft_cd(cmdlist);
	else if (ft_strcmp(cmd_list->cmd, "export") == 0)
		ft_export(cmdlist, &env);
	else if (ft_strcmp(cmd_list->cmd, "unset") == 0)
		ft_unset(cmdlist, &env);
	else
	{
		shell_luncher(cmdlist);
	}
	free(cmd_list->cmd);
    
	// for (int i = 0; i < history_length; i++) {
	//     HIST_ENTRY *entrie = history_get(i + 1);  // Get the history list
	//     printf("%d: %s\n", i + 1, entrie->line);  // Print each history entry
	// }
}
