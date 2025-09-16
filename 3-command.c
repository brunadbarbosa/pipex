/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3-command.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brmaria- <brmaria-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 18:15:35 by brmaria-          #+#    #+#             */
/*   Updated: 2025/09/16 18:16:40 by brmaria-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*pipex_find_path(char *command, char **envp, char *which)
{
	char	**split;
	char	*path;
	char	*pre_path;
	int		i;

	i = 0;
	while (envp[i] && ft_strcmp(envp[i], which, ft_strlen(which)) != 0)
		i++;
	split = ft_split(envp[i] + ft_strlen(which), ':');
	i = 0;
	while (split[i])
	{
		pre_path = ft_strjoin(split[i], "/");
		path = ft_strjoin(pre_path, command);
		free(pre_path);
		if (access(path, F_OK | X_OK) == 0)
		{
			free_split(split);
			return (path);
		}
		free(path);
		i++;
	}
	free_split(split);
	return (0);
}

void	pipex_execute(char *argv, char **envp)
{
	char	**command;
	char	*path;

	command = split_quote(argv);
	if (!command)
	{
		free(command);
		exit(1);
	}
	path = pipex_find_path(command[0], envp, "PATH=");
	if (command[0][0] == '/')
	{
		free(path);
		path = ft_strdup(command[0]);
	}
	if (!path)
	{
		path = pipex_find_path(command[0], envp, "PWD=");
		if(!path || access(path, F_OK | X_OK) == -1)
			exit(1);
	}
	if(execve(path, command, envp) == -1)
		exit(1);
}
