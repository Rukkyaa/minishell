/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:08:08 by rukkyaa           #+#    #+#             */
/*   Updated: 2022/12/29 10:29:29 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"



//int	exec_command(char **paths, char **cmd, char **env)
//{
//	int		i;
//	char	*path;

//	i = -1;
//	if (!cmd || ft_strlen(cmd[0]) == 0)
//		return (ft_putstr_fd("'' : command not found\n", 2), -1);
//	while (paths[++i] != NULL)
//	{
//		path = ft_strjoin_spe(paths[i], cmd[0]);
//		if (path == NULL)
//			return (-1);
//		if (access(path, F_OK) == 0)
//		{
//			if (access(path, X_OK) != 0)
//				return (perror(""), free(path), -1);
//			if (execve(path, cmd, env) == -1)
//				return (free(path), -1);
//			return (free(path), 0);
//		}
//		free(path);
//	}
//	ft_putstr_fd(cmd[0], 2);
//	ft_putstr_fd(": command not found\n", 2);
//	return (-1);
//}

//int	redir_non_null(char **redir)
//{
//	if (redir[0] && ft_strlen(redir[0]) != 0)
//		return (1);
//	if (redir[1] && ft_strlen(redir[1]) != 0)
//		return (1);
//	if (redir[2] && ft_strlen(redir[2]) != 0)
//		return (1);
//	return (0);
//}

//int redirecting(char *file, int port) // ajout d'une option append qui va lire tout le fichier avant de renvoyer le port
//{
//	int fdt;

//	if (access(file, F_OK) != 0)
//		return (perror(""), -1);
//	fdt = open(file, O_RDONLY);
//	if (fdt == -1)
//		return (perror(""), -1);
//	if (dup2(fdt, port) < 0)
//		return (perror(""), -1);
//	return (0);
//}

//int	exec_command_one(char *line, char **paths, char **env)
//{
//	t_minishell *elem;
//	//int	status1;

//	printf("Debut de exec_command_one\n");
//	elem = gen_maillon(line);
//	if (!elem)
//		return (-1);
//	print_maillon(elem);
//	printf("Fin de exec_command_one\n");
//	printf("%p%p", paths, env);
//	// elem->pid = fork();
//	// if (elem->pid < 0)
//	// 	return (-1);
//	// if (elem->pid == 0)
//	// {
//	// 	if (elem->redirections[0] && ft_strlen(elem->redirections[0]) != 0 && redirecting(elem->redirections[0], STDIN_FILENO) == -1)
//	// 			exit (42);
//	// 	if (elem->redirections[1] && ft_strlen(elem->redirections[1]) != 0 && redirecting(elem->redirections[1], STDOUT_FILENO) == -1)
//	// 			exit (42); //ajouter le cas >> append
//	// 	if (exec_command(paths, elem->cmd, env) == -1)
//	// 		exit(42);
//	// }
//	// if (waitpid(elem->pid, &status1, 0) < -1 || (WIFEXITED(status1) && WEXITSTATUS(status1) == 42))
//	// 	return (-1);
//	return (0);
//}

//int count_op(char *line, int i, int compt, int indic)
//{
//	if (line[i] == '\0' || line[i + 1] == '\0' || line[i + 2] == '\0')
//		return (compt);
//	if (indic == 1 && line[i] == ')')
//		return (compt);
//	else if (indic == 0 && line[i] == ')')
//		return (-1);
//	if (line[i] == '(')
//	{
//		compt *= count_op(line, ++i, 1, 1);
//		return (count_op(line, ++i, compt, 0));
//	}
//	if ((line[i] == '&' && line[i + 1] == '&')||(line[i] == '|' && line[i + 1] == '|'))
//		return (count_op(line, ++i, ++compt, 0));
//	return (-1);
//}


t_tree *parsingator(char *line, t_all *p)
{
	//reste a gerer les quotes, les meta carateres
	//fonction qui recupere les here_docs
	// fonction qui anticipe les quotes (calcul de la somme)
	t_tree *start;
	char **line_bis;

	line_bis = malloc(sizeof(char *));
	if (!line_bis)
		return (NULL);
	*line_bis = ft_strdup(line);
	if (!line || !p || !(*line_bis))
		return (NULL);
	//ajout d'emblee d'une securite qui traite les quotes
	p->here_docs = get_here_docs(line_bis);
	if (p->here_docs == NULL && heredoc_count(line, 0) != 0)
		return (free(*line_bis), NULL);
	start = init_tree(line_bis);
	if (init_shell(start, p) == -1)
		return (free_start(start), free(line_bis), NULL);
	return (start);
}

void print_all(t_all *p)
{
	printf("-------AFFICHAGE DES DIFFERENTES BRANCHES-------\n");
	print_tree(p->start, 1);
	printf("------------------------------------------------\n");
	print_cmd(p->start, 1);
	print_here_doc(p->here_docs);
}

void	check_builtins(char *str, t_env *env)
{
	if (!strncmp(str, "pwd", 3))
		ft_pwd();
	else if (!strncmp(str, "echo", 4))
		ft_echo(str + 4);
	//else if (!strncmp(str, "cd", 2))
	//	ft_cd(str);
	else if (!strncmp(str, "env", 3))
		ft_env(env);
	//else if (!strncmp(str, "export", 6))
	//	ft_export(str);
	else if (!strncmp(str, "unset", 5))
		ft_unset(env, str);
	//else if (!strncmp(str, "exit", 4))
	//	ft_exit();
}

int	main(int argc, char **argv, char **env)
{
	t_all *p;
	t_env *env_struct;
	char *line;

	(void)argc;
	(void) **argv;
	p = init_env(env);
	if (!p)
		return (EXIT_FAILURE);
	env_struct = NULL;
	env_struct = env_to_struct(env, env_struct);
	line = NULL;
	while (line == NULL)
	{
		line = ft_epur(readline("Minishell> "));
		//line = ft_strdup("cat test | (wc && (ls || ifconfig))");
		p->start = parsingator(line, p);
		print_all(p);
		check_builtins(line, env_struct);
		
		free_cmd(p);
		free(line);
		line = NULL;
	}
	free_all(p);
	return (EXIT_SUCCESS);
}
