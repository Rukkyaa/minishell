/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 09:55:17 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/17 12:38:07 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// int	opening(char *file, int port, int append, int mode)
// {
// 	int fdt;
// 	char *line;

// 	if (file == NULL || !file)
// 		return (-1);
// 	if (mode == 0 && (access(file, F_OK) != 0 || access(file, R_OK) != 0))
// 		return (perror(""), -1);
// 	else if (mode == 1 && (access(file, F_OK) == 0 && access(file, W_OK) != 0))
// 		return (perror(""), -1);
// 	else if (mode == 1 && access(file, F_OK) != 0)
// 		fdt = open(file, O_WRONLY | O_CREAT, S_IRWXU);
// 	else if (mode == 1)
// 		fdt = open(file, O_WRONLY);
// 	else if (mode == 0)
// 		fdt = open(file, O_RDONLY);
// 	else
// 		fdt = -1;
// 	if (append == 1 && mode == 1 && fdt != -1)
// 	{
// 		line = get_next_line(fdt);
// 		while (line != NULL)
// 		{
// 			free(line);
// 			line = get_next_line(fdt);
// 		}
// 	}
// 	if (fdt == -1)
// 		return (perror(""), -1);
// 	if (dup2(fdt, port) < 0)
// 		return (perror(""), close(fdt), -1);
// 	return (close(fdt), 0);
// }

int	opening_out(t_outfile *file_org, int port)
{
	int fdt;
	t_outfile *file;

	file = file_org;
	if (file == NULL || !file)
		return (-1);
	while (file->next != NULL)
	{
		if (access(file->file_out, F_OK) == 0 && access(file->file_out, W_OK) != 0)
			return (perror(file->file_out), -1);
		else
		{
			if (file->append == 1)
				fdt = open(file->file_out, O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
			else
				fdt = open(file->file_out, O_WRONLY | O_CREAT, S_IRWXU);
			if (fdt == -1)
				return (perror(""), -1);
			if (close(fdt) == -1)
				return (perror(""), -1);
		}
		file = file->next;
	}
	if (access(file->file_out, F_OK) == 0 && access(file->file_out, W_OK) != 0)
		return (perror(file->file_out), -1);
	else if (file->append == 1)
		fdt = open(file->file_out, O_WRONLY | O_APPEND | O_CREAT, S_IRWXU);
	else
		fdt = open(file->file_out, O_WRONLY | O_CREAT, S_IRWXU);
	if (fdt == -1)
		return (perror(""), -1);
	if (dup2(fdt, port) < 0)
		return (perror(""), close(fdt), -1);
	return (close(fdt), 0);
}

int	opening_in(t_infile *file_org, int port)
{
	int fdt;
	t_infile *file;

	file = file_org;
	if (file == NULL || !file)
		return (-1);
	while(file->next != NULL)
	{
		if (access(file->file_in, F_OK) != 0 || access(file->file_in, R_OK) != 0)
			return (perror(file->file_in),  -1);
		file = file->next;
	}
	if (access(file->file_in, F_OK) != 0 || access(file->file_in, R_OK) != 0)
		return (perror(file->file_in), -1);
	fdt = open(file->file_in, O_RDONLY);
	if (fdt == -1)
		return (perror(""), -1);
	if (dup2(fdt, port) < 0)
		return (perror(""), close(fdt), -1);
	return (close(fdt), 0);
}

char **env_to_char(t_env *env)
{
	char **reforged;
	int i;
	int e;
	t_env *temp;

	i = 0;
	e = 0;
	if (!env || env == NULL)
		return (NULL);
	temp = env;
	while (temp != NULL && i++ > -1)
		temp = temp->next;
	reforged = malloc((i + 1) * sizeof(char *));
	if (!reforged)
		return (NULL);
	while (e < i)
	{
		if (env->code == 1 || env->code == 2)
		{
			reforged[e] = ft_strjoin(env->key, "=");
			reforged[e] = ft_strjoin_spe(reforged[e], env->value); //securite direc dans le spe pour le reforged
			if (reforged[e++] == NULL)
				return (free_tab(reforged), NULL);
		}
		env = env->next;
	}
	return (reforged[e] = NULL, reforged);
}

// int	lengthmegatab(char ***cmd)
// {
// 	int i;
// 	int len;

// 	i = 0;
// 	len = 0;
// 	while (cmd[i] != NULL)
// 		len += length_tab(cmd[i++]);
// 	return (len);
// }

// char **mega_to_tab(char ***cmd)
// {
// 	char **reforged_cmd;
// 	int i;
// 	int e;

// 	i = -1;
// 	reforged_cmd = malloc((lengthmegatab(cmd) + 1) * sizeof(char *));
// 	if (!reforged_cmd)
// 		return (NULL);
// 	while (cmd[++i] != NULL)
// 	{
// 		reforged_cmd[i] = ft_strdup(cmd[i][0]);
// 		if (reforged_cmd[i] == NULL)
// 			return (free_tab(reforged_cmd), NULL);
// 		e = 0;
// 		while (cmd[i][++e] != NULL)
// 		{
// 			reforged_cmd[i] = ft_strjoin_spe(reforged_cmd[i], " ");
// 			reforged_cmd[i] = ft_strjoin_spe(reforged_cmd[i], cmd[i][e]); // add securite
// 		}
// 	}
// 	reforged_cmd[i] = NULL;
// 	return (reforged_cmd);
// }

int	exec_command(char **paths, char **cmd, t_all *p, t_tree *start)
{
	int		i;
	char	*path;
	char	**reforged_env;

	i = -1;
	if (!cmd || !*cmd || ft_strlen(cmd[0]) == 0)
		return (ft_putstr_fd("'' : command not found\n", 2), -1);
	if (path_comp_builtins(cmd) > 0)
	{
		printf("---entering builtins---\n");
		if (exec_builtin(path_comp_builtins(cmd), cmd, p, start) == EXIT_FAILURE)
			return (-1);
		return (0);
	}
	else if (path_comp_builtins(cmd) < 0)
		return (0);
	reforged_env = env_to_char(p->env);
	if (reforged_env == NULL && p->env != NULL)
		return (-1);
	while (paths[++i] != NULL)
	{
		path = ft_strjoin(paths[i], cmd[0]);
		if (path == NULL)
			return (-1); //gesrtion d'erreurs 
		if (access(path, F_OK) == 0)
		{
			if (access(path, X_OK) != 0)
				return (perror(""), free(path), g_sig.cmd_stat = 126, 126);
			if (execve(path, cmd, reforged_env) == -1)
				return (free(path), free_tab(reforged_env), -1);
			return (free(path), free_tab(reforged_env), 0);
		}
		free(path);
	}
	if (access(cmd[0], F_OK) == 0)
	{
		if (access(cmd[0], X_OK) != 0)
			return (perror(""), free(path), free_tab(reforged_env), g_sig.cmd_stat = 126, 126);
		if (execve(cmd[0], cmd, reforged_env) == -1)
			return (free_tab(reforged_env),  -1);
		return (free_tab(reforged_env), 0);
	}
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	return (free_tab(reforged_env), free(g_sig.line), g_sig.cmd_stat = 127, 127);
}

void error_process(t_all *p)
{
	free_start(p->start, 0);
	free_tab(p->paths);
	free_env(p->env);
	free_here_docs(p->here_docs);
	free(p);
	if (g_sig.sig_quit == 1)
		exit(134);
	else if (g_sig.sig_int == 1)
		exit(0);
	else if (g_sig.cmd_stat == 127)
		exit (127);
	else if (g_sig.cmd_stat == 126)
		exit (126);
	exit(1);
}

int	exec_command_one(t_minishell *elem, t_all *p, t_tree *start)
{
	int	status1;

	// if (g_sig.sig_int == 0 && g_sig.sig_quit == 0 && ft_strcmp(elem->cmd[0], "cd") == 0)
	// 	return(ft_cd(p->env, elem->cmd));
	if (path_comp_builtins(elem->cmd) < 0)
		return (exec_builtin(path_comp_builtins(elem->cmd), elem->cmd, p, start));
	elem->pid = fork();
	if (elem->pid < 0)
		return (-1);
	if (elem->pid == 0)
	{
		if (signal(SIGINT, &sighandler) == SIG_ERR || signal(SIGQUIT, &sig_eof) == SIG_ERR)
			error_process(p);
		init_signal(-1);
		if (g_sig.sig_int != 0 || g_sig.sig_quit != 0 || (elem->file_in != NULL && opening_in(elem->file_in, STDIN_FILENO) == -1))
			error_process(p);
		if (g_sig.sig_int != 0 || g_sig.sig_quit != 0 || (elem->file_out != NULL && opening_out(elem->file_out, STDOUT_FILENO) == -1))
			error_process(p);
		if (g_sig.sig_int != 0 || g_sig.sig_quit != 0 || exec_command(p->paths, elem->cmd, p, start) != 0)
			error_process(p);
		exit(0);
	}
	if (waitpid(elem->pid, &status1, 0) < -1 || ((WIFEXITED(status1)) && WEXITSTATUS(status1) != 0))
		return (WEXITSTATUS(status1));
	return (0);
}

int	pipex(t_minishell *first_elem, t_all *p, t_tree *start)
{
	if (first_elem == NULL || !first_elem)
		return (0);
	else if (first_elem->next == NULL)
		return (exec_command_one(first_elem, p, start));
	else if (first_elem->next != NULL && first_elem->next->next != NULL && ft_strcmp(first_elem->cmd[0], "cat") == 0)
		return (printf("spe\n"), first_pipe_al(first_elem, p, start));
	return (first_pipe(first_elem, p, start));
}

void kill_process(t_tree *start, t_all *p, char *line)
{
	free_start(start, 1);
	free_here_docs(p->here_docs);
	free(line); //A ACTIVER AVEC LE MAIN PRINCIPAL
	//printf("%s\n", line);
	free_all(p);
	clear_history();
	exit (0);
}

// int wildcard_treat(t_minishell *first_elem, t_all *p, t_tree *start)
// {
// 	int ret;
// 	// if (start->first_elem->other_w == NULL)
// 	// 	return (pipex(start->first_elem, p, start));
// 	ret == pipex(start->first_elem, p, start);
// 	if (ret != 0)
// 		return (ret);
// 	else if (start->first_elem->other_w == NULL)
// 		return (0);
// 	else
// 		return (wildcard_treat())
// }

int	executor(t_tree *start, t_all *p, char *line)
{
	t_tree	*intermed;

	if (start == NULL || !start)
		return (-1);
	g_sig.cmd_stat = pipex(start->first_elem, p, start);
	if (check_minishell(start->first_elem->cmd) == 1 && g_sig.sig_int < 0 && g_sig.sig_quit < 0)
	{
		g_sig.sig_int = 0;
		g_sig.sig_quit = 0;
	}
	if (g_sig.cmd_stat == 134)
		kill_process(start, p, line);
	free(start->cmd);
	if (g_sig.cmd_stat != 0) //verifier que c'est bien ce qui est attendu
		free_minishell(start->first_elem, 1);
	else
		free_minishell(start->first_elem, 0);
	if (start->and != NULL && g_sig.cmd_stat == 0)
		return (intermed = start->and, free(start), executor(intermed, p, line));
	else if (start->or != NULL && g_sig.cmd_stat != 0)
		return (intermed = start->or, free(start), executor(intermed, p, line));
	//else if (p->last_status != 0)
	return (free(start), g_sig.cmd_stat); //quid en cas d'erreur
	//return (free(start), 0);
}