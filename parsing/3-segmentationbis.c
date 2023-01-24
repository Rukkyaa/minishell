/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3-segmentationbis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 10:44:47 by gatsby            #+#    #+#             */
/*   Updated: 2023/01/24 10:45:45 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	op_scd_treat(t_tree *start, int i, char *reserve, t_tree *next)
{
	t_tree	*new;

	start->cmd = clean_rest_op(start, i);
	new = fill_branch(reserve, i + 1);
	clean_res(reserve, i);
	if (!new)
		return (-1);
	if (next != NULL)
		new->and = next;
	next = new;
	return (op_scd(next, i + 1, reserve));
}

int	op_scd(t_tree *start, int i, char *reserve)
{
	if (reserve[i] == '\0')
		return (1);
	if (reserve[i] == '\"' || reserve[i] == '\'')
		return (op_scd(start, avoid_quotes(reserve, i), reserve));
	if (reserve[i] == '&' && reserve[i + 1] == '&')
		return (op_scd_treat(start, i, reserve, start->and));
	else if (reserve[i] == '|' && reserve[i + 1] == '|')
		return (op_scd_treat(start, i, reserve, start->or));
	return (op_scd(start, ++i, reserve));
}

static int	op_seg_and(t_tree *start, int i, int end, char *reserve)
{
	t_tree	*new;

	clean_rest(start, i, end);
	new = fill_branch(reserve, i + 1);
	if (!new)
		return (-1);
	if (start->and != NULL)
		new->and = start->and;
	start->and = new;
	start->cmd[last_char_spe(start->cmd, '(')] = ' ';
	return (op_segmentation(start->and, i + 2, end, start->and->cmd));
}

static int	op_seg_or(t_tree *start, int i, int end, char *reserve)
{
	t_tree	*new;

	clean_rest(start, i, end);
	new = fill_branch(reserve, i + 1);
	if (!new)
		return (-1);
	if (start->or != NULL)
		new->or = start->or;
	start->or = new;
	start->cmd[last_char_spe(start->cmd, '(')] = ' ';
	return (op_segmentation(start->or, i + 2, end, start->or->cmd));
}

int	op_segmentation(t_tree *start, int i, int end, char *reserve)
{
	if (i == end || i + 1 == end)
		return (start->cmd[last_char_spe(start->cmd, '(')] = ' ', start->cmd[end] = ' ', 1);
	if (reserve[i] == '&' && reserve[i + 1] == '&')
		return (op_seg_and(start, i, end, reserve));
	else if (reserve[i] == '|' && reserve[i + 1] == '|')
		return (op_seg_or(start, i, end, reserve));
	return (op_segmentation(start, ++i, end, reserve));
}