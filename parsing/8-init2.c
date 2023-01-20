/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   8-init2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 12:29:05 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/19 15:54:54 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_trim_quotes(char *s1, int *alert)
{
	int		i;
	char	*s1_bis;

	i = 0;
	if (!s1)
		return (NULL);
	if (ft_strlen(s1) == 2 && ((s1[0] == '\"' && s1[1] == '\"')
			|| (s1[0] == '\'' && s1[1] == '\'')))
		return (free(s1), NULL);
	if (ft_strlen(s1) > 2 && ((s1[0] == '\'' && s1[ft_strlen(s1) - 1] == '\'')
		|| (s1[0] == '\"' && s1[ft_strlen(s1) - 1] == '\"')))
	{
		s1_bis = malloc((ft_strlen(s1) - 1) * sizeof(char));
		if (!s1_bis)
			return ((*alert) = -1, free(s1), NULL);
		while (s1[++i + 1] != '\0')
			s1_bis[i - 1] = s1[i];
		s1_bis[i - 1] = '\0';
		free(s1);
		return (s1_bis);
	}
	return (s1);
}

char	*ft_trim(char *s1)
{
	int		i;
	char	*s1_bis;

	i = 0;
	if (!s1)
		return (NULL);
	if (ft_strlen(s1) == 2 && ((s1[0] == '\"' && s1[1] == '\"')
			|| (s1[0] == '\'' && s1[1] == '\'')))
		return (NULL);
	if ((s1[0] == '\'' || s1[ft_strlen(s1) - 1] == '\'')
		|| (s1[0] == '\"' || s1[ft_strlen(s1) - 1] == '\"'))
	{
		s1_bis = malloc((ft_strlen(s1) - 1) * sizeof(char));
		if (!s1_bis)
			return (free(s1), NULL);
		while (s1[++i + 1] != '\0')
			s1_bis[i - 1] = s1[i];
		s1_bis[i - 2] = '\0';
		free(s1);
		return (s1_bis);
	}
	return (s1);
}

char	**trim_tab(char **tabl)
{
	int	i;
	int	alert;

	i = -1;
	alert = 0;
	if (!tabl || tabl == NULL)
		return (NULL);
	while (tabl[++i] != NULL)
	{
		tabl[i] = ft_trim_quotes(tabl[i], &alert);
		if (alert == -1)
			return (tabl[i] = NULL, free_tab(tabl), NULL); //risque de leak
	}
	return (tabl);
}

char **tab_generator(char *str)
{
	char **minitab;

	minitab = malloc(2 * sizeof(char *));
	if (!minitab)
		return (NULL);
	minitab[0] = str;
	minitab[1] = NULL;
	return (minitab);
}

int	length_tab(char **tabl)
{
	int i;
	
	i = 0;
	while (tabl[i] != NULL)
		i++;
	return (i);
}

int	w_found(char *str)
{
	int i;

	i = 0;
	if ((str[0] == '\'' || str[ft_strlen(str) - 1] == '\'')
		|| (str[0] == '\"' || str[ft_strlen(str) - 1] == '\"'))
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || str[i] == '\"')
			i = avoid_quotes(str, i);
		if (str[i++] == '*')
			return (1);
	}
	return (0);
}

char *tab_to_str(char **tabl)
{
	char *str;
	int	i;

	i = 0;
	if (!tabl || tabl == NULL)
		return (NULL);
	str = ft_strdup(tabl[0]);
	if (str == NULL && tabl[0] != NULL)
		return (free_tab(tabl), NULL);
	while (tabl[++i] != NULL)
	{
		str = ft_strjoin_spe(str, " ");
		str = ft_strjoin_spe(str, tabl[i]);
		if (str == NULL)
			return (free_tab(tabl), NULL);
	}
	return (free_tab(tabl), str);
}

char *tab_to_str_spe(char **tabl)
{
	char *str;
	int	i;

	i = 0;
	if (!tabl || tabl == NULL)
		return (NULL);
	str = ft_strdup(tabl[0]);
	if (str == NULL && tabl[0] != NULL)
		return (free_tab(tabl), NULL);
	while (tabl[++i] != NULL)
	{
		str = ft_strjoin_spe(str, " ");
		str = ft_strjoin_spe(str, tabl[i]);
		if (str == NULL)
			return (free_tab(tabl), NULL);
	}
	return (free(tabl), str);
}

char	*concat(char *s1)
{
	int	i;
	int	s;

	if (!s1)
		return (NULL);
	s = 0;
	while (s1[s] != '\0' && s1[s] == '*')
		s++;
	if (s > 0)
		s--;
	i = (int)(ft_strlen(s1) - 1);
	while (i > 0 && s1[i] == '*')
		i--;
	if (i != (int)(ft_strlen(s1) - 1))
		i++;
	return (ft_substr(s1, s, i + 1));
}

char **w_finder(char **tabl) //cas particulier du grep a gerer 
{
	int i;
	char **tabfinal;
	char *str;

	i = 0;
	tabfinal = malloc((length_tab(tabl) + 1) * sizeof(char *));
	if (!tabfinal)
		return (free_tab(tabl), NULL);
	while (tabl[i] != NULL)
	{
		if (w_found(tabl[i]) == 1)
		{
			str = wildcard(concat(tabl[i]));
			if (str == NULL)
				tabfinal[i] = ft_strdup(tabl[i]);//tab_generator(tabl[i]); //= fct qui genere un tab;
			else
			{
				tabfinal[i] = ft_strdup(str);
				free(str);
			}
			if (tabfinal[i] == NULL)
				return (free_tab(tabfinal), free_tab(tabl), NULL);
		}
		else
			tabfinal[i] = ft_strdup(tabl[i]);
		if (tabfinal[i++] == NULL)
			return (free_tab(tabfinal), free_tab(tabl), NULL);
	}
	tabfinal[i] = NULL;
	// if (length_tab(tabl) > 0 && megatab[0][1] != NULL) //securite a add
	// 	return (free_megatab(megatab), free_tab(tabl), NULL);
	str = tab_to_str(tabfinal);
	tabfinal = ft_split_spe(str, ' ');
	return (free(str), free_tab(tabl), tabfinal);
}

int	recursive_lst(t_minishell *init, char **cmd, int nb, t_all *p)
{
	t_minishell	*new_elem;

	if (!init)
		return (-1);
	init->paths = p->paths;
	init->env = p->env;
	if (check_redirection(cmd[nb], init) == -1)
		return (-1);
	cmd[nb] = erase_redir(cmd[nb]);
	init->cmd = trim_tab(w_finder(ft_split_spe(cmd[nb], ' ')));
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

int	all_spaces(char **tabl, char *str)
{
	int		i;
	char	**inter;

	i = 0;
	if (tabl == NULL || tabl[i] == NULL)
		return (1);
	while (i < count_words2(str, '|'))
	{
		inter = ft_split_spe(tabl[i], ' ');
		if (*inter != NULL)
			return (free_tab(inter), 0);
		free_tab(inter);
		i++;
	}
	return (1);
}

int	init_cmd(t_tree *start, t_all *p)
{
	t_minishell	*init_mini;
	char		**tab_cmd;

	if (!start || !p || count_pipe(start->cmd) == 0)
		return (-1);
	tab_cmd = ft_split_spe(start->cmd, '|');
	if (all_spaces(tab_cmd, start->cmd) == 1)
		return (printf("syntax error near unexpected token `|'\n"),
			free_tab(tab_cmd), 2);
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
	int	ret;

	if (!start || start == NULL)
		return (-1);
	//
	ret = init_cmd(start, p);
	if (ret != 1)
		return (-1);
	if (start->and != NULL && init_shell(start->and, p) == -1)
		return (-1);
	if (start->or != NULL && init_shell(start->or, p) == -1)
		return (-1);
	return (1);
}
