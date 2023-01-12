/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 00:00:45 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/12 13:49:17 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	ft_is_in_env(t_env *env, char *str)
{
	while (env)
	{
		if (!ft_strcmp(env->key, str))
			return (true);
		env = env->next;
	}
	return (false);
}

void	change_value(t_env *env, char *key, char *value)
{
	t_env	*new;

	if (ft_is_in_env(env, key))
	{
		while (env)
		{
			if (!ft_strncmp(env->key, key, ft_strlen(key)))
			{
				free(env->value);
				env->value = value;
				return ;
			}
			env = env->next;
		}
	}
	else
	{
		new = ft_envnew(key, value);
		if (!new)
			return ;
		ft_env_add_back(&env, new);
	}
}

int	ft_export(t_env *env, char **split)
{
	char	*key;
	char	*value;

	if (!(split[0]) || !(split[1]))
		return (EXIT_FAILURE);
	key = ft_strdup(split[0]);
	if (!key)
		return (EXIT_FAILURE);
	value = ft_strdup(split[1]);
	if (!value)
		return (free_tab(split), free(key), EXIT_FAILURE);
	change_value(env, key, value);
	return (free_tab(split), EXIT_SUCCESS);
}
