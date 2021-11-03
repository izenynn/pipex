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

/* open file and handle errors */
static int	open_f(char *file, int oflag)
{
	int	fd;

	fd = open(file, oflag, 0644);
	if (fd == -1)
		die(file);
	return (fd);
}

/* find command in path */
static char	*get_path(char *cmd, const char *path)
{
	char	*file;
	char	*dir;
	int		diff;

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
		if (ft_strlen(path) < (size_t)diff)
			break ;
		path += diff;
		if (*path)
			path++;
	}
	ft_dprintf(STDERR_FILENO, "%s: command not found\n", cmd);
	exit(EXIT_FAILURE);
	return (NULL);
}

/* execute a command */
static void	exec_cmd(char *cmd)
{
	extern char	**environ;
	char		*cmd_path;
	char		**args;
	int			i;

	args = ft_split(cmd, ' ');
	if (*args[0] == '/' || *args[0] == '.' || *args[0] == '~')
		cmd_path = args[0];
	else
	{
		i = 0;
		while (environ[i] && ft_strncmp(environ[i], "PATH=", 5))
			i++;
		if (!environ[i])
		{
			ft_putstr_fd("Error: path not found\n", STDERR_FILENO);
			exit(EXIT_FAILURE);
		}
		cmd_path = get_path(args[0], environ[i] + 5);
	}
	execve(cmd_path, args, environ);
	die(cmd_path);
}

/* redirection intput and output with pipe() */
static void	redir(char *cmd)
{
	int		fd[2];
	pid_t	pid;
	int		status;

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
		waitpid(pid, &status, 0);
		if (WEXITSTATUS(status) == EXIT_FAILURE)
			exit(EXIT_FAILURE);
	}
	else
	{
		close(fd[READ_END]);
		dup2(fd[WRITE_END], STDOUT_FILENO);
		close(fd[WRITE_END]);
		exec_cmd(cmd);
	}
}

/* main */
int	main(int argc, char *argv[])
{
	int	i;
	int	fd_io[2];

	if (argc >= 5 || (argc >= 6 && !ft_strncmp(argv[1], "here_doc", 9)))
	{
		if (!ft_strncmp(argv[1], "here_doc", 9))
		{
			handle_here_doc(argv[2]);
			fd_io[F_OP] = open_f(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND);
			i = 2;
		}
		else
		{
			fd_io[F_IP] = open_f(argv[1], O_RDONLY);
			fd_io[F_OP] = open_f(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC);
			dup2(fd_io[F_IP], STDIN_FILENO);
			i = 1;
		}
		while (++i < argc - 2)
			redir(argv[i]);
		dup2(fd_io[F_OP], STDOUT_FILENO);
		exec_cmd(argv[i]);
	}
	ft_putstr_fd("Error: invalid arguments", STDERR_FILENO);
	return (EXIT_FAILURE);
}
