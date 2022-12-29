/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4-operations.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 11:47:45 by gabrielduha       #+#    #+#             */
/*   Updated: 2022/12/28 12:42:14 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	op_found(char *line)
{
	int i;

	i = 0;
	while (line[i + 1] != '\0')
	{
		if (line[i] == '&' && line[i + 1] == '&')
			return (i);
		if (line[i] == '|' && line[i + 1] == '|')
			return (i);
		i++;
	}
	return (-1);
}

char *clean_rest_op(t_tree *start, int index)
{
	int i;

	i = index;
	while (start->cmd[i] != '\0')
		start->cmd[i++] = ' ';
	return (start->cmd);
}

void clean_res(char *reserve, int index)
{
	int i;

	i = 0;
	while (i < index)
		reserve[i++] = ' ';
}