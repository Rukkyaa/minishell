/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:08:08 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/20 15:54:46 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

t_sig g_sig;

t_tree	*parsingator(char *line, t_all *p)
{
	t_tree	*start;
	char	**line_bis;

	if (line == NULL || !line)
		return (NULL);
	line_bis = malloc(sizeof(char *));
	if (!line_bis)
		return (NULL);
	*line_bis = ft_strdup(line);
	if (!(*line_bis) || *line_bis == NULL)
		return (free(line_bis), NULL);
	if (invalid_quote(*line_bis) == 1 || first_check(*line_bis) == 1)
		return (free(*line_bis), free(line_bis), NULL);
	p->here_docs = get_here_docs(line_bis, p);
	if (p->here_docs == NULL && heredoc_count(line, 0) != 0)
		return (free(*line_bis), free(line_bis), NULL);
	*line_bis = replace_var(*line_bis, p);
	if (*line_bis == NULL)
		return (free(line_bis), NULL);
	start = init_tree(line_bis);
	if (init_shell(start, p) == -1)
		return (free_start(start, 1), NULL);
	return (start);
}

//	important : modifier l'exit dans les finctions des pipes pour qu'il soit traite avant fork (EN FCT de bash)
// verifier que les builtins hors forks n'ont pas a utiliser les redirections
//encore qq test sur les signaux
//4- REVOIR TOUTE LA GESTION D'ERREURS
//5- VOIR CAS SPECIFIQUES DES BUILTINS
//6- VOIR CAS SPECIFIQUES DES OP LOGIQUES
//voir pour la regeneration du prompt avec les signaux

// trim quote des var a l'exec uniquement et l$test avec test = "s -la"

// valgrind pendant here docs --> LEAKS

//lancer cd export et unset dans le parent process direct (test avec les pipes)

// cas particulier : ls -l | grep mi > axel | cat < axel
//TESTS FINAUX
//test de la var $? (0 - 1 - 134 - 127)
//Refaire des test sur le traitement des variables
//verifier les leaks avec la manipulaiton des signaux
//Faire les tests sur les operateurs logiques


//revoir la gestion d'erreur au sein des builtins pour qu'elle s'accorde au reste
//cas du heredoc avec le free a corriger

//reprendre tous les builtins pour leaks (ftstdup)
//revoir la gestion d'erreur du here docs
//verifier l'impact de exit si on le met dans une commande avec differents fichiers et redirections
//pb dans l'organisation des operations de pipe en cas de exit /// Quel est le bon fonctionnement ??

//PB DE BUILTINS
//cas des mutiples exit et lancement de minishell dans le minishell
//TRAITER LE CAS DES ./ ET ../ AVEC LA FCT CD AXEL
//cas de l'executeur du shel (reinterpreter ./minishelle en /minishell par ex), pareil les ../ et faire un accesss + pouvoir lire les chemins de fichier qui ramene en arriere (../../...)
//cas special avec le cd ou on peut creer deux dossier puis rm le parent


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
	if (!p || p == NULL)
		return (EXIT_FAILURE);
	g_sig.line = NULL;
	while (g_sig.line == NULL)
	{
		init_signal(0);
		rl_event_hook = event;
		g_sig.line = readline("\033[1;033mMinishell> \033[m");
		if (ft_strcmp(g_sig.line, "end") == 0 && g_sig.sig_quit == 1)
			return (free(g_sig.line), free_all(p), rl_clear_history(), EXIT_SUCCESS);
		if (check_whitespace(g_sig.line) == 1)
			g_sig.sig_int = 1;
		if (g_sig.sig_int == 0 && g_sig.sig_quit == 0)
		{
			add_history(g_sig.line);
			g_sig.p_status = 1;
			p->start = parsingator(g_sig.line, p); //leaks
			if (g_sig.sig_int == 1) // ajouter l'autre var globale ?
				free_start(p->start, 1);
			//print_all(p);
			if (p->start != NULL && g_sig.sig_int == 0)
				executor(p->start, p, g_sig.line);
			free_here_docs(p->here_docs);
		}
		free(g_sig.line);
		g_sig.line = NULL;
	}
	return (rl_clear_history(), free_all(p), EXIT_SUCCESS);
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
