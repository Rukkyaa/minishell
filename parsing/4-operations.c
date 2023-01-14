/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4-operations.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 11:47:45 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/14 21:20:52 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	op_found(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0' && line[i + 1] != '\0')
	{
		if (line[i] != '\0' && (line[i] == '\"' || line[i] == '\''))
		{
			i = avoid_quotes(line, i);
			if (i == ft_strlen(line))
				break ;
		}
		if (line[i] == '&' && line[i + 1] == '&')
			return (i);
		if (line[i] == '|' && line[i + 1] == '|') //possible invalid read
			return (i);
		i++;
	}
	return (-1);
}

char	*clean_rest_op(t_tree *start, int index)
{
	int	i;

	i = index;
	while (start->cmd[i] != '\0')
		start->cmd[i++] = ' ';
	return (start->cmd);
}

void	clean_res(char *reserve, int index)
{
	int	i;

	i = 0;
	while (i < index)
		reserve[i++] = ' ';
}

void	free_all(t_all *p)
{
	free_tab(p->paths);
	free_env(p->env);
	free(p);
}
