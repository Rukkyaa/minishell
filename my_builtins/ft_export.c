/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 00:00:45 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/16 16:13:38 by gduhau           ###   ########.fr       */
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

	printf("Basic\n");
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

int	before(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] != '=')
		++i;
	return (i);
}

int	ft_export(t_env *env, char **cmd)
{
	char	*key;
	char	*value;

	if (!cmd[1])
		return (sort_env(env_to_char_export(env)), 0);
	key = ft_strndup(cmd[1], before(cmd[1]));
	if (!key)
		return (EXIT_FAILURE);
	if (cmd[1][ft_strlen(key)] == '=' && !(cmd[1][ft_strlen(key) + 1]))
		change_value_equal(env, key);
	else if (!cmd[1][ft_strlen(key)])
		change_value_empty(env, key);
	else
	{
		value = ft_strdup(cmd[1] + ft_strlen(key) + 1);
		if (!value)
			return (free(key), EXIT_FAILURE);
		change_value(env, key, value);
	}
	return (EXIT_SUCCESS);
}
