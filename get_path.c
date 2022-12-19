/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 13:27:26 by gabrielduha       #+#    #+#             */
/*   Updated: 2022/12/19 13:27:58 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//function to get the path from the environment
char    **get_path(char **envp)
{
        int     i;
        char    **path;

        i = -1;
        while (envp[++i])
            if (ft_strncmp(envp[i], "PATH=", 5) == 0)
                    return (split_path(envp[i] + 5, ':'));
        return (NULL);
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