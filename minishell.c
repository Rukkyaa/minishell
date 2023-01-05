/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:08:08 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/05 16:20:12 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

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
	//verfier si le heredocs remplace les var avant d'ouvrir le stdin (probablement pas)
	p->here_docs = get_here_docs(line_bis);
	if (p->here_docs == NULL && heredoc_count(line, 0) != 0)
		return (free(*line_bis), NULL);
	//apres le here docs, faire le tri des asterisques
	//remplacer les dollars par les var/ avant ou apres les asterisques a voir
	start = init_tree(line_bis);
	if (init_shell(start, p) == -1)
		return (free_start(start), free(line_bis), NULL);
	return (free(*line_bis), free(line_bis), start);
}

void print_all(t_all *p)
{
	printf("-------AFFICHAGE DES DIFFERENTES BRANCHES-------\n");
	print_tree(p->start, 1);
	printf("------------------------------------------------\n");
	print_cmd(p->start, 1);
	print_here_doc(p->here_docs);
}

//implanter les * (GAB)
	//trouver comment lire les dossiers (AXEL)
//implanter les traitements de $variables (avant ou apres asterisque ?) (GAB/AXEL (fonctionne avec unset))
//effacer tous les strncmp
//integrer les signaux (AXEL)
//integrer les quotes (a priori good) (GAB)
//integrer la gestion d'erreur dans l'exec de pipex (GAB)
//integrer le delire tty (A voir)
//ajout de l'historique (AXEL)
//Tester append (pb quand mixer a <<) (GAB)
//Corriger le probleme des here docs mutiples dans l'exec (GAB)
// faire les builtins sous forme de fonctions (AXEL)
// integrer les builtins (GAB)
//regarder l'histoire du $?

int	main(int argc, char **argv, char **env)
{
	t_all *p;
	char *line;

	(void)argc;
	(void) **argv;
	p = init_env(env);
	if (!p)
		return (1);
	line = NULL;
	while (line == NULL)
	{
		line = ft_epur(readline("Minishell> "));
		//line = ft_strdup("cat test | (wc && (ls || ifconfig))");
		p->start = parsingator(line, p);
		print_all(p);
		if (executor(p->start) == -1)
			printf("ERRRRROOOOOR\n");
		//if (!strncmp(line, "pwd", 3))
		//	ft_pwd();
		//else if (!strncmp(line, "echo", 4))
		//	ft_echo(line);
		free_cmd(p);
		free(line);
		line = NULL;
	}
	free_all(p);
	return (EXIT_SUCCESS);
}

//MAIN SANS READLINE POUR CHECK LEAKS
// int main(int argc, char **argv, char **env)
// {
// 	t_all *p;

// 	(void)argc;
// 	p = init_env(env);
// 	if (!p)
// 		return (1);
// 	p->start = parsingator(argv[1], p);
// 	print_all(p);
// 	free_cmd(p);
// 	free_all(p);
// 	return (EXIT_SUCCESS);
// }

