/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:08:08 by rukkyaa           #+#    #+#             */
/*   Updated: 2022/12/19 13:40:04 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_filename(char *line, int i) //attention a gerer les quotes
{
	char *text;
	int a;
	int len;

	len = 0;
	while(is_whitespace(line[i]) == 1)
		i++;
	a = i;
	while (ft_isprint(line[a]) == 1)
		a++;
	text = malloc((a - i + 1) * sizeof(char));
	if (!text)
		return (NULL);
	while (i < a)
		text[len++] = line[i++];
	return (text[len] = '\0', text);
}

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

void	exec_command_one(char *line, char **paths, char **env)
{
	t_list *elem;
	
	elem = malloc(sizeof(t_list));
	if (!elem)
		return ;
	elem->redirections = check_redirection(line);
	if (!elem->redirections)
	{
		free(elem);
		return ;
	}
	//trouver le moyen d'extraire proprement la cmd et les options de la string line (complique si il y a des redirections, pt que c'est qch a faire des la fct check redirections)
	//faire une fonction pipe qui redirige les ports si certains elements du tableau redirections ne sont pas nuls;
	printf("line : %s\n<: %s\n>: %s\n>>: %s\n", line, elem->redirections[0], elem->redirections[1], elem->redirections[2]);
	printf("\n %p%p\n", paths, env);
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
	int i = 0;
	while (paths[i])
		printf("%s\n", paths[i++]);
	/*
	if (paths == NULL)
		return (1);
	while (line == NULL)
	{
		line = readline("Minishell> ");
		printf("Line : %s\n", line);
		decomposition(line, paths, env);
		free(line);
		line = NULL;
	}*/
	return (EXIT_SUCCESS);
}
