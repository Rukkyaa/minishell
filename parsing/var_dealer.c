/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_dealer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 18:04:39 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/31 10:32:52 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*modifier(char *line, int *i, char *var, int *leng)
{
	if (line == NULL)
		return (NULL);
	if (line[*i] == '$' && var_empt(var) == 1)
		line[*i] = '\v';
	else if (line[*i] == '$')
		line[*i] = ' ';
	else if (line[*i + 1] == '~')
		line[++(*i)] = ' ';
	line = change_line(line, var, *i, leng);
	if (line == NULL)
		return (NULL);
	*i = 0;
	return (line);
}

static char	*quote_modifier(char *line, int *i, char *var, int *leng)
{
	if (line == NULL)
		return (NULL);
	if (line[*i] != '\0' && line[*i] == '$'
		&& line[*i + 1] != '\0' && line[*i + 1] == '?')
		*leng = 1;
	else if (line[*i] == '$' && line[*i + 1] != '\0'
		&& is_whitespace(line[*i + 1]) == 0)
		line[*i] = ' ';
	line = change_line(line, var, *i, leng);
	if (line == NULL)
		return (NULL);
	return (line);
}

static char	*quote_treat(char *li, int *i, int *lg, t_all *p)
{
	int	init;
	int	e;

	init = (*i)++;
	e = find_other(li, init, 0, 0);
	while (li != NULL && *i < e)
	{
		if (check_line(li, i, 1))
			*i += 2;
		else if (quote_cond(li, i, 1) == 1)
			li = special_back(li, i);
		else if (check_line(li, i, 932587))
		{
			li = quote_modifier(li, i, ft_itoa(g_sig.cmd_stat), lg);
			e = find_other(li, init, i, 1);
		}
		else if (quote_cond(li, i, 2) == 1)
		{
			li = quote_modifier(li, i, get_var(li, p->env, *i, lg), lg);
			e = find_other(li, init, i, 1);
		}
		else
			(*i)++;
	}
	return ((*i)++, li);
}

static char	*var_seg(char *li, int *i, int *leng, t_all *p)
{
	if (*i < ft_strlen(li) && li[*i] == ' ' && li[*i + 1] == '~'
		&& (*i == ft_strlen(li) - 2 || li[*i + 2] == ' '))
		li = modifier(li, i, tilde(p->env), leng);
	else if (*i + 1 < ft_strlen(li) && li[*i] == '\\' && li[*i + 1] == '$')
	{
		li[*i] = '\v';
		*i += 2;
	}
	else if (*i < ft_strlen(li) && li[*i] != '\0' && li[*i] == '$'
		&& li[*i + 1] != '\0' && li[*i + 1] == '$')
		*i += 2;
	else if (*i < ft_strlen(li) && li[*i] != '\0' && li[*i] == '$'
		&& li[*i + 1] != '\0' && li[*i + 1] == '?')
	{
		*leng = 1;
		li = modifier(li, i, ft_itoa(g_sig.cmd_stat), leng);
	}
	else if (*i < ft_strlen(li) && li[*i] == '$'
		&& li[*i + 1] != '\0' && potential_name(li[*i + 1]) == 1)
		li = modifier(li, i, get_var(li, p->env, *i, leng), leng);
	else
		(*i)++;
	return (li);
}

char	*replace_var(char *line, t_all *p)
{
	int	i;
	int	leng;

	i = 0;
	leng = 0;
	if (line == NULL)
		return (NULL);
	while (line != NULL && i < ft_strlen(line) && line[i] != '\0')
	{
		if (line[i] == '\"' && find_other(line, i, 0, 0) > i)
			line = quote_treat(line, &i, &leng, p);
		else if (line[i] == '\'')
			i = avoid_quotes_spe(line, i) + 1;
		line = var_seg(line, &i, &leng, p);
	}
	return (trimator(line));
}
