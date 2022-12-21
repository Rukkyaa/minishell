/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 02:37:12 by rukkyaa           #+#    #+#             */
/*   Updated: 2022/12/21 02:39:21 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_redirection(char *str)
{
	char	*infile = NULL;
	char	*outfile = NULL;
	char	*here_doc = NULL;
	char	*append = NULL;

	while (*str)
	{
		if (ft_strlen(str) >= 2 && !ft_strncmp(str, "<<", 2))
			here_doc = get_filename(str++, 2);
		else if (*str == '<')
			infile = get_filename(str, 1);
		else if (*str == '>')
		{
			if (ft_strlen(str) >= 2 && !ft_strncmp(str, ">>", 2))
				append = get_filename(str++, 2);
			else
				outfile = get_filename(str, 1);
		}
		str ++;
	}
	printf("\nInfile : %s\n", infile);
	printf("Outfile : %s\n", outfile);
	printf("Here_doc : %s\n", here_doc);
	printf("Append : %s\n", append);
}
