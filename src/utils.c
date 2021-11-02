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

/* like join two strings with a '/' between them */
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

/* open file and handle errors */
int	open_f(char *file, int oflag)
{
	int	fd;

	fd = open(file, oflag, 0644);
	if (fd == -1)
		die(file);
	return (fd);
}

/* handle here document "<<" */
void	handle_here_doc(char *argv)
{
	int		fd[2];
	pid_t	pid;
	char	*line;
	char	*delim;

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
		delim = ft_strjoin(argv, "\n");
		close(fd[READ_END]);
		line = ft_get_next_line(STDIN_FILENO);
		while (line)
		{
			if (!ft_strncmp(line, delim, ft_strlen(delim) + 1))
			{
				free(line);
				free(delim);
				exit(EXIT_SUCCESS);
			}
			ft_putstr_fd(line, fd[WRITE_END]);
			free(line);
			line = ft_get_next_line(STDIN_FILENO);
		}
		free(line);
		free(delim);
	}
}
