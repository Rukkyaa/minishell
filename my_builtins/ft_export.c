/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 00:00:45 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/13 18:01:02 by axlamber         ###   ########.fr       */
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
				env->code = 1;
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
		new->code = 1;
		ft_env_add_back(&env, new);
	}
}

void	change_value_equal(t_env *env, char *key)
{
	t_env	*new;

	if (ft_is_in_env(env, key))
	{
		while (env)
		{
			if (!ft_strncmp(env->key, key, ft_strlen(key)))
			{
				free(env->value);
				env->code = 2;
				return ;
			}
			env = env->next;
		}
	}
	else
	{
		new = ft_envnew(key, NULL);
		if (!new)
			return ;
		new->code = 2;
		ft_env_add_back(&env, new);
	}
}

void	change_value_empty(t_env *env, char *key)
{
	t_env	*new;

	if (!ft_is_in_env(env, key))
	{
		new = ft_envnew(key, NULL);
		if (!new)
			return ;
		new->code = 3;
		ft_env_add_back(&env, new);
	}
}

int	ft_export(t_env *env, char *str)
{
	char	**split;
	char	*key;
	char	*value;

	split = ft_split(str, '=');
	if (!split)
		return (EXIT_FAILURE);
	if (!split[0])
		sort_env(env_to_char_export(env));
	key = ft_strdup(split[0]);
	if (!key)
		return (free_tab(split), EXIT_FAILURE);
	if (!(split[1]) && str[ft_strlen(split[0])] == '=')
		change_value_equal(env, key);
	else if (!(split[1]) && !str[ft_strlen(split[0])])
		change_value_empty(env, key);
	else
	{
		value = ft_strdup(split[1]);
		if (!value)
			return (free_tab(split), free(key), EXIT_FAILURE);
		change_value(env, key, value);
	}
	return (free_tab(split), EXIT_SUCCESS);
}
