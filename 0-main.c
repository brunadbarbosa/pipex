/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0-main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brmaria- <brmaria-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 12:38:24 by brmaria-          #+#    #+#             */
/*   Updated: 2025/09/16 18:16:31 by brmaria-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static int	count_args(const char *com)
{
	int		count;
	char	quote;
	
	count = 0;
	while (*com)
	{
		while (*com == ' ')
			com++;
		if (!*com)
			break ;
		count++;
		if (*com == '\''|| *com == '"')
		{
			quote = *com++;
			while (*com && *com != quote)
				com++;
			if (!*com)
				return (-1);
			com++;
		}
		else
			while (*com && *com != '"' && *com != '\''&& *com != ' ')
				com++;
	}
	return (count);
}
static char	*extract_arg(const char *str, int *i)
{
	char	quote;
	int		start;
	char	*arg;

	if (str[*i] == '\'' || str[*i] == '"')
	{
		quote = str[(*i)++];
		start = *i;
		while (str[*i] && str[*i] != quote)
			(*i)++;
		if (!str[*i])
			return (NULL);
		arg = ft_substr(str, start, *i - start);
		(*i)++;
	}
	else
	{
		start = *i;
		while (str[*i] && str[*i] != ' ' && str[*i] != '\''&& str[*i] != '"')
			(*i)++;
		arg = ft_substr(str, start, *i - start);
	}
	if (!arg)
		return (NULL);
	return (arg);
}

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
