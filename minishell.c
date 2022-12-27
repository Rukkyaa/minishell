/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:08:08 by rukkyaa           #+#    #+#             */
/*   Updated: 2022/12/27 02:13:08 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

char **check_redirection(char *line)
{
	char **redirections;
	int i;

	i = 0;
	redirections = malloc(4 * sizeof(char *)); //a mettre dans la stack direct ?
	if (!redirections)
		return (NULL);
	redirections[0] = NULL;
	redirections[1] = NULL;
	redirections[2] = NULL;
	redirections[3] = NULL;
	while (line[i + 2] != '\0') //il y a sans doute une plus opti a faire
	{
		if ((i == 0 && line[i] == '<' && line[i + 1] != '<') || (line[i - 1] != '<' && line[i] == '<' && line[i + 2] != '<')) //NB: il peut y avoir plusieurs fois la meme redirection
			redirections[0] = get_filename(line, i + 1);
		if (line[i] != '>' && line[i + 1] == '>' && line[i + 2] != '>') //NB: il peut y avoir plusieurs fois la meme redirection
			redirections[1] = get_filename(line, i + 2);
		i++;
	}
	i = 0;
	while (line[i + 3] != '\0')
	{
		if (line[i] != '>' && line[i + 1] == '>' && line[i + 2] == '>' && line[i + 3] != '>') //NB: il peut y avoir plusieurs fois la meme redirection
			redirections[2] = get_filename(line, i + 3);
		i++;
	}
	return (redirections); //traiter le cas de <<
}

int	exec_command(char **paths, char **cmd, char **env)
{
	int		i;
	char	*path;

	i = -1;
	if (!cmd || ft_strlen(cmd[0]) == 0)
		return (ft_putstr_fd("'' : command not found\n", 2), -1);
	while (paths[++i] != NULL)
	{
		path = ft_strjoin_spe(paths[i], cmd[0]);
		if (path == NULL)
			return (-1);
		if (access(path, F_OK) == 0)
		{
			if (access(path, X_OK) != 0)
				return (perror(""), free(path), -1);
			if (execve(path, cmd, env) == -1)
				return (free(path), -1);
			return (free(path), 0);
		}
		free(path);
	}
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	return (-1);
}

int	redir_non_null(char **redir)
{
	if (redir[0] && ft_strlen(redir[0]) != 0)
		return (1);
	if (redir[1] && ft_strlen(redir[1]) != 0)
		return (1);
	if (redir[2] && ft_strlen(redir[2]) != 0)
		return (1);
	return (0);
}

int redirecting(char *file, int port) // ajout d'une option append qui va lire tout le fichier avant de renvoyer le port
{
	int fdt;

	if (access(file, F_OK) != 0)
		return (perror(""), -1);
	fdt = open(file, O_RDONLY);
	if (fdt == -1)
		return (perror(""), -1);
	if (dup2(fdt, port) < 0)
		return (perror(""), -1);
	return (0);
}

void	print_maillon(t_minishell *elem)
{
	int i = 0;

	printf("------------INFOS REPEREES-----------\n");
	printf("Redirections :\n");
	printf("< : %s\n", elem->redirections[0]);
	printf("> : %s\n", elem->redirections[1]);
	printf(">> : %s\n", elem->redirections[2]);
	printf("Commande stockees dans un tableau:\n");
	while (elem->cmd[i] != NULL)
	{
		printf("cmd[%d]: %s\n", i, elem->cmd[i]);
		i++;
	}
}

int	exec_command_one(char *line, char **paths, char **env)
{
	t_minishell *elem;
	//int	status1;

	printf("Debut de exec_command_one\n");
	elem = gen_maillon(line);
	if (!elem)
		return (-1);
	print_maillon(elem);
	printf("Fin de exec_command_one\n");
	printf("%p%p", paths, env);
	// elem->pid = fork();
	// if (elem->pid < 0)
	// 	return (-1);
	// if (elem->pid == 0)
	// {
	// 	if (elem->redirections[0] && ft_strlen(elem->redirections[0]) != 0 && redirecting(elem->redirections[0], STDIN_FILENO) == -1)
	// 			exit (42);
	// 	if (elem->redirections[1] && ft_strlen(elem->redirections[1]) != 0 && redirecting(elem->redirections[1], STDOUT_FILENO) == -1)
	// 			exit (42); //ajouter le cas >> append
	// 	if (exec_command(paths, elem->cmd, env) == -1)
	// 		exit(42);
	// }
	// if (waitpid(elem->pid, &status1, 0) < -1 || (WIFEXITED(status1) && WEXITSTATUS(status1) == 42))
	// 	return (-1);
	return (0);
}

int decomposition(char *line, char **paths, char **env)
{
	//traiter ici les operateurs || et && et les parentheses avec un while pour lancer autant de fois et dans le bon ordre la fct "exec_command"
	//cas ou il n'y a pas d'operateurs logiques
	int i;

	i = 0;
	while (line[i + 2] != '\0') //gerer le cas ou il va y avoir un pipe seul, ex : "cat file |"
	{
		if (line[i] != '|' && line[i + 1] == '|' && line[i + 2] != '|')
			break;
		i++;
	}
	if (line[i + 2] == '\0')
		exec_command_one(line, paths, env); //cas ou on a une seule commande, reste a traiter les < et > et >>
	// else
	// 	pipex();
	return (0);
}

void	minishell_init(t_minishell **minishell)
{
	(*minishell)->infile = dup(STDIN_FILENO);
	(*minishell)->outfile = dup(STDOUT_FILENO);
}

int	main(int argc, char **argv, char **env)
{
	char		*line;
	char		**paths;
	//t_minishell	*minishell;
	t_env		*env_list;
	
	(void)argc;
	(void) **argv;
	//minishell_init(&minishell);
	line = NULL;
	(void) line;
	paths = split_path(get_env_var(env, "PATH"), ':');
	if (!paths)
		return (1);
	env_list = NULL;
	env_list = env_to_struct(env, env_list);
	while (line == NULL)
	{
		line = ft_epur(readline("Minishell> "));
		if (!strncmp(line, "pwd", 3))
			ft_pwd();
		else if (!strncmp(line, "echo", 4))
			ft_echo(line);
		else if (!strncmp(line, "env", 3))
			ft_env(env_list);
		else if (!strncmp(line, "unset", 5))
			ft_unset(env_list, line + 6);
		free(line);
		line = NULL;
	}
	free_tab(paths);
	t_env	*tmp;
	while (env_list)
	{
		tmp = env_list->next;
		free(env_list->key);
		free(env_list->value);
		free(env_list);
		env_list = tmp;
	}
	return (EXIT_SUCCESS);
}
