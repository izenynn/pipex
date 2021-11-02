/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpoveda- <me@izenynn.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 22:51:37 by dpoveda-          #+#    #+#             */
/*   Updated: 2021/11/02 22:51:39 by dpoveda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

/* file i/o */
# define F_INPUT 0
# define F_OUTPUT 1

/* pipe ends */
# define READ_END 0
# define WRITE_END 1

/* libraries */
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>

/* utils.c */
void	err_exit(const char *msg, const char *err);
char	*dir_join(char const *dir, char const *dir2);
void	free_split(char **split);
void	handle_here_doc(int argc, char)

#endif
