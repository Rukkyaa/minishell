/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 00:16:01 by gatsby            #+#    #+#             */
/*   Updated: 2023/01/26 22:03:36 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_directory(char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
		return (0);
	if (S_ISDIR(st.st_mode) == 0)
		return (0);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	g_sig.cmd_stat = 126;
	return (126);
}

static int	exec_now(char *path, char **cmd, char **reforged_env, int opt)
{
	if (access(path, X_OK) != 0)
	{
		if (opt == 1)
			free(path);
		return (perror(""), free_tab(reforged_env), g_sig.cmd_stat = 126, 126);
	}
	if (execve(path, cmd, reforged_env) == -1)
	{
		if (opt == 1)
			free(path);
		return (free_tab(reforged_env), -1);
	}
	if (opt == 1)
		free(path);
	return (free_tab(reforged_env), 0);
}

static int	final_opt(char **cmd, char **reforged_env)
{
	if (check_directory(cmd[0]) != 0)
		return (free_tab(reforged_env), -1);
	if (access(cmd[0], F_OK) == 0
		&& (ft_strncmp(cmd[0], "./", 2) == 0
			|| ft_strncmp(cmd[0], "../", 3) == 0
			|| ft_strncmp(cmd[0], "/", 1) == 0))
		return (exec_now(cmd[0], cmd, reforged_env, 0));
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	free_tab(reforged_env);
	g_sig.cmd_stat = 127;
	return (127);
}

void	clean_p(char **paths, int flag, t_all *p)
{
	if (paths != NULL)
		free_tab(paths);
	if (flag == 1)
	{
		ft_putstr_fd("'' : command not found\n", 2);
		end_process(p, 0);
	}
}

int	exec_command(char **paths, char **cmd, t_all *p, t_tree *start)
{
	int		i;
	char	*path;
	char	**reforged_env;

	i = -1;
	if (!cmd || !*cmd || ft_strlen(cmd[0]) == 0)
		clean_p(paths, 1, p);
	if (path_comp_builtins(cmd) > 0)
		return (clean_p(paths, 0, p),
			exec_builtin(path_comp_builtins(cmd), cmd, p, start));
	else if (path_comp_builtins(cmd) < 0)
		return (clean_p(paths, 0, p), end_process(p, 0), 0);
	reforged_env = env_to_char(p->env);
	if (reforged_env == NULL && p->env != NULL)
		return (-1);
	while (paths != NULL && paths[++i] != NULL)
	{
		path = ft_strjoin(paths[i], cmd[0]);
		if (path == NULL)
			return (free_tab(reforged_env), free_tab(paths), -1);
		if (access(path, F_OK) == 0)
			return (free_tab(paths), exec_now(path, cmd, reforged_env, 1));
		free(path);
	}
	return (clean_p(paths, 0, p), final_opt(cmd, reforged_env));
}
