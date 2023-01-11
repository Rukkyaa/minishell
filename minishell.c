/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:08:08 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/11 16:29:59 by axlamber         ###   ########.fr       */
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
	start = init_tree(replace_var(line_bis, p));
	if (init_shell(start, p) == -1) //verifier la gestion d'erreur au cas ou le replacement var bug, quelles implications sur init tree et le cleaning
		return (free(*line_bis), free(line_bis), free(start->cmd), free(start), NULL);
	return (free(*line_bis), free(line_bis), start);
}

void print_all(t_all *p)
{
	if (p->start == NULL)
		return ;
	printf("-------AFFICHAGE DES DIFFERENTES BRANCHES-------\n");
	print_tree(p->start, 1);
	printf("------------------------------------------------\n");
	print_cmd(p->start, 1);
	print_here_doc(p->here_docs);
}


//effacer tous les strncmp
//integrer les signaux (AXEL)
//integrer les quotes (a priori good) (GAB) il se passe un truc bizarre si il 'y en a qu'une
//ajout de l'historique (AXEL)
// faire les builtins sous forme de fonctions (AXEL)


//revoir la gestion d'erreur au sein des builtins pour qu'elle s'accorde au reste
//definir une politique claire sur le cas ou il n' a qu'une seule quote
//traite le cas "cat " OU "cat |" // En gros le bloquer et dire syntax error 
//cas du heredoc avec $"" (traduit en \0) + heredoc chelou si il y a pas de commande avant 
//souci de parsing et de reecriture des chaines A PRIORI GOOD
//Refaire des test sur le traitement des variables
//Faire les tests sur les operateurs logiques
//regarder fct chdir pour les paths a executer
// gerer les differents statuts de sortie
//TEST=tptp
//traiter l'env em moins (env -i | bash)
//reprendre la tilde, bug avec la nvlle struct de env


//PB DE BUILTINS
//cas des mutiples exit et lancement de minishell dans le minishell
//TRAITER LE CAS DES ./ ET ../ AVEC LA FCT CD AXEL
//cas de l'executeur du shel (reinterpreter ./minishelle en /minishell par ex), pareil les ../ et faire un accesss + pouvoir lire les chemins de fichier qui ramene en arriere (../../...)
//cas special avec le cd ou on peut creer deux dossier puis rm le parent

//reintegrer la struct env GOOD
//regarder aussi le segfault si " | "    "| || etc" GOOD
//regarder l'histoire du $? GOOD
//leaks a gerer pour | GOOD
//trouver pq les multiples outfiles bugs avec le pipe GOOD
//ajourter la tilde ~ GOOD
//gerer les destructions de fichiers en cas d'erreur de la commande GOOD
//env -i, savoir gerer sans l'env et copier dans le dur le reste du env GOOD
//verifier que le parsing encaisse bien plusieurs redirections semblables et supprime ou garde les fichiers vides selon bash GOOD
//cas des ulltiples $ : $$$USER GOOD
//cas du "'$USER'" GOOD

void	check_builtins(char *str, t_env *env)
{
	// printf("Builtins !\n");
	if (!strncmp(str, "pwd", 3))
		ft_pwd();
	else if (!strncmp(str, "echo", 4))
		ft_echo(str + 4);
	else if (!strncmp(str, "cd", 2))
		ft_cd(env, str);
	else if (!strncmp(str, "env", 3))
		ft_env(env);
	else if (!strncmp(str, "export", 6))
		ft_export(env, str + 7);
	else if (!strncmp(str, "unset", 5))
		ft_unset(env, str + 6);
	//else if (!strncmp(str, "exit", 4))
	//	ft_exit();
}

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
		// //line = ft_strdup("cat test | (wc && (ls || ifconfig))");
		// p->start = parsingator(line, p);
		// // if (p->start == NULL)
		// // 	free_start(p->start)
		// print_all(p);
		// if (p->start != NULL && executor(p->start, p) == -1)
		// 	printf("ERRRRROOOOOR\n");
		// //if (!strncmp(line, "pwd", 3))
		// //	ft_pwd();
		// //else if (!strncmp(line, "echo", 4))
		// //	ft_echo(line);
		check_builtins(line, p->env);
		free_here_docs(p->here_docs);
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
// 	if (!p || p == NULL)
// 		return (printf("Error in setting environment\n"), 1);
// 	p->start = parsingator(argv[1], p);
// 	print_all(p);
// 	if (executor(p->start, p) == -1)
// 		printf("ERRRRROOOOOR\n");
// 	printf("finito");
// 	free_here_docs(p->here_docs);
// 	free_all(p);
// 	return (EXIT_SUCCESS);
// }

