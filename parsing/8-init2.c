/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   8-init2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 12:29:05 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/10 17:26:57 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_trim_quotes(char *s1, int *alert)
{
	int i;
	char *s1_bis;

	i = 0;
	if (!s1)
		return (NULL);
	if (ft_strlen(s1) == 2 && ((s1[0] == '\"' && s1[1] == '\"') || (s1[0] == '\'' && s1[1] == '\'')))
		return (NULL);
	if ((s1[0] == '\'' && s1[ft_strlen(s1) - 1] == '\'') || (s1[0] == '\"' && s1[ft_strlen(s1) - 1] == '\"'))
	{
		s1_bis = malloc((ft_strlen(s1) -1) * sizeof(char));
		if (!s1_bis)
			return ((*alert)++, s1);
		while (s1[++i + 1] != '\0')
			s1_bis[i - 1] = s1[i];
		s1_bis[i - 1] = '\0';
		free(s1);
		return(s1_bis);
	}
	return (s1);
}

char	*ft_trim(char *s1)
{
	int i;
	char *s1_bis;

	i = 0;
	if (!s1)
		return (NULL);
	if (ft_strlen(s1) == 2 && ((s1[0] == '\"' && s1[1] == '\"') || (s1[0] == '\'' && s1[1] == '\'')))
		return (NULL);
	if ((s1[0] == '\'' || s1[ft_strlen(s1) - 1] == '\'') || (s1[0] == '\"' || s1[ft_strlen(s1) - 1] == '\"'))
	{
		s1_bis = malloc((ft_strlen(s1) -1) * sizeof(char));
		if (!s1_bis)
			return (free(s1), NULL);
		while (s1[++i + 1] != '\0')
			s1_bis[i - 1] = s1[i];
		s1_bis[i - 2] = '\0';
		free(s1);
		return(s1_bis);
	}
	return (s1);
}

char **trim_tab(char **tab)
{
	int	i;
	int	alert;

	i = 0;
	alert = 0;
	while (tab[i] != NULL)
	{
		tab[i] = ft_trim_quotes(tab[i], &alert);
		i++;
		if (alert == 1)
			return(free_tab(tab), NULL);
	}
	return(tab);
}

int recursive_lst(t_minishell *init, char **cmd, int nb, t_all *p)
{
	t_minishell *new_elem;

	if (!init)
		return (-1);
	init->paths = p->paths;
	init->env = p->env;
	if (check_redirection(cmd[nb], init) == -1)
		return (-1);
	cmd[nb] = erase_redir(cmd[nb]);
	init->cmd = trim_tab(ft_split_spe(cmd[nb], ' '));
	if (!init->cmd)
		return (-1); //traiter cas erruer
	if (cmd[nb + 1] == NULL)
		return (free_tab(cmd), 1);
	new_elem = malloc(sizeof(t_minishell));
	if (!new_elem)
		return (-1);
	new_elem->next = NULL;
	new_elem->file_in = NULL;
	new_elem->file_out = NULL;
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
	tab_cmd = ft_split_spe(start->cmd, '|');
	init_mini = malloc(sizeof(t_minishell));
	if (!init_mini)
		return (-1);
	init_mini->next = NULL;
	init_mini->file_in = NULL;
	init_mini->file_out = NULL;
	init_mini->cmd = NULL;
	init_mini->append = 0;
	if (recursive_lst(init_mini, tab_cmd, 0, p) == -1)
		return (-1); //free lst
	start->first_elem = init_mini;
	return (1);
}

int	init_shell(t_tree *start, t_all *p)
{
	if (!start || start == NULL)
		return (-1);
	if (init_cmd(start, p) == -1)
		return (-1);
	if (start->and != NULL && init_shell(start->and, p) == -1)
		return (-1);
	if (start->or != NULL && init_shell(start->or, p) == -1)
		return (-1);
	return (1);
}
