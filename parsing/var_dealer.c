/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_dealer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 18:04:39 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/24 14:49:47 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char *modifier(char *line, int *i, char *var, int *leng)
{
	if (line == NULL)
		return (NULL);
	if (line[*i] == '$')
		line[*i] = ' ';
	else if (line[*i + 1] == '~')
		line[++(*i)] = ' ';
	line = change_line(line, var, *i, leng);
	if (line == NULL)
		return (NULL);
	*i = 0;
	return line;
}

static char *quote_modifier(char *line, int *i, char *var, int *leng)
{
	if (line == NULL)
		return (NULL);
	if (line[*i] != '\0' && line[*i] == '$' && line[*i + 1] != '\0' && line[*i + 1] == '?')
		*leng = 1;
	else if (line[*i] == '$' && line[*i + 1] != '\0' && is_whitespace(line[*i + 1]) == 0)
		line[*i] = ' ';
	line = change_line(line, var, *i, leng);
	if (line == NULL)
		return (NULL);
	return (line);
}

static char *quote_treat(char *line, int *i, int *leng, t_all *p)
{
	int init;
	int e;

	init = (*i)++;
	e = find_other(line, init, 0, 0);
	while (line != NULL && *i < e)
	{
		if (line[*i] != '\0' && line[*i] == '$' && line[*i + 1] != '\0' && line[*i + 1] == '$')
			*i += 2;
		else if (line[*i] != '\0' && line[*i] == '$' && line[*i + 1] != '\0' && line[*i + 1] == '?')
		{
			line = quote_modifier(line, i,  ft_itoa(g_sig.cmd_stat), leng);
			e = find_other(line, init, i, 1);
		}
		else if (line[*i] == '$' && line[*i + 1] != '\0' && is_whitespace(line[*i + 1]) == 0)
		{
			line = quote_modifier(line, i,  get_var(line, p->env, *i, leng), leng);
			e = find_other(line, init, i, 1);
		}
		else
			(*i)++;
	}
	return ((*i)++, line);
}

static char *var_seg(char *line, int *i, int *leng, t_all *p)
{
	if (*i < ft_strlen(line) && line[*i] == ' ' && line[*i + 1] == '~' && (*i == ft_strlen(line) - 2 || line[*i + 2] == ' '))
		line = modifier(line, i, tilde(p->env), leng);
	else if (*i < ft_strlen(line) && line[*i] != '\0' && line[*i] == '$' && line[*i + 1] != '\0' && line[*i + 1] == '$')
		*i += 2;
	else if (*i < ft_strlen(line) && line[*i] != '\0' && line[*i] == '$' && line[*i + 1] != '\0' && line[*i + 1] == '?')
	{
		*leng = 1;
		line = modifier(line, i, ft_itoa(g_sig.cmd_stat), leng);
	}
	else if (*i < ft_strlen(line) && line[*i] == '$' && line[*i + 1] != '\0' && is_whitespace(line[*i + 1]) == 0)
		line = modifier(line, i, get_var(line, p->env, *i, leng), leng);
	else
		(*i)++;
	return (line);
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
	return line;
}

//BACKUP REPLACE VAR
// char	*replace_var(char *line, t_all *p)
// {
// 	int	i;
// 	int	leng;
// 	int	e;
// 	int	init;

// 	i = 0;
// 	leng = 0;
// 	if (line == NULL)
// 		return (NULL);
// 	while (line[i] != '\0')
// 	{
// 		if (line[i] == '\"' && find_other(line, i) > i)
// 		{
// 			init = i++;
// 			e = find_other(line, init);
// 			while (i < e)
// 			{
// 				if (line[i] != '\0' && line[i] == '$' && line[i + 1] != '\0' && line[i + 1] == '$')
// 					i += 2;
// 				else if (line[i] != '\0' && line[i] == '$' && line[i + 1] != '\0' && line[i + 1] == '?')
// 				{
// 					leng = 1;
// 					line = change_line(line, ft_itoa(g_sig.cmd_stat), i, &leng);
// 					if (line == NULL)
// 						return (NULL);
// 					e = find_other(line, init);
// 					i = init + 1;
// 				}
// 				else if (line[i] == '$' && line[i + 1] != '\0' && is_whitespace(line[i + 1]) == 0)
// 				{
// 					line[i] = ' ';
// 					line = change_line(line, get_var(line, p->env, i, &leng), i, &leng);
// 					if (line == NULL)
// 						return (NULL);
// 					e = find_other(line, init);
// 					i = init + 1;
// 				}
// 				else
// 					i++;
// 			}
// 			i++;
// 		}
// 		else if (line[i] == '\'')
// 		{
// 			i = avoid_quotes_spe(line, i);
// 			if (i == ft_strlen(line))
// 				break ;
// 			i++;
// 		}
// 		//if (i < ft_strlen(*line) - 2 && (*line)[i] == ' ' && (*line)[i + 1] == '~' && (((*line)[i + 2] != '\0' && (*line)[i + 2] == ' ') || i == ft_strlen(*line) -1))
// 		if (line[i] == ' ' && line[i + 1] == '~' && (i == ft_strlen(line) - 2 || line[i + 2] == ' '))
// 		{
// 			line[++i] = ' ';
// 			line = change_line(line, tilde(p->env), i, &leng);
// 			if (line == NULL)
// 				return (NULL);
// 			i = 0;
// 		}
// 		else if (line[i] != '\0' && line[i] == '$' && line[i + 1] != '\0' && line[i + 1] == '$')
// 			i += 2;
// 		else if (line[i] != '\0' && line[i] == '$' && line[i + 1] != '\0' && line[i + 1] == '?')
// 		{
// 			leng = 1;
// 			line = change_line(line, ft_itoa(g_sig.cmd_stat), i, &leng);
// 			if (line == NULL)
// 				return (NULL);
// 			i = 0;
// 		}
// 		else if (line[i] == '$' && line[i + 1] != '\0' && is_whitespace(line[i + 1]) == 0)
// 		{
// 		//if ((*line)[i + 1] != '\0' && (*line)[i + 1] == '?')
// 			//WHAT THE FUCK
// 			line[i] = ' ';
// 			line = change_line(line, get_var(line, p->env, i, &leng), i, &leng);
// 			if (line == NULL)
// 				return (NULL);
// 			i = 0;
// 		}
// 		else
// 			i++;
// 		if (i >= ft_strlen(line))
// 			break;
// 	}
// 	return line;
// }