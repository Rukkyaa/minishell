/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   9-redirebisbis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 12:06:34 by axlamber          #+#    #+#             */
/*   Updated: 2023/01/30 12:11:07 by axlamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	clean_rest(t_tree *start, int i, int end)
{
	while (i <= end)
		start->cmd[i++] = ' ';
}

int	increment_i(char *line, int i)
{
	if (line[i] == '\"' || line[i] == '\'')
		return (avoid_quotes(line, i));
	return (++i);
}

int	init_shell(t_tree *start, t_all *p)
{
	int	ret;

	if (!start || start == NULL)
		return (-1);
	ret = init_cmd(start, p);
	if (ret != 1)
		return (-1);
	if (start->and != NULL && init_shell(start->and, p) == -1)
		return (-1);
	if (start->or != NULL && init_shell(start->or, p) == -1)
		return (-1);
	return (1);
}

char	*trimator(char *str)
{
	int		i;
	int		count;
	char	*new_str;

	i = 0;
	count = 0;
	while (i < ft_strlen(str) && str[i] != '\0')
	{
		if (str[i++] != '\v')
			count++;
	}
	new_str = malloc((count + 1) * sizeof(char));
	if (!new_str)
		return (free(str), NULL);
	i = -1;
	count = 0;
	while (++i < ft_strlen(str))
	{
		if (str[i] != '\v')
			new_str[count++] = str[i];
	}
	new_str[count] = '\0';
	return (free(str), new_str);
}

int	find_other(char *line, int init, int *i, int opt)
{
	int	a;

	a = init + 1;
	if (opt == 1)
		*i = init + 1;
	while (line[a] != '\0')
	{
		if (line[a] == '\"')
			return (a);
		a++;
	}
	return (0);
}
