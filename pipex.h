/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brmaria- <brmaria-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 12:38:37 by brmaria-          #+#    #+#             */
/*   Updated: 2025/09/16 18:34:44 by brmaria-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PIPEX
# define PIPEX

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>

char	**split_quote(char *command);
void	free_split(char **split);
void	call_parent(char **argv, char **envp, int *pipefd);
void	call_child(char **argv, char **envp, int *pipefd);
void	pipex_execute(char *argv, char **envp);
char	*pipex_find_path(char *command, char **envp, char *which);

#endif