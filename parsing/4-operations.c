/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4-operations.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 11:47:45 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/05 12:22:13 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	op_found(char *line)
{
	int i;

	i = 0;
	while (line[i + 1] != '\0')
	{
		if (line[i] == '\"' || line[i] == '\'')
			i = avoid_quotes(line, i);
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