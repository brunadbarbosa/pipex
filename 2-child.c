/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2-child.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brmaria- <brmaria-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 18:04:07 by brmaria-          #+#    #+#             */
/*   Updated: 2025/09/16 18:04:41 by brmaria-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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