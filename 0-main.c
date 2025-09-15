/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0-main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brmaria- <brmaria-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 12:38:24 by brmaria-          #+#    #+#             */
/*   Updated: 2025/09/12 18:30:38 by brmaria-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**split_quote(char *command)
{
	char **result;
	int	i;
	int	j;

	i = 0;
	j = 0;
	result = ft_calloc(count_args(command) + 1, sizeof(char *));
	if (!result)
		return(NULL);
	while (command[j] && (i < count_args(command)))
	{
		while (command[j] == ' ')
			j++;
		if (!command[j])
			break ;
		result[i] = extract_arg(command, &j);
		if(!result[i])
		{
			free_split(result);
			return (NULL);
		}
		i++;
	}
	result[i] = NULL;
	return (result);
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

void	call_parent(char **argv, char **envp, int *pipefd)
{
	int	fd;

	fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1);
	{
		close(fd);
		exit(1);		
	}
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	pipex_execute(argv[3], envp);
}

void	call_child(char **argv, char **envp, int *pipefd)
{
	int	fd;

	fd = open(argv[1], O_RDONLY);
	if (fd == -1);
	{
		close(fd);
		exit(1);		
	}
	dup2(fd, STDIN_FILENO);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	close(fd);
	pipex_execute(argv[2], envp);
}

int	main(int argc, char **argv, char **envp)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;
	
	if (argc != 5)
		printf("Usage: ./pipex infile <cmd1> <cmd2> outfile");
	if (pipe(pipefd) == -1)
		printf("Pipe");
	pid1 = fork();
	if (pid1 == -1)
		printf("Fork");
	if (pid1 == 0)
		call_child(argv, envp, pipefd);
	pid2 = fork();
	if (pid2 == -1)
		printf("Fork");
	if (pid2 == 0)
	call_parent(argv, envp, pipefd);
	close_fds(pipefd[0], pipefd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
	return (WEXITSTATUS(status2));
}
