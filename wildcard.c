/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 10:20:56 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/17 11:10:13 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

// static int	nb_of_files_ext(char *current_dir, char *e)
// {
// 	DIR				*dir;
// 	struct dirent	*f;
// 	int				i;

// 	dir = opendir(current_dir);
// 	if (!dir)
// 		return (0);
// 	i = 0;
// 	while (1)
// 	{
// 		f = readdir(dir);
// 		if (!f)
// 			break ;
// 		if (strcmp(f->d_name, ".") && strcmp(f->d_name, ".."))
// 			if (strlen(e) < strlen(f->d_name))
// 				if (!strcmp(f->d_name + strlen(f->d_name) - strlen(e), e))
// 					++i;
// 	}
// 	closedir(dir);
// 	return (i);
// }

// static int	nb_of_files(char *current_dir)
// {
// 	DIR				*dir;
// 	struct dirent	*f;
// 	int				i;

// 	dir = opendir(current_dir);
// 	if (!dir)
// 		return (0);
// 	i = 0;
// 	while (1)
// 	{
// 		f = readdir(dir);
// 		if (!f)
// 			break ;
// 		if (strcmp(f->d_name, ".") && strcmp(f->d_name, ".."))
// 			++i;
// 	}
// 	closedir(dir);
// 	return (i);
// }

// char	**get_wildcard_extension(char *current_dir, char *e)
// {
// 	DIR				*dir;
// 	struct dirent	*f;
// 	int const		nb_file = nb_of_files_ext(current_dir, e);
// 	int				i;
// 	char			**tabl;

// 	dir = opendir(current_dir);
// 	if (!dir)
// 		return (0);
// 	tabl = (char **)malloc((nb_file + 1) * sizeof(char *));
// 	if (!tabl)
// 		return (NULL);
// 	tabl[nb_file] = NULL;
// 	i = 0;
// 	while (1)
// 	{
// 		f = readdir(dir);
// 		if (!f)
// 			break ;
// 		if (strcmp(f->d_name, ".") && strcmp(f->d_name, ".."))
// 		{
// 			if (strlen(e) < strlen(f->d_name))
// 			{
// 				if (!strcmp(f->d_name + strlen(f->d_name) - strlen(e), e))
// 				{
// 					tabl[i] = strdup(f->d_name);
// 					if (!tabl[i++])
// 						return (free_tab(tabl), NULL);
// 				}
// 			}
// 		}
// 	}
// 	closedir(dir);
// 	return (tabl);
// }

// char	**get_wildcard(char *current_dir, char *extension)
// {
// 	DIR				*dir;
// 	struct dirent	*f;
// 	int const		nb_file = nb_of_files(current_dir);
// 	int				i;
// 	char			**tabl;

// 	dir = opendir(current_dir);
// 	if (!dir)
// 		return (0);
// 	tabl = (char **)malloc((nb_file + 1) * sizeof(char *));
// 	if (!tabl)
// 		return (NULL);
// 	tabl[nb_file] = NULL;
// 	i = 0;
// 	while (1)
// 	{
// 		f = readdir(dir);
// 		if (!f)
// 			break ;
// 		if (strcmp(f->d_name, ".") && strcmp(f->d_name, ".."))
// 		{
// 			tabl[i] = strdup(f->d_name);
// 			if (!tabl[i++])
// 				return (NULL);
// 		}
// 	}
// 	closedir(dir);
// 	return (tabl);
// }

// int	matching(char *str, char *pattern, int i, int e)
// {
// 	if (!str || str == NULL)
// 		return (0);
// 	else if (ft_strlen(pattern) == 1 && pattern[0] == '*')
// 		return (1);
// 	if (ft_strlen(pattern) > 1 && pattern[0] == '*' && str[0] == '*')
// 		e = 1;
// 	else if (ft_strlen(pattern) > 1 && pattern[0] == '*')
// 	{
// 		while (str[i] != '\0' && str[i] != pattern[1])
// 			i++;
// 		if (str[i] == pattern[1])
// 			e = 2;
// 		else if (str[i] == '\0')
// 			return (0);
// 	}
// 	else if (str[0] != pattern[0])
// 		return (0);
// 	while (e < ft_strlen(pattern) - 1)
// 	{
// 		//if ((pattern[e] == '*' && str[i] == '*') || (pattern[e] ))
// 		if (pattern[e] != '*' && str[i] != '\0' && str[i] != pattern[e])
// 			return (0);
// 		else if (!(pattern[e] == '*' && str[i] == '*'))
// 			return (0);
// 		e++;
// 		i++;
// 	}
// 	if (pattern[e] == '*' && str[i] != '\0' && str[i] == '*')
// 		return (1);
// 	else if (pattern[e] != '*' && str[i] != '\0' && str[i] != pattern[e])
// 		return (0);
// 	else if (pattern[e] == '*' && str[i] != '\0')
// 		return (1);
// 	return (printf("impossible\n"), 1);
// }
int	matching(char *str, char *pattern, int i, int e);

int	start_pattern(char *str, char *pattern, int i, int e)
{
	if (i == 0 && str[i] == '*')
		return (matching(str, pattern, 1, 1));
	while (str[i] != '\0' && str[i] != pattern[e + 1])
		i++;
	if (str[i] == '\0')
		return (0);
	return (matching(str, pattern, ++i, ++e));
}

int	matching(char *str, char *pattern, int i, int e)
{
	if (!str || str == NULL)
		return (0);
	else if (ft_strlen(pattern) == 1 && pattern[0] == '*')
		return (1);
	else if (str[i] == '\0')
		return (0);
	else if (e == 0 && pattern[0] == '*')
		return (start_pattern(str, pattern, i, e));
	else if (e > 0 && e < ft_strlen(pattern) - 1)
	{
		if (pattern[e] != '*' && str[i] == pattern[e])
			return (matching(str, pattern, ++i, ++e));
		else if (pattern[e] == '*')
			return (matching(str, pattern, ++i, ++e));
		else
			return (0);
	}
	
	// if (e == 0 && pattern[e] == '*')
	// {
	// 	if (i >= 1 && pattern[e + 1] == str[i])
	// 		return (matching(str, pattern, ++i, e + 2));
	// 	else if (str[i] == '*')
	// 		return (matching(str, pattern, ++i, ++e));
	// 	else
	// 		return (matching(str, pattern, ++i, e));
	// }
	// else if (e == 0 && pattern[e] != '*')
	// {
	// 	if (pattern[e] == str[i])
	// 		return(matching(str, pattern, ++i, ++e));
	// 	else
	// 		return (matching(str, pattern, ++i, e));
	// }
	// else if (e != ft_strlen(pattern) - 1)
	// {
	// 	if (pattern[e] == '*')
	// 		return (matching(str, pattern, ++i, ++e));
	// 	else if (pattern[e] != '*' && str[i] == pattern[e])
	// 		return (matching(str, pattern, ++i, ++e));
	// 	else
	// 		return (0);
	// }
	if (e == ft_strlen(pattern) - 1)
	{
		if (pattern[e] == '*' && str[i] != '\0')
			return (1);
		else if (pattern[e] != '*' && str[i] == pattern[e] && i == ft_strlen(str) -1)
			return (1);
		return (0);
	}
	return (0);
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
		if (ft_strncmp(f->d_name, ".", 1) != 0 && ft_strncmp(f->d_name, "..", 2) != 0)// && matching(f->d_name, extension, 0, 0) == 1)
		{
			if (str == NULL)
				str = ft_strdup(f->d_name);
			else
			{
				str = ft_strjoin_spe(str, " "); //gestion erreur
				str = ft_strjoin_spe(str, f->d_name);
			}
			if (str == NULL)
				return (closedir(dir), printf("no found\n"),free(extension), NULL);
			printf("%s\n", str);
		}
		f = readdir(dir);
	}
	closedir(dir);
	return (free(extension), str);
}
