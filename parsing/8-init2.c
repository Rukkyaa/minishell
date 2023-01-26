/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   8-init2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 12:29:05 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/26 17:17:27 by axlamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_minishell	*parameting(t_minishell *new_elem)
{
	new_elem->next = NULL;
	new_elem->file_in = NULL;
	new_elem->file_out = NULL;
	new_elem->cmd = NULL;
	new_elem->append = 0;
	return (new_elem);
}

int	recursive_lst(t_minishell *init, char **cmd, int nb, t_all *p)
{
	t_minishell	*new_elem;

	if (!init)
		return (-1);
	init->env = p->env;
	if (check_redirection(cmd[nb], init) == -1)
		return (-1);
	cmd[nb] = erase_redir(cmd[nb]);
	if (que_space(cmd[nb]) == 1)
		init->cmd = tabone(NULL);
	else if (check_spaces(cmd[nb], ' ') == 1)
		init->cmd = trim_tab(tabone(ft_strtrim(cmd[nb], " ")));
	else
		init->cmd = trim_tab(w_finder(ft_split_spe(cmd[nb], ' ')));
	if (cmd[nb + 1] == NULL)
		return (free_tab(cmd), 1);
	new_elem = malloc(sizeof(t_minishell));
	if (!new_elem)
		return (-1);
	new_elem = parameting(new_elem);
	init->next = new_elem;
	return (recursive_lst(new_elem, cmd, ++nb, p));
}

int	all_spaces(char **tabl)
{
	int		i;
	char	**inter;

	i = 0;
	if (tabl == NULL || tabl[i] == NULL)
		return (1);
	while (tabl[i] != NULL)
	{
		inter = ft_split_spe(tabl[i], ' ');
		if (*inter == NULL)
			return (free_tab(inter), 1);
		free_tab(inter);
		i++;
	}
	return (0);
}

int	init_cmd(t_tree *start, t_all *p)
{
	t_minishell	*init_mini;
	char		**tab_cmd;

	if (!start || !p)
		return (-1);
	if (check_spaces_spe(start->cmd, ' ') == 1)
		tab_cmd = tabone(ft_strdup(start->cmd));
	else
	{
		tab_cmd = ft_split_spe(start->cmd, '|');
		if (all_spaces(tab_cmd) == 1)
			return (printf("syntax error near unexpected token `|'\n"),
				free_tab(tab_cmd), 2);
	}
	init_mini = malloc(sizeof(t_minishell));
	if (!init_mini)
		return (-1);
	init_mini = parameting(init_mini);
	if (recursive_lst(init_mini, tab_cmd, 0, p) == -1)
		return (free_minishell(init_mini, 1), free_tab(tab_cmd), -1);
	start->first_elem = init_mini;
	return (1);
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
