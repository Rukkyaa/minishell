/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:08:08 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/15 00:18:51 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

t_sig g_sig;

int	countbis(int i, char d, char *line)
{
	while (line[i] != '\0' && line[i] != d)
		i++;
	return (++i);
}

int	countofquotes(char *line, char c, int compt)
{
	int i;

	i = -1;
	if (!(*line))
		return (0);
	while (line[++i] != '\0')
	{
		if (line[i] != '\0' && line[i] == '\"' && c == '\'')
			i = countbis(++i, '\"', line);
		else if (line[i] != '\0' && line[i] == '\'' && c == '\"')
			i = countbis(++i, '\'', line);
		else if (line[i] != '\0' && line[i] == c)
		{
			compt++;
			i++;
			while (line[i] != '\0' && line[i] != c)
				i++;
			if (line[i++] == c)
				compt++;
		}
		if (line[i] == '\0')
			break;
	}
	return (compt);
}

int	invalid_quote(char *line)
{
	if (countofquotes(line, '\"', 0) % 2 != 0
		|| countofquotes(line, '\'', 0) % 2 != 0)
		return (printf("Syntax error\n"), 1);
	else if (ft_strlen(line) > 1 && line[ft_strlen(line) - 2] == '|'
		&& line[ft_strlen(line) - 1] == '|')
		return (printf("Syntax error\n"), 1);
	else if (ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '|')
		return (printf("Syntax error\n"), 1);
	else if (ft_strlen(line) > 1 && line[ft_strlen(line) - 2] == '&'
		&& line[ft_strlen(line) - 1] == '&')
		return (printf("Syntax error\n"), 1);
	return (0);
}

t_tree *parsingator(char *line, t_all *p)
{
	t_tree	*start;
	char	**line_bis;

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

//1- CORRIGER LE PB DE STATUTS
//2- AJOUTER LE BONUS DU WILDCARD
//3- REVOIR TOUTES LES LEAKS
//4- REVOIR TOUTE LA GESTION D'ERREURS
//5- VOIR CAS SPECIFIQUES DES BUILTINS
//6- VOIR CAS SPECIFIQUES DES OP LOGIQUES


//TESTS FINAUX
//test de la var $? (0 - 1 - 134 - 127)
//Refaire des test sur le traitement des variables
//verifier les leaks avec la manipulaiton des signaux
//Faire les tests sur les operateurs logiques

//integrer les signaux (AXEL)
//integrer les quotes (a priori good) (GAB) il se passe un truc bizarre si il 'y en a qu'une
//ajout de l'historique (AXEL)
// faire les builtins sous forme de fonctions (AXEL)


//REVOIR LA SORTIE DE STATUT BUG COMPLET

//revoir la gestion d'erreur au sein des builtins pour qu'elle s'accorde au reste
//cas du heredoc avec le free a corriger
//regarder fct chdir pour les paths a executer, good CHECK LEAKS + PATH ../exec
//reprendre tous les builtins pour leaks (ftstdup)
//revoir la gestion d'erreur du here docs
//verifier l'impact de exit si on le met dans une commande avec differents fichiers et redirections
//pb dans l'organisation des operations de pipe en cas de exit /// Quel est le bon fonctionnement ??



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

int	main(int argc, char **argv, char **env) //ajout du clear history
{
	t_all *p;

	(void)argc;
	(void) **argv;
	p = init_env(env);
	if (!p || p == NULL)
		return (1);
	g_sig.line = NULL;
	while (g_sig.line == NULL)
	{
		init_signal(0);
		rl_event_hook = event;
		g_sig.line = readline("Minishell> ");
		if (ft_strcmp(g_sig.line, "end") == 0 && g_sig.sig_quit == 1)
			return (free(g_sig.line), free_all(p), EXIT_SUCCESS);
		if (g_sig.sig_int == 0 && g_sig.sig_quit == 0)
		{
			add_history(g_sig.line);
			g_sig.p_status = 1;
			g_sig.line = ft_epur(g_sig.line);
			p->start = parsingator(g_sig.line, p); //leaks
			if (g_sig.sig_int == 1) // ajouter l'autre var globale ?
			{
				free_start(p->start, 1);
				p->start = NULL;
			}
			print_all(p);
			if (p->start != NULL && g_sig.sig_int == 0 && executor(p->start, p, g_sig.line) == -1) //distinguer les erreurs de fct des erreurs volontaires dans la gestion
				printf("ERRRRROOOOOR\n");  //UTILISER -1 UNIQUEMENT POUR LES FAILS DE FONCTIONS
			//check_builtins(line, p->env);
			free_here_docs(p->here_docs);
		}
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

