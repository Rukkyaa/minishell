/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_dealer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 18:04:39 by gduhau            #+#    #+#             */
/*   Updated: 2023/01/20 17:41:44 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	avoid_quotes_spe(char *line, int i)
{
	int	ibis;

	ibis = i;
	if (line[i] == '\'')
	{
		i++;
		while (line[i] != '\0' && line[i] != '\'')
			i++;
	}
	if (line[i] == '\0')
		return (++ibis);
	return (++i);
}

static char	*get_var(char *line, t_env *envp, int i, int *leng)
{
	int		e;
	char	*var;
	t_env	*env;

	e = i + 1;
	env = envp;
	if (line == NULL || env == NULL)
		return (NULL);
	while ((line)[e] != '\0' && is_whitespace((line)[e]) == 0
			&& (line)[e] != '\"' && (line)[e] != '\'' && (line)[e] != '$')
		e++;
	var = ft_substr(line, i + 1, e - i - 1);
	if (!var)
		return (NULL);
	*leng = ft_strlen(var);
	e = -1;
	while (env != NULL)
	{
		if (ft_strncmp(env->key, var, ft_strlen(var)) == 0 && (env->code == 1 || env->code == 2 || env->code == 4))
			return (free(var), ft_strdup(env->value));
		env = env->next;
	}
	e = 0;
	while (var[e] != '\0')
		var[e++] = ' ';
	return (var);
}

char	*tilde(t_env *env)
{
	char	*tild;
	char	*subst;

	if (!env || env == NULL)
		return (NULL);
	tild = ft_strdup("HOME");
	if (!tild || tild == NULL)
		return (NULL);
	while (env != NULL)
	{
		if (ft_strncmp(env->key, tild, ft_strlen(tild)) == 0)
			return (free(tild), ft_strdup(env->value));
		env = env->next;
	}
	subst = malloc(2);
	if (!subst)
		return (free(tild), NULL);
	subst[0] = ' ';
	subst[1] = '\0';
	return (free(tild), subst);
}

static char	*change_line(char *line, char *var, int i, int *leng)
{
	char	*new_line;
	char	*reste;

	if (!var || var == NULL)
		return (free(line), NULL);
	new_line = ft_substr(line, 0, i);
	new_line = ft_strjoin_spe(new_line, var);
	if (new_line == NULL || !new_line)
		return (free(line), free(var), NULL);
	reste = ft_substr(line, i + *leng + 1, ft_strlen(line));
	new_line = ft_strjoin_spe(new_line, reste);
	if (reste != NULL)
		free(reste);
	if (new_line == NULL || !new_line)
		return (free(var), free(line), NULL);
	return (free(line), free(var), new_line);
}

int	find_other(char *line, int i)
{
	int	a;

	a = i + 1;
	while (line[a] != '\0')
	{
		if (line[a] == '\"')
			return (a);
		a++;
	}
	return (0);
}

// char *parsing_var(char *line, int *i, int *leng)
// {
// 	int	init;
// 	int	e;
// 				init = i++;
// 			e = find_other(line, i);
// 			while (i < e)
// 			{
// 				if ((line)[*i] != '\0' && (line)[*i] == '$' && (line)[*i + 1] != '\0' && (line)[*i + 1] == '$')
// 					i += 2;
// 				else if ((line)[*i] != '\0' && (line)[*i] == '$' && (line)[*i + 1] != '\0' && (line)[*i + 1] == '?')
// 				{
// 					leng = 1;
// 					line = change_line(line, ft_itoa(g_sig.cmd_stat), i, &leng);
// 					if (line == NULL)
// 						return (NULL);
// 					e = find_other(line, init);
// 					i = init + 1;
// 				}
// 				else if ((line)[i] == '$' && (line)[i + 1] != '\0' && is_whitespace((line)[i + 1]) == 0)
// 				{
// 					(line)[i] = ' ';
// 					line = change_line(line, get_var(line, p->env, i, &leng), i, &leng);
// 					if (line == NULL)
// 						return (NULL);
// 					e = find_other(line, init);
// 					i = init + 1;
// 				}
// 				else
// 					i++;
// 						}
// 			i++;
// }

// char	*replace_var(char *line, t_all *p)
// {
// 	int	i;
// 	int	leng;

// 	i = 0;
// 	leng = 0;
// 	if (line == NULL)
// 		return (NULL);
// 	while ((line)[i] != '\0')
// 	{
// 		if (line[i] == '\"' && find_other(line, i) > i)
// 			line = parsing_var(line, &i, &leng);
// 		else if ((line)[i] == '\'')
// 		{
// 			i = avoid_quotes_spe(line, i);
// 			if (i == ft_strlen(line))
// 				break ;
// 			i++;
// 		}
// 		//if (i < ft_strlen(*line) - 2 && (*line)[i] == ' ' && (*line)[i + 1] == '~' && (((*line)[i + 2] != '\0' && (*line)[i + 2] == ' ') || i == ft_strlen(*line) -1))
// 		if ((line)[i] == ' ' && (line)[i + 1] == '~' && (i == ft_strlen(line) - 2 || (line)[i + 2] == ' '))
// 		{
// 			(line)[++i] = ' ';
// 			line = change_line(line, tilde(p->env), i, &leng);
// 			if (line == NULL)
// 				return (NULL);
// 			i = 0;
// 		}
// 		else if ((line)[i] != '\0' && (line)[i] == '$' && (line)[i + 1] != '\0' && (line)[i + 1] == '$')
// 			i += 2;
// 		else if ((line)[i] != '\0' && (line)[i] == '$' && (line)[i + 1] != '\0' && (line)[i + 1] == '?')
// 		{
// 			leng = 1;
// 			line = change_line(line, ft_itoa(g_sig.cmd_stat), i, &leng);
// 			if (line == NULL)
// 				return (NULL);
// 			i = 0;
// 		}
// 		else if ((line)[i] == '$' && (line)[i + 1] != '\0' && is_whitespace((line)[i + 1]) == 0)
// 		{
// 			(line)[i] = ' ';
// 			line = change_line(line, get_var(line, p->env, i, &leng), i, &leng);
// 			if (line == NULL)
// 				return (NULL);
// 			i = 0;
// 		}
// 		else
// 			i++;
// 	}
// 	return (line);
// }

//BACKUP REPLACE VAR
char	*replace_var(char *line, t_all *p)
{
	int	i;
	int	leng;
	int	e;
	int	init;

	i = 0;
	leng = 0;
	if (line == NULL)
		return (NULL);
	while (line[i] != '\0')
	{
		if (line[i] == '\"' && find_other(line, i) > i)
		{
			init = i++;
			e = find_other(line, init);
			while (i < e)
			{
				if ((line)[i] != '\0' && (line)[i] == '$' && (line)[i + 1] != '\0' && (line)[i + 1] == '$')
					i += 2;
				else if ((line)[i] != '\0' && (line)[i] == '$' && (line)[i + 1] != '\0' && (line)[i + 1] == '?')
				{
					leng = 1;
					line = change_line(line, ft_itoa(g_sig.cmd_stat), i, &leng);
					if (line == NULL)
						return (NULL);
					e = find_other(line, init);
					i = init + 1;
				}
				else if ((line)[i] == '$' && (line)[i + 1] != '\0' && is_whitespace((line)[i + 1]) == 0)
				{
		//if ((*line)[i + 1] != '\0' && (*line)[i + 1] == '?')
			//WHAT THE FUCK
					(line)[i] = ' ';
					line = change_line(line, get_var(line, p->env, i, &leng), i, &leng);
					if (line == NULL)
						return (NULL);
					e = find_other(line, init);
					i = init + 1;
				}
				else
					i++;
			}
			i++;
		}
		else if ((line)[i] == '\'')
		{
			i = avoid_quotes_spe(line, i);
			if (i == ft_strlen(line))
				break ;
			i++;
		}
		//if (i < ft_strlen(*line) - 2 && (*line)[i] == ' ' && (*line)[i + 1] == '~' && (((*line)[i + 2] != '\0' && (*line)[i + 2] == ' ') || i == ft_strlen(*line) -1))
		if ((line)[i] == ' ' && (line)[i + 1] == '~' && (i == ft_strlen(line) - 2 || (line)[i + 2] == ' '))
		{
			(line)[++i] = ' ';
			line = change_line(line, tilde(p->env), i, &leng);
			if (line == NULL)
				return (NULL);
			i = 0;
		}
		else if ((line)[i] != '\0' && (line)[i] == '$' && (line)[i + 1] != '\0' && (line)[i + 1] == '$')
			i += 2;
		else if ((line)[i] != '\0' && (line)[i] == '$' && (line)[i + 1] != '\0' && (line)[i + 1] == '?')
		{
			leng = 1;
			line = change_line(line, ft_itoa(g_sig.cmd_stat), i, &leng);
			if (line == NULL)
				return (NULL);
			i = 0;
		}
		else if ((line)[i] == '$' && (line)[i + 1] != '\0' && is_whitespace((line)[i + 1]) == 0)
		{
		//if ((*line)[i + 1] != '\0' && (*line)[i + 1] == '?')
			//WHAT THE FUCK
			(line)[i] = ' ';
			line = change_line(line, get_var(line, p->env, i, &leng), i, &leng);
			if (line == NULL)
				return (NULL);
			i = 0;
		}
		else
			i++;
		if (i >= ft_strlen(line))
			break;
	}
	return (line);
}