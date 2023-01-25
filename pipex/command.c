/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 00:16:01 by gatsby            #+#    #+#             */
/*   Updated: 2023/01/25 00:52:21 by gatsby           ###   ########.fr       */
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

static int exec_now(char *path, char **cmd, char **reforged_env, int opt)
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

static int final_opt(char **cmd, char **reforged_env)
{
	if (check_directory(cmd[0]) != 0)
		return (free_tab(reforged_env), -1);
	if (access(cmd[0], F_OK) == 0
        && (ft_strncmp(cmd[0], "./", 2) == 0
            || ft_strncmp(cmd[0], "../", 3) == 0))
		return (exec_now(cmd[0], cmd, reforged_env, 0));
    ft_putstr_fd(cmd[0], 2);
    ft_putstr_fd(": command not found\n", 2);
    free_tab(reforged_env);
    g_sig.cmd_stat = 127;
    return (127);
}

int	exec_command(char **paths, char **cmd, t_all *p, t_tree *start)
{
	int		i;
	char	*path;
	char	**reforged_env;

	i = -1;
	if (!cmd || !*cmd || ft_strlen(cmd[0]) == 0)
		return (ft_putstr_fd("'' : command not found\n", 2), -1);
	if (path_comp_builtins(cmd) > 0)
		return (exec_builtin(path_comp_builtins(cmd), cmd, p, start));
	else if (path_comp_builtins(cmd) < 0)
		end_process(p, 0);
	reforged_env = env_to_char(p->env);
	if (reforged_env == NULL && p->env != NULL)
		return (-1);
	while (paths[++i] != NULL)
	{
		path = ft_strjoin(paths[i], cmd[0]);
        if (path == NULL)
		    return (free_tab(reforged_env), -1);
	    if (access(path, F_OK) == 0)
            return (exec_now(path, cmd, reforged_env, 1));
        free(path);
    }
	return (final_opt(cmd, reforged_env));
}

void end_process(t_all *p, int nb)
{
	free_start(p->start, 0);
	free_tab(p->paths);
	free_env(p->env);
	free_here_docs(p->here_docs);
	free(p);
	free(g_sig.line);
	rl_clear_history();
	if (g_sig.sig_quit == 1)
		exit(134);
	else if (g_sig.sig_int == 1)
		exit(0);
	else if (g_sig.cmd_stat == 127)
		exit (127);
	else if (g_sig.cmd_stat == 126)
		exit (126);
	exit(nb);
}