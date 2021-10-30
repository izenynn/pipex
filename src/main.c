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

/* file i/o */
#define F_INPUT 0
#define F_OUTPUT 1

/* pipe ends */
#define READ_END 0
#define WRITE_END 1

void	die(const char *msg);
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
	perror("execve");
	free_split(args);
	exit(EXIT_FAILURE);
}

static void redir(char *cmd)
{
	int fd[2];
	pid_t pid;

	if (pipe(fd) == -1)
		die("pipe");
	pid = fork();
	/* check if fork failed */
	if (pid < 0)
		die("fork");
	/* parent process */
	if (pid > 0)
	{
		close(fd[WRITE_END]);
		dup2(fd[READ_END], STDIN_FILENO);
		close(fd[READ_END]);
		waitpid(pid, NULL, 0);
	}
	/* child will write the output of the command */
	else
	{
		close(fd[READ_END]);
		dup2(fd[WRITE_END], STDOUT_FILENO);
		close(fd[WRITE_END]);
		exec_cmd(cmd);
	}
}

/*void leaks()
{
	system("leaks -q pipex");
}*/

int	main(int argc, char *argv[])
{
	int	i;
	int	fd_io[2];

	//atexit(leaks);
	if (argc < 5)
	{
		write(STDERR_FILENO, "ERROR: Invalid arguments\n", 25);
		return(EXIT_FAILURE);
	}
	fd_io[F_INPUT] = open(argv[1], O_RDONLY);
	fd_io[F_OUTPUT] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_io[0] == -1 || fd_io[1] == -1)
	{
		perror("open");
		return (EXIT_FAILURE);
	}
	dup2(fd_io[F_INPUT], STDIN_FILENO);
	dup2(fd_io[F_OUTPUT], STDOUT_FILENO);
	i = 1;
	while (++i < argc)
		redir(argv[i]);
	exec_cmd(argv[3]);
	return (0);
}
