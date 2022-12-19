/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:08:08 by rukkyaa           #+#    #+#             */
/*   Updated: 2022/12/19 18:14:26 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **check_redirection(char *line)
{
	char **redirections;
	int i;

	i = 0;
	redirections = malloc(3 * sizeof(char *)); //a mettre dans la stack direct ?
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
		path = ft_strjoin(paths[i], cmd[0]);
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

void	print_maillon(t_list *elem)
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
	t_list *elem;
	int	status1;
	
	elem = gen_maillon(line);
	if (!elem)
		return (-1);
	print_maillon(elem);
	elem->pid = fork();
	if (elem->pid < 0)
		return (-1);
	if (elem->pid == 0)
	{
		if (elem->redirections[0] && ft_strlen(elem->redirections[0]) != 0 && redirecting(elem->redirections[0], STDIN_FILENO) == -1)
				exit (42);
		if (elem->redirections[1] && ft_strlen(elem->redirections[1]) != 0 && redirecting(elem->redirections[1], STDOUT_FILENO) == -1)
				exit (42); //ajouter le cas >> append
		if (exec_command(paths, elem->cmd, env) == -1)
			exit(42);
	}
	if (waitpid(elem->pid, &status1, 0) < -1 || (WIFEXITED(status1) && WEXITSTATUS(status1) == 42))
		return (-1);
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
	if (line[i + 2] == '\0') //ca veut dire qu'on a parcouru toute la chaine sans trouver un pipe
		exec_command_one(line, paths, env); //cas ou on a une seule commande, reste a traiter les < et > et >>
	//else
	//	pipex();
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char	*line;
	char **paths;

	if (argc != 1)
		return (0);
	(void) **argv;
	line = NULL;
	paths = get_path(env);
	(void) line;
	//int i = 0;
	//while (paths[i])
	//	printf("%s\n", paths[i++]);
	if (paths == NULL)
		return (1);
	while (line == NULL)
	{
		//line = ft_strdup("< test cat");
		line = readline("Minishell> ");
		printf("Line : %s\n", line);
		decomposition(line, paths, env);
		free(line);
		line = NULL;
	}
	return (EXIT_SUCCESS);
}
