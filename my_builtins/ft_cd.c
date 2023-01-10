/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 23:15:08 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/10 23:54:23 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_in_env(t_env *env, char *key)
{
	if (ft_is_in_env(env, key))
		return (NULL);
	while (ft_strncmp(env->key, key, ft_strlen(key)))
		env = env->next;
	return (env->value);
}

static void	update_oldpw(t_env *env)
{
	char	*str;

	if (ft_is_in_env(env, "OLDPWD"))
	{
		str = ft_strjoin("OLDPWD=", get_in_env(env, "PWD"));
		if (!str)
			return ;
		ft_export(env, str);
	}
}

void	go_home(t_env *env)
{{
	if (ft_is_in_env(env, "HOME"))
	{
		while (ft_strncmp(env->key, "HOME", 4))
			env = env->next;
		chdir(env->value);
	}
}}

int	ft_cd(t_env *env, char *new_cd)
{
	char	**split;

	split = ft_split(new_cd, ' ');
	if (!split)
		return (EXIT_FAILURE);
	if (!(split[1]))
	{
		update_oldpw(env);
		go_home(env);
	}
	return (0);
}