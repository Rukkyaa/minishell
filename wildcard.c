/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 10:20:56 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/20 15:46:43 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static int	midmatch(char *str, char *pattern, int i)
{
	int	e;

	e = 1;
	while (pattern[e] != '\0' && pattern[e + 1] != '\0'
		&& ((pattern[e] != '*' && str[i] == pattern[e])
			|| pattern[e] == '*'))
	{
		i++;
		e++;
	}
	if (pattern[e + 1] != '\0')
		return (0);
	if (pattern[e + 1] == '\0' && pattern[e] == '*')
		return (1);
	if (pattern[e + 1] == '\0' && pattern[e] != '*'
		&& str[i] == pattern[e] && str[i + 1] == '\0')
		return (1);
	return (0);
}

static int	matching(char *str, char *pattern)
{
	int	i;

	i = 0;
	if (!str || str == NULL)
		return (0);
	else if (ft_strlen(pattern) == 1 && pattern[0] == '*')
		return (1);
	else if (pattern[0] != '*' && str[0] != pattern[0])
		return (0);
	else if (pattern[0] != '*' && str[0] == pattern[0])
		return (midmatch(str, pattern, 1));
	else if (pattern[0] == '*' && str[0] == '*')
		return (midmatch(str, pattern, 1));
	else if (pattern[0] == '*')
	{
		while (str[i] != '\0')
		{
			if (str[i] == pattern[1] && midmatch(str, pattern, i) == 1)
				return (1);
			i++;
		}
		return (0);
	}
	return (0);
}

static char	*w_treat(char *str, char *filename)
{
	if (str == NULL)
		return (ft_strdup(filename));
	str = ft_strjoin_spe(str, " ");
	if (str == NULL)
		return (NULL);
	str = ft_strjoin_spe(str, filename);
	if (str == NULL)
		return (NULL);
	return (str);
}

char	*wildcard(char *extension)
{
	DIR				*dir;
	char *str;
	struct dirent	*f;

	dir = opendir(".");
	str = NULL;
	if (!dir || dir == NULL)
		return (free(extension), NULL);
	f = readdir(dir);
	while (f != NULL)
	{
		if (ft_strncmp(f->d_name, ".", 1) != 0
			&& ft_strncmp(f->d_name, "..", 2) != 0
			&& matching(f->d_name, extension) == 1)
		{
			str = w_treat(str, f->d_name);
			if (str == NULL)
				return (closedir(dir), free(extension), NULL);
		}
		f = readdir(dir);
	}
	return (closedir(dir), free(extension), str);
}
