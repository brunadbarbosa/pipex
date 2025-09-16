/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1-parent.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brmaria- <brmaria-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 18:03:12 by brmaria-          #+#    #+#             */
/*   Updated: 2025/09/16 18:03:48 by brmaria-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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