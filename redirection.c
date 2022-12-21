/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 02:37:12 by rukkyaa           #+#    #+#             */
/*   Updated: 2022/12/21 12:33:53 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_redirection(char *str, t_list *minishell)
{
	char	*here_doc = NULL;

	while (*str)
	{
		if (ft_strlen(str) >= 2 && !ft_strncmp(str, "<<", 2))
			here_doc = get_filename(str++, 2);
		else if (*str == '<')
		{
			printf("Test1\n");
			minishell->infile = open(get_filename(str, 1), O_RDONLY);
			printf("Test2\n");
			if (minishell -> infile == -1)
				printf("minishell: %s: No such file or directory\n", get_filename(str, 1));
		}
		else if (*str == '>')
		{
			if (ft_strlen(str) >= 2 && !ft_strncmp(str, ">>", 2))
				minishell -> outfile = open(get_filename(str++, 2), O_WRONLY | O_APPEND | O_CREAT, 0644);
			else
				minishell -> outfile = open(get_filename(str, 1), O_WRONLY | O_TRUNC | O_CREAT, 0644);
			if (minishell -> outfile == -1)
				printf("minishell: %s: Error when opening file\n", get_filename(str, 1));
		}
		str ++;
	}
	(void) here_doc;
	//if (here_doc)
	//	printf("Here doc : %s\n", here_doc);
	printf("coucou\n");
}
