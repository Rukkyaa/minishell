/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 10:20:56 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/16 15:57:49 by gduhau           ###   ########.fr       */
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
// 	while (1ls )
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

// char	**wildcard(char *extension)
// {
// 	char	**tabl;

// 	if (extension)// *.c
// 		tabl = get_wildcard_extension(".", extension);
// 	else //*
// 		tabl = get_wildcard(".", extension);
// 	if (!tabl)
// 		return (NULL);
// }
