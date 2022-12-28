/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 02:37:12 by rukkyaa           #+#    #+#             */
/*   Updated: 2022/12/26 16:27:06 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

/*
** This function get the redirection, and open the file with the right flags
** to stock the fd in the struct
** It also get the here_doc if there is one
**
** @param str: the string to parse
** @param minishell: the struct to stock the fd
**
** @return void
*/
//int	get_redirection(char *str, t_minishell *minishell)
//{
//	char	*here_doc = NULL;

//	while (*str)
//	{
//		if (ft_strlen(str) >= 2 && !ft_strncmp(str, "<<", 2))
//			here_doc = get_filename(str++, 2);
//		else if (*str == '<')
//		{
//			minishell->infile = open(get_filename(str, 1), O_RDONLY);
//			if (minishell -> infile == -1)
//				return (printf("minishell: %s: No such file or directory\n", get_filename(str, 1)), -1);
//		}
//		else if (*str == '>')
//		{
//			if (ft_strlen(str) >= 2 && !ft_strncmp(str, ">>", 2))
//				minishell -> outfile = open(get_filename(str++, 2), O_WRONLY | O_APPEND | O_CREAT, 0644);
//			else
//				minishell -> outfile = open(get_filename(str, 1), O_WRONLY | O_TRUNC | O_CREAT, 0644);
//			if (minishell -> outfile == -1)
//				return (printf("minishell: %s: Error when opening file\n", get_filename(str, 1)), -1);
//		}
//		str ++;
//	}
//	if (here_doc)
//		printf("Here doc : %s\n", here_doc);
//	return (0);
//}
