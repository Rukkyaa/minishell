/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_dealer2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 14:40:48 by gatsby            #+#    #+#             */
/*   Updated: 2023/01/26 12:11:59 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	find_other(char *line, int init, int *i, int opt)
{
	int	a;

	a = init + 1;
	if (opt == 1)
		*i = init + 1;
	while (line[a] != '\0')
	{
		if (line[a] == '\"')
			return (a);
		a++;
	}
	return (0);
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

char	*change_line(char *line, char *var, int i, int *leng)
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

static char	*empty_var(char *var)
{
	int	e;

	e = 0;
	while (var[e] != '\0')
		var[e++] = ' ';
	return (var);
}

char	*get_var(char *line, t_env *envp, int i, int *leng)
{
	int		e;
	char	*var;
	t_env	*env;

	e = i + 1;
	env = envp;
	if (line == NULL || env == NULL)
		return (NULL);
	while (line[e] != '\0' && is_whitespace(line[e]) == 0
		&& line[e] != '\"' && line[e] != '\'' && line[e] != '$')
		e++;
	var = ft_substr(line, i + 1, e - i - 1);
	if (!var)
		return (NULL);
	*leng = ft_strlen(var);
	e = -1;
	while (env != NULL)
	{
		if (ft_strncmp(env->key, var, ft_strlen(var)) == 0
			&& (env->code == 1 || env->code == 2 || env->code == 4))
			return (free(var), ft_strdup(env->value));
		env = env->next;
	}
	return (empty_var(var));
}
