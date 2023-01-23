/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1-init.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 15:13:57 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/23 11:36:36 by gduhau           ###   ########.fr       */
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
	p->paths = split_path(get_env_var(p->env, "PATH"), ':');
	if (!p->paths)
		p->paths = NULL;
	p->here_docs = NULL;
	if (create_signal() == -1)
		return (free_all(p), NULL);
	g_sig.cmd_stat = 0;
	return ( p);
}

int	check_interligne(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '(')
		{
			while (line[i] != '\0' && line[i + 1] != '\0' && ((line[i] == '&' && line[i + 1] == '&') || (line[i] == '|' && line[i + 1] == '|')))
			{
				if (line[i++] == ')')
					return (0);
			}
			while (line[i] != '\0' && line[i] != ')')
				i++;
			if (line[i] == '\0')
				return (0);
		}
		i++;
	}
	return (1);
}

int	check_extraligne(char *line)
{
	int	i;
	int	count;

	i = 0;
	while (line[i] != '\0')
	{
		count = 0;
		if (line[i] == ')')
		{
			while (line[i] != '\0' && line[i + 1] != '\0' && line[i + 1] != '(')
			{
				if (line[i] != '\0' && line[i + 1] != '\0' && ((line[i] == '&' && line[i + 1] == '&') || (line[i] == '|' && line[i + 1] == '|')))
					count++;
				i++;
			}
			if ((line[i] != '\0' && line[i + 1] != '\0') && count == 0)
				return (0);
		}
		i++;
	}
	return (1);
}

char *clean_first(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0' && is_whitespace(line[i]) == 1)
		i++;
	if (line[i] != '\0' && line[i] == '(')
	{
		line[i] = ' ';
		while (line[i] != '\0' && line[i] != ')')
			i++;
		if (line[i] != '\0' && line[i] == ')')
			line[i] = ' ';
	}
	return (line);
}

t_tree	*init_tree(char **line)
{
	t_tree	*start;

	if (!line || line == NULL)
		return (NULL);
	if (countof_spe(line, '(', 0, -1) != countof_spe(line, ')', 0, -1) || check_interligne(*line) != 1 || check_extraligne(*line) != 1)
		return (printf("Syntax error\n"), free(*line), free(line), NULL);
	*line = clean_first(*line);
	start = init_m_tree(*line);
	if (!start || start == NULL)
		return (free(*line), free(line), NULL);
	start->cmd = ft_strcpy(start->cmd, *line);
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
	return (new_elem);
}
