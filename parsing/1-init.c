/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1-init.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 15:13:57 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/02/01 10:13:23 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_all	*init_env(char **env)
{
	t_all	*p;

	p = malloc(sizeof(t_all));
	if (!p)
		return (NULL);
	p->env = env_to_struct(env);
	if (!p->env)
		return (free(p), NULL);
	p->here_docs = NULL;
	if (create_signal() == -1)
		return (free_all(p), NULL);
	g_sig.cmd_stat = 0;
	return (p);
}

t_tree	*init_tree(char **line)
{
	t_tree	*start;

	if (!line || line == NULL)
		return (NULL);
	if (countof_spe(line, '(', 0, -1) != countof_spe(line, ')', 0, -1)
		|| check_interligne(*line) != 1 || check_extraligne(*line) != 1)
		return (ft_putstr_fd("Syntax error\n", 2), free(*line),
			free(line), NULL);
	*line = clean_first(*line);
	start = init_m_tree(*line);
	if (!start || start == NULL)
		return (free(*line), free(line), NULL);
	if (first_segmentation(start, start) == -1)
		return (free_start(start, 0), free(*line), free(line), NULL);
	if (scnd_segmentation(start, start) == -1)
		return (free_start(start, 0), free(*line), free(line), NULL);
	return (free(*line), free(line), start);
}

t_tree	*init_m_tree(char *str)
{
	t_tree	*start;

	start = malloc(sizeof(t_tree));
	if (!start)
		return (NULL);
	start->cmd = ft_strdup(str);
	if (start->cmd == NULL)
		return (free(start), NULL);
	start->first_elem = NULL;
	start->and = NULL;
	start->or = NULL;
	return (start);
}

t_tree	*fill_branch(char *reserve, int i)
{
	t_tree	*new_elem;

	new_elem = malloc(sizeof(t_tree));
	if (!new_elem)
		return (NULL);
	new_elem->and = NULL;
	new_elem->or = NULL;
	new_elem->cmd = ft_strdup(reserve);
	if (!new_elem->cmd || new_elem->cmd == NULL)
		return (free(new_elem), NULL);
	while (i >= 0)
		new_elem->cmd[i--] = ' ';
	if (invalid_quote(new_elem->cmd) == 1 || invalid_start(new_elem->cmd) == 1)
		return (free(new_elem->cmd), free(new_elem), NULL);
	return (new_elem);
}
