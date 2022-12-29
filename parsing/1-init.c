/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1-init.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 15:13:57 by gabrielduha       #+#    #+#             */
/*   Updated: 2022/12/29 10:05:47 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_all *init_env(char **env)
{
	t_all *p;

	p = malloc(sizeof(t_all));
	if (!p)
		return (NULL);
	p->paths = split_path(get_env_var(env, "PATH"), ':');
	if (!p->paths)
		return (free(p), NULL);
	p->env = env;
	return (p);
}

t_tree *init_tree(char **line)
{
//	int i;
//	int compt_cmd;
	t_tree	*start;

//	i = 0;
//	compt_cmd = 1;
	if (countof_spe(line, '(', 0) != countof_spe(line, ')', 0))
		return (printf("bash: syntax error\n"), NULL);
	start = init_m_tree(ft_strlen(*line));
	if (!start)
		return (NULL);
	start->cmd = ft_strcpy(start->cmd, *line);
	if (first_segmentation(start, start) == -1)
		return (free_start(start), NULL);
	if (scnd_segmentation(start, start) == -1)
		return (free_start(start), NULL);
	return (start);
}

t_tree *init_m_tree(int length)
{
	t_tree *start;

	start = malloc(sizeof(t_tree));
	if (!start)
		return (NULL);
	start->cmd = ft_calloc(length, sizeof(char));
	start->and = NULL;
	start->or = NULL;
	return (start);
}

t_tree *fill_branch(char *reserve, int i)
{
	t_tree *new_elem;

	new_elem = malloc(sizeof(t_tree));
	if (!new_elem)
		return (NULL);
	new_elem->and = NULL;
	new_elem->or = NULL;
	new_elem->cmd = ft_strdup(reserve);
	if (!new_elem->cmd)
		return (free(new_elem), NULL);
	while (i >= 0)
		new_elem->cmd[i--] = ' ';
	return (new_elem);
}