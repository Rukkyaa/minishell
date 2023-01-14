/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:08:08 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/14 01:20:13 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

t_sig g_sig;

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

int	countofquotes(char *line, char c, int compt)
{
	int i;

	i = -1;
	if (!(*line))
		return (0);
	while (line[++i] != '\0')
	{
		if (line[i] != '\0' && line[i] == '\"' && c == '\'')
		{
			i++;
			while (line[i] != '\0' && line[i] != '\"')
				i++;
			i++;
		}
		else if (line[i] != '\0' && line[i] == '\'' && c == '\"')
		{
			i++;
			while (line[i] != '\0' && line[i] != '\'')
				i++;
			i++;
		}
		else if (line[i] != '\0' && line[i] == c)
		{
			compt++;
			i++;
			while (line[i] != '\0' && line[i] != c)
				i++;
			if (line[i] == c)
				compt++;
			i++;
		}
		if (line[i] == '\0')
			break;
	}
	return (compt);
}

int	invalid_quote(char *line)
{
	//printf("\": %d || \': %d\n", countofquotes(line, '\"', 0), countofquotes(line, '\'', 0));
	if (countofquotes(line, '\"', 0) % 2 != 0 || countofquotes(line, '\'', 0) % 2 != 0)
		return (printf("Syntax error\n"), 1);
	else if (ft_strlen(line) > 1 && line[ft_strlen(line) - 2] == '|' && line[ft_strlen(line) - 1] == '|')
		return (printf("Syntax error\n"), 1);
	else if (ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '|')
		return (printf("Syntax error\n"), 1);
	else if (ft_strlen(line) > 1 && line[ft_strlen(line) - 2] == '&' && line[ft_strlen(line) - 1] == '&')
		return (printf("Syntax error\n"), 1);
	return (0);
}

t_tree *parsingator(char *line, t_all *p)
{
	t_tree *start;
	char **line_bis;

	line_bis = malloc(sizeof(char *));
	if (!line_bis)
		return (NULL);
	*line_bis = ft_strdup(line);
	if (!line || !p || !(*line_bis))
		return (NULL);
	if (invalid_quote(*line_bis) == 1)
		return (free(*line_bis), free(line_bis), NULL);
	p->here_docs = get_here_docs(line_bis);
	if (p->here_docs == NULL && heredoc_count(line, 0) != 0)
		return (free(*line_bis), NULL);
	printf("c");
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


//integrer les signaux (AXEL)
//integrer les quotes (a priori good) (GAB) il se passe un truc bizarre si il 'y en a qu'une
//ajout de l'historique (AXEL)
// faire les builtins sous forme de fonctions (AXEL)


//revoir la gestion d'erreur au sein des builtins pour qu'elle s'accorde au reste
//cas du heredoc avec $"" (traduit en \0) + heredoc chelou si il y a pas de commande avant 
//Refaire des test sur le traitement des variables
//Faire les tests sur les operateurs logiques
//regarder fct chdir pour les paths a executer, good CHECK LEAKS + PATH ../exec
// gerer les differents statuts de sortie
//TEST=tptp
//integrer le code de la struct env aux fonctions
//reprendre tous les builtins pour leaks (ftstdup)
//revoir la gestion d'erreur du here docs
//verifier l'impact de exit si on le met dans une commande avec differents fichiers et redirections
//pb dans l'organisation des operations de pipe en cas de exit
//double free du exit avec par ex "cat Makefile && exit"
//integrer la comprehension des signaux dans les get nex line

//PB DE BUILTINS
//cas des mutiples exit et lancement de minishell dans le minishell
//TRAITER LE CAS DES ./ ET ../ AVEC LA FCT CD AXEL
//cas de l'executeur du shel (reinterpreter ./minishelle en /minishell par ex), pareil les ../ et faire un accesss + pouvoir lire les chemins de fichier qui ramene en arriere (../../...)
//cas special avec le cd ou on peut creer deux dossier puis rm le parent

//traiter l'env em moins (env -i | bash) GOOD
//empecher l'exec si nombre de quotes impaires GOOD
//definir une politique claire sur le cas ou il n' a qu'une seule quote GOOD
//traite le cas "cat " OU "cat |" // En gros le bloquer et dire syntax error GOOD
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

int event(void) 
{
	return (42);
}

// void	check_builtins(char *str, t_env *env)
// {
// 	// printf("Builtins !\n");
// 	if (!strncmp(str, "pwd", 3))
// 		ft_pwd();
// 	else if (!strncmp(str, "echo", 4))
// 		ft_echo(str + 4);
// 	else if (!strncmp(str, "cd", 2))
// 		ft_cd(env, str);
// 	else if (!strncmp(str, "env", 3))
// 		ft_env(env);
// 	else if (!strncmp(str, "export", 6))
// 		ft_export(env, str + 7);
// 	else if (!strncmp(str, "unset", 5))
// 		ft_unset(env, str + 6);
// 	//else if (!strncmp(str, "exit", 4))
// 	//	ft_exit();
// }


//valgrind --leak-check=full --show-leak-kinds=all --suppressions=./.readline.supp ./minishell

int	main(int argc, char **argv, char **env)
{
	t_all *p;

	(void)argc;
	(void) **argv;
	p = init_env(env);
	if (!p)
		return (1);
	g_sig.line = NULL;
	if (signal(SIGINT, &sig_int) == SIG_ERR || signal(SIGQUIT, &sig_quit) == SIG_ERR)
		return (free_all(p), 1);
	while (g_sig.line == NULL)
	{
		init_signal(0);
		rl_event_hook = event;
		g_sig.line = readline("Minishell> ");
		add_history(g_sig.line);
		if (ft_strcmp(g_sig.line, "end") == 0 && g_sig.sig_int == 1)
			return (free(g_sig.line), free_all(p), EXIT_SUCCESS);
		g_sig.p_status = 1;
		g_sig.line = ft_epur(g_sig.line);
		p->start = parsingator(g_sig.line, p); //leaks
		if (g_sig.sig_int == 1)
			return (free_start(p->start, 1), free_here_docs(p->here_docs), free(g_sig.line), free_all(p), EXIT_SUCCESS);
		print_all(p);
		if (p->start != NULL && executor(p->start, p, g_sig.line) == -1)
			printf("ERRRRROOOOOR\n");
		//check_builtins(line, p->env);
		free_here_docs(p->here_docs);
		free(g_sig.line);
		g_sig.line = NULL;
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
// 	p->start = parsingator(argv[1], p);
// 	print_all(p);
// 	if (executor(p->start, p, argv[1]) == -1)
// 		printf("ERRRRROOOOOR\n");
// 	printf("finito");
// 	free_here_docs(p->here_docs);
// 	free_all(p);
// 	return (EXIT_SUCCESS);
// }

