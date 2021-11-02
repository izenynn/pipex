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

#include <pipex.h>
#include <libft.h>

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
	{
		ft_putstr_fd("Error: path not found", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	args = ft_split(cmd, ' ');
	if (*args[0] == '/' || *args[0] == '.' || *args[0] == '~')
		cmd_path = args[0];
	else
		cmd_path = get_path(args[0], path);
	free(path);
	execve(cmd_path, args, environ);
	free_split(args);
	die(cmd_path);
}

static void	redir(char *cmd)
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
	if (argc >= 6 && !ft_strncmp(argv[1], "here_doc", 9))
	{
		fd_io[F_OP] = open_file(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND);
		handle_here_doc(argv[2]);
		i = 2;
	}
	else if (argc >= 5)
	{
		fd_io[F_IP] = open_file(argv[1], O_RDONLY);
		fd_io[F_OP] = open_file(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC);

		if (fd_io[F_IP] == -1)
			die(argv[1]);
		else if (fd_io[F_OP] == -1)
			die(argv[argc - 1]);

		dup2(fd_io[F_IP], STDIN_FILENO);
		i = 1;
	}
	else
	{
		ft_putstr_fd("Error: invalid arguments", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	while (++i < argc - 2)
		redir(argv[i]);
	dup2(fd_io[F_OP], STDOUT_FILENO);
	exec_cmd(argv[i]);
	return (0);
}
