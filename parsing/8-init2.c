/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   8-init2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 12:29:05 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/30 23:13:27 by gatsby           ###   ########.fr       */
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

char **lsfact(char **tabl)
{
	int	i;
	int	d;

	i = -1;
	while (tabl[++i] != NULL)
	{
		d = -1;
		while (tabl[i][++d] != '\0')
		{
			if (tabl[i][d] == '\\')
				tabl[i][d] = ' ';
		}
	}
	return (tabl);
}

char	*prev_redir(char *cmd)
{
	int	i;

	i = 0;
	while (i < ft_strlen(cmd))
	{
		if (i + 1 < ft_strlen(cmd) && cmd[i] == '<'
			&& cmd[i + 1] == '>')
			cmd[i] = '\v';
		i++;
	}
	return (trimator(cmd));
}

int	recursive_lst(t_minishell *init, char **cmd, int nb, t_all *p)
{
	t_minishell	*new_elem;

	if (!init)
		return (-1);
	init->env = p->env;
	cmd[nb] = prev_redir(cmd[nb]);
	if (check_redirection(cmd[nb], init) == -1)
		return (-1);
	cmd[nb] = erase_redir(cmd[nb]);
	if (que_space(cmd[nb]) == 1)
		init->cmd = tabone(NULL);
	else if (check_spaces(cmd[nb], ' ') == 1)
		init->cmd = lsfact(trim_tab(tabone(ft_strtrim(cmd[nb], " "))));
	else
		init->cmd = lsfact(trim_tab(w_finder(ft_split_spe(cmd[nb], ' '))));
	if (cmd[nb + 1] == NULL)
		return (free_tab(cmd), 1);
	new_elem = malloc(sizeof(t_minishell));
	if (!new_elem)
		return (-1);
	new_elem = parameting(new_elem);
	init->next = new_elem;
	return (recursive_lst(new_elem, cmd, ++nb, p));
}

int	pipe_in(char *str)
{
	int	i;

	i = -1;
	while (++i < ft_strlen(str))
	{
		if (str[i] == '\"' || str[i] == '\'')
			i = avoid_quotes(str, i) - 1;
		else if (str[i] == '|')
			return (1);
	}
	return (0);
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
			return (free_tab(inter), g_sig.cmd_stat = 2, 1);
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
	if (pipe_in(start->cmd) == 1)
	{
		tab_cmd = ft_split_spe(start->cmd, '|');
		if (all_spaces(tab_cmd) == 1)
			return (ft_putstr_fd("syntax error near unexpected token `|'\n", 2),
				free_tab(tab_cmd), 2);
	}
	else
		tab_cmd = tabone(ft_strdup(start->cmd));
	init_mini = malloc(sizeof(t_minishell));
	if (!init_mini)
		return (-1);
	init_mini = parameting(init_mini);
	if (recursive_lst(init_mini, tab_cmd, 0, p) == -1)
		return (free_minishell(init_mini, 1), free_tab(tab_cmd), -1);
	start->first_elem = init_mini;
	return (1);
}
