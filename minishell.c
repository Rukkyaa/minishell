/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 11:08:08 by rukkyaa           #+#    #+#             */
/*   Updated: 2022/12/19 12:17:56 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**path_generator(int path, char **env)
{
	if (path == -1)
		return (NULL);
	return (split_path(env[find_path(env)], ':'));
}

int	find_path(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return (-1);
	while (env[i] != NULL)
	{
		if (env[i][0] == 'P' && env[i][1] == 'A' && env[i][2] == 'T'
			&& env[i][3] == 'H')
			return (i);
		i++;
	}
	return (-1);
}

char	**split_path(char *s, char c)
{
	char	**tab;
	int		a;
	int		i;
	int		d;

	i = 0;
	a = 0;
	tab = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!(tab))
		return (0);
	while (a < count_words(s, c) && s[i] != '\0')
	{
		tab[a] = malloc((words_length(s, i, c) + 2) * sizeof(char));
		if (!(tab[a]))
			return (free_tab(tab), (char **)0);
		d = 0;
		while (s[i] == c && s[i] != '\0')
			i++;
		while (s[i] != c && s[i] != '\0')
			tab[a][d++] = s[i++];
		tab[a][d++] = '/';
		tab[a][d] = '\0';
		a++;
	}
	return (tab[a] = 0, tab);
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
	}
	if (line[i + 2] == '\0') //ca veut dire qu'on a parcouru toute la chaine sans trouver un pipe
		exec_command_one(line, paths, env); //cas ou on a une seule commande, reste a traiter les < et > et >>
	//else
	//	pipex();
}

int	main(int argc, char **argv, char **env)
{
	char	*line;
	char **paths;

	//if (argc != 1)
	//	return (0);
	line = NULL;
	paths = path_generator(find_path(env), env);
	if (paths == NULL)
		return (1);
	while (line == NULL)
	{
		line = readline("Minishell> ");
		printf("Line : %s\n", line);
		decomposition(line, paths, env);
		free(line);
		line = NULL;
	}
	return (EXIT_SUCCESS);
}
