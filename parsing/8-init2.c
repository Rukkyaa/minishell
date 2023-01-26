/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   8-init2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 12:29:05 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/26 12:16:49 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// char	*ft_trim_quotes(char *s1, int *alert)
// {
// 	int		i;
// 	char	*s1_bis;

// 	i = 0;
// 	if (!s1)
// 		return (NULL);
// 	if (ft_strlen(s1) == 2 && ((s1[0] == '\"' && s1[1] == '\"')
// 			|| (s1[0] == '\'' && s1[1] == '\'')))
// 		return (free(s1), NULL);
// 	if (ft_strlen(s1) > 2 && ((s1[0] == '\'' && s1[ft_strlen(s1) - 1] == '\'')
// 		|| (s1[0] == '\"' && s1[ft_strlen(s1) - 1] == '\"')))
// 	{
// 		s1_bis = malloc((ft_strlen(s1) - 1) * sizeof(char));
// 		if (!s1_bis)
// 			return ((*alert) = -1, free(s1), NULL);
// 		while (s1[++i + 1] != '\0')
// 			s1_bis[i - 1] = s1[i];
// 		s1_bis[i - 1] = '\0';
// 		free(s1);
// 		return (s1_bis);
// 	}
// 	return (s1);
// }

char	*treat_str(char *s1, int *i, char c)
{
	s1[*i] = '\v';
	while (s1[*i] != '\0' && s1[*i] != c)
		(*i)++;
	if (s1[*i] == c)
		s1[*i] = '\v';
	return (s1);
}

// int	check_comma(char *cmd)
// {
// 	int	i;

// 	i = 0;
// 	while (i < ft_strlen(cmd) && is_whitespace(cmd[i]) == 1)
// 		i++;
// 	if (cmd[i] == '\"')
// 	{
// 		while (i < ft_strlen(cmd) && is_whitespace(cmd[i]) == 1)
// 			i++;
// 		if (cmd[i] == '\"')
// 		{
// 			while (i < ft_strlen(cmd) && is_whitespace(cmd[i]) == 1)
// 				i++;
// 			if (i == ft_strlen(cmd))
// 				return (1);
// 		}
// 	}
// 	else if (cmd[i] == '\'')
// 	{
// 		while (i < ft_strlen(cmd) && is_whitespace(cmd[i]) == 1)
// 			i++;
// 		if (cmd[i] == '\'')
// 		{
// 			while (i < ft_strlen(cmd) && is_whitespace(cmd[i]) == 1)
// 				i++;
// 			if (i == ft_strlen(cmd))
// 				return (1);
// 		}
// 	}
// 	return (0);
// }

// char **gen_minitab(char *s)
// {
// 	char **mini;

// 	mini = malloc(2 * sizeof(char *));
// 	if (!mini)
// 		return (NULL);
// 	mini[0] = ft_strdup(s);
// 	if (mini[0] == NULL)
// 		return (free(mini), mini);
// 	mini[1] = NULL;
// 	return (mini);
// }

char	**tabone(char *s)
{
	char	**tabl;

	tabl = malloc(2 * sizeof(char *));
	if (!tabl)
	{
		if (s != NULL)
			free(s);
		return (NULL);
	}
	if (s == NULL)
		tabl[0] = NULL;
	else
		tabl[0] = ft_strdup(s);
	if (s != NULL && tabl[0] == NULL)
		return (free(tabl), free(s), NULL);
	tabl[1] = NULL;
	return (free(s), tabl);
}

int	que_space(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

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
