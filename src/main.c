/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpoveda- <me@izenynn.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/24 18:36:26 by dpoveda-          #+#    #+#             */
/*   Updated: 2021/10/24 18:36:46 by dpoveda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strchr(const char *s, int c);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s1);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_substr(char const *s, unsigned int start, size_t len);

/*static char	*get_path(char *cmd, const char *path)
{
	char		*file;
	char		*dir;
	int			i;

	while (path)
	{
		dir = ft_substr(path, 0, (size_t)(path - ft_strchr(path, ':')));
		file = ft_strjoin(dir, cmd);
		free(dir);
		if (access(file, X_OK) == 0)
			return (file);
		free(file);
		path += path - ft_strchr(path, ':');
	}
	return (cmd);
}*/

static void	exec_cmd(char *cmd)
{
	extern char	**environ;
	char		*path;
	int			i;

	i = 0;
	while (environ[i] && ft_strncmp(environ[i], "PATH=", 5))
		i++;
	path = ft_strdup(environ[i] + 5);
	// TODO exec command
	printf("PATH:\n%s\n", path);
	//printf("CMD PATH:\n%s\n", get_path(cmd, path));

	// silence warnings :D
	if (cmd)
		printf("-_-\n");

	free(path);
}

int	main(int argc, char *argv[])
{
	if (argc < 2)
	{
		write(STDERR_FILENO, "ERROR: Invalid arguments\n", 25);
		return(EXIT_FAILURE);
	}
	printf("CMD:\n%s\n", argv[1]);
	exec_cmd(argv[1]);
	return (0);
}
