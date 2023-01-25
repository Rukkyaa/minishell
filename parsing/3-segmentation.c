/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3-segmentation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 11:43:39 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/25 11:12:21 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	first_segmentation(t_tree *start, t_tree *init)
{
	int		end;
	char	*reserve;

	if (last_char_spe(start->cmd, '(') == -1 && start->and == NULL && start->or == NULL)
		return (1);
	if (last_char_spe(start->cmd, '(') == -1)
	{
		if (start->or != NULL)
			return (first_segmentation(start->or, init));
		if (start->and != NULL)
			return (first_segmentation(start->and, init));
	}
	reserve = ft_strdup(start->cmd);
	if (!reserve)
		return (-1);
	end = first_char_spe(reserve, ')', last_char_spe(reserve, '('));
	if (end == -1)
		return (free(reserve), -1);
	if (op_segmentation(start, last_char_spe(reserve, '(') + 1, end, reserve) == -1)
		return (free(reserve), -1);
	return (free(reserve), first_segmentation(start, init));
}

// int	op_scd(t_tree *start, int i, char *reserve)
// {
// 	t_tree	*new;

// 	if (reserve[i] == '\0')
// 		return (1);
// 	if (reserve[i] == '\"' || reserve[i] == '\'')
// 		return (op_scd(start, avoid_quotes(reserve, i), reserve));
// 	if (reserve[i] == '&' && reserve[i + 1] == '&')
// 	{
// 		start->cmd = clean_rest_op(start, i);
// 		new = fill_branch(reserve, i + 1);
// 		clean_res(reserve, i);
// 		if (!new)
// 			return (-1);
// 		if (start->and != NULL)
// 			new->and = start->and;
// 		start->and = new;
// 		return (op_scd(start->and, i + 1, reserve));
// 	}
// 	else if (reserve[i] == '|' && reserve[i + 1] == '|')
// 	{
// 		start->cmd = clean_rest_op(start, i);
// 		new = fill_branch(reserve, i + 1);
// 		clean_res(reserve, i);
// 		if (!new)
// 			return (-1);
// 		if (start->or != NULL)
// 			new->or = start->or;
// 		start->or = new;
// 		return (op_scd(start->or, i + 1, reserve));
// 	}
// 	return (op_scd(start, ++i, reserve));
// }

int	scnd_segmentation(t_tree *start, t_tree *init)
{
	char	*reserve;

	if (op_found(start->cmd) == -1 && start->and == NULL && start->or == NULL)
		return (1);
	if (op_found(start->cmd) == -1)
	{
		if (start->or != NULL)
			return (scnd_segmentation(start->or, init));
		if (start->and != NULL)
			return (scnd_segmentation(start->and, init));
	}
	reserve = ft_strdup(start->cmd);
	if (!reserve)
		return (-1);
	if (op_scd(start, 0, reserve) == -1)
		return (free(reserve), -1);
	return (free(reserve), scnd_segmentation(start, init));
}
