/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpoveda- <me@izenynn.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/24 20:11:12 by dpoveda-          #+#    #+#             */
/*   Updated: 2021/10/24 20:11:13 by dpoveda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>
#include <libft.h>

/* handle errors */
void	die(const char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}

/* join two strings with a '/' between them */
char	*dir_join(char const *dir1, char const *dir2)
{
	char	*save;
	char	*out;

	if (!dir1 || !dir2)
		return (NULL);
	out = malloc(ft_strlen(dir1) + ft_strlen(dir2) + 2);
	if (!out)
		return (NULL);
	save = out;
	while (*dir1)
		*out++ = *dir1++;
	*out++ = '/';
	while (*dir2)
		*out++ = *dir2++;
	*out = '\0';
	return (save);
}

/* free a **char returned by ft_split */
void	free_split(char **split)
{
	int	i;

	i = -1;
	while (split[++i])
		free(split[i]);
	free(split);
}

/* handle here document read until delim */
static void	handle_read_hd(char *argv, int fd[2])
{
	char	*line;
	char	*delim;

	delim = ft_strjoin(argv, "\n");
	close(fd[READ_END]);
	line = ft_get_next_line(STDIN_FILENO);
	while (line)
	{
		if (!ft_strncmp(line, delim, ft_strlen(delim) + 1))
		{
			close(WRITE_END);
			free(line);
			free(delim);
			exit(EXIT_SUCCESS);
		}
		ft_putstr_fd(line, fd[WRITE_END]);
		free(line);
		line = ft_get_next_line(STDIN_FILENO);
	}
	close(WRITE_END);
	free(line);
	free(delim);
}

/* handle here document "<<" */
void	handle_here_doc(char *argv)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		die("pipe");
	pid = fork();
	if (pid < 0)
		die("fork");
	if (pid > 0)
	{
		close(fd[WRITE_END]);
		dup2(fd[READ_END], STDIN_FILENO);
		close(fd[READ_END]);
		waitpid(pid, NULL, 0);
	}
	else
	{
		handle_read_hd(argv, fd);
	}
}
