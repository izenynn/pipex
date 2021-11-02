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

void	err_exit(const char *msg, const char *err)
{
	ft_dprintf(1, "%s: %s\n", msg, err);
	exit(EXIT_FAILURE);
}

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

void	free_split(char **split)
{
	int	i;

	i = -1;
	while (split[++i])
		free(split[i]);
	free(split);
}

void	handle_here_doc(int argc, char *delim)
{
	int		fd[2];
	pid_t	pid;
	char	*line;

	if (pipe(fd) == -1)
		err_exit("pipe", errno);
	pid = fork();
	if (pid < 0)
		err_exit("fork", errno);
	/* parent */
	if (pid > 0)
	{
		close(fd[READ_END]);
		dup2(fd[WRITE_END], STDIN_FILENO);
		close(fd[WRITE_END]);
		waitpid(pid, NULL, 0);
	}
	/* child */
	else
	{
		close(fd[READ_END]);
		line = ft_get_next_line(STDIN_FILENO);
		while (line)
		{
			if (!ft_strncmp(line, delim, ft_strlen(delim) + 1))
				exit(EXIT_SUCCESS);
			ft_putstr_fd(fd[WRITE_END], line);
			free(line);
			line = ft_get_next_line(STDIN_FILENO);
		}
		free(line);
	}
}
