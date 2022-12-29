/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   8-init2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabrielduhau <gabrielduhau@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 12:29:05 by gabrielduha       #+#    #+#             */
/*   Updated: 2022/12/28 12:29:56 by gabrielduha      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int recursive_lst(t_minishell *init, char **cmd, int nb, t_all *p)
{
	t_minishell *new_elem;

	if (!init)
		return (-1);
	init->paths = p->paths;
	init->env = p->env;
	if (check_redirection(cmd[nb], init) == -1)
		return (-1);
	cmd[nb] = erase_redir(cmd[nb]);;
	init->cmd = ft_split(cmd[nb], ' ');
	if (!init->cmd)
		return (-1); //traiter cas erruer
	if (cmd[nb + 1] == NULL)
		return (free_tab(cmd), 1);
	new_elem = malloc(sizeof(t_minishell));
	if (!new_elem)
		return (-1);
	new_elem->next = NULL;
	new_elem->infile = NULL;
	new_elem->outfile = NULL;
	new_elem->cmd = NULL;
	new_elem->append = 0;
	init->next = new_elem;
	return (recursive_lst(new_elem, cmd, ++nb, p));
}

int	init_cmd(t_tree *start, t_all *p)
{
	t_minishell *init_mini;
	char **tab_cmd;

	if (!start || !p || count_pipe(start->cmd) == 0)
		return (-1);
	tab_cmd = ft_split(start->cmd, '|');
	init_mini = malloc(sizeof(t_minishell));
	if (!init_mini)
		return (-1);
	init_mini->next = NULL;
	init_mini->infile = NULL;
	init_mini->outfile = NULL;
	init_mini->cmd = NULL;
	init_mini->append = 0;
	if (recursive_lst(init_mini, tab_cmd, 0, p) == -1)
		return (-1); //free lst
	start->first_elem = init_mini;
	return (1);
}

int	init_shell(t_tree *start, t_all *p)
{
	if (!start)
		return (-1);
	if (init_cmd(start, p) == -1)
		return (-1);
	if (start->and != NULL && init_shell(start->and, p) == -1)
		return (-1);
	if (start->or != NULL && init_shell(start->or, p) == -1)
		return (-1);
	return (1);
}
