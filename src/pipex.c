/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 17:08:36 by cagonzal          #+#    #+#             */
/*   Updated: 2022/07/06 11:29:34 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	openfile(char *filename, int mode)
{
	if (mode == INFILE)
	{
		if (open (filename, O_RDONLY) < 0)
			ft_error(errno, filename);
		return (open (filename, O_RDONLY));
	}
	else
	{
		if (open (filename, O_CREAT | O_WRONLY | O_TRUNC, 0644) < 0)
			ft_error(errno, filename);
		return (open (filename, O_CREAT | O_WRONLY | O_TRUNC, 0644));
	}
	return (0);
}

static char	*getpath(char *cmd, char **env)
{
	char	*path;
	char	*dir;
	char	*bin;
	int		i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (cmd);
	path = env[i] + 5;
	while (path && ft_strchr(path, ':'))
	{
		dir = ft_strndup(path, ft_strichr(path, ':'));
		bin = ft_strjoin(dir, "/");
		bin = ft_strjoin(bin, cmd);
		free(dir);
		if (access(bin, F_OK) == 0)
			return (bin);
		free(bin);
		path += ft_strichr(path, ':') + 1;
	}
	return (cmd);
}

static void	exec(char *cmd, char **env)
{
	char	**args;
	char	*path;
	int		i;

	i = 1;
	args = ft_split(cmd, ' ');
	if (ft_strchr(args[0], '/'))
		path = args[0];
	else
		path = getpath(args[0], env);
	while (args[i])
	{
		if (args[i][0] == 39)
			args[i] = ft_substr(args[i], 1, ft_strlen(args[i]));
		if (args[i][ft_strlen(args[i]) - 1] == 39)
			args[i] = ft_substr(args[i], 0, ft_strlen(args[i]) - 1);
		i++;
	}
	execve(path, args, env);
	ft_error(-1, args[0]);
	exit(127);
}

static void	redir(char *cmd, char **env)
{
	int		pipefd[2];
	pid_t	pid;

	pipe(pipefd);
	pid = fork();
	if (pid < 0)
		ft_error(errno, NULL);
	else if (pid != 0)
	{
		dup2(pipefd[0], STDIN);
		close(pipefd[1]);
		close(pipefd[0]);
		waitpid(pid, NULL, 0);
	}
	else
	{
		dup2(pipefd[1], STDOUT);
		close(pipefd[0]);
		close(pipefd[1]);
		exec(cmd, env);
	}
}

int	main(int argc, char **argv, char **env)
{
	int		fdin;
	int		fdout;

	if (argc == 5)
	{
		fdin = openfile(argv[1], INFILE);
		fdout = openfile(argv[4], OUTFILE);
		dup2(fdin, STDIN);
		dup2(fdout, STDOUT);
		redir(argv[2], env);
		exec(argv[3], env);
	}
	else
		ft_error(-2, NULL);
	return (1);
}
