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
#include <errno.h>

int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(const char *s);
void	err_exit(const char *err, const char *msg);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s1);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*dir_join(char const *dir, char const *dir2);
char	**ft_split(const char *s, char c);
void	free_split(char **split);

static char	*get_path(char *cmd, const char *path)
{
	char		*file;
	char		*dir;
	int			diff;

	while (*path)
	{
		diff = ft_strchr(path, ':') - path;
		if (diff < 0)
			diff = ft_strlen(path);
		dir = ft_substr(path, 0, diff);
		file = dir_join(dir, cmd);
		free(dir);
		if (access(file, X_OK) == 0)
			return (file);
		free(file);
		path += diff;
		if (*path)
			path++;
	}
	return (cmd);
}

static void	exec_cmd(char *cmd)
{
	extern char	**environ;
	char		*path;
	char		*cmd_path;
	char		**args;
	int			i;

	i = 0;
	while (environ[i] && ft_strncmp(environ[i], "PATH=", 5))
		i++;
	path = ft_strdup(environ[i] + 5);
	if (!path)
		err_exit("Error", "path not found");
	args = ft_split(cmd, ' ');
	if (*args[0] == '/' || *args[0] == '.' || *args[0] == '~')
		cmd_path = args[0];
	else
	{	
		cmd_path = get_path(args[0], path);
	}
	free(path);
	execve(cmd_path, args, environ);
	err_exit("Error", "execve returned error");
	free_split(args);
}

int	main(int argc, char *argv[])
{
	if (argc < 2)
	{
		write(STDERR_FILENO, "ERROR: Invalid arguments\n", 25);
		return(EXIT_FAILURE);
	}
	exec_cmd(argv[1]);
	return (0);
}
