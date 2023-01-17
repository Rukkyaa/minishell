/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 00:00:45 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/17 16:25:38 by axlamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int	before(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '+' && str[i + 1] == '=')
			break ;			
		else if (str[i] == '+' && str[i + 1] != '=')
			return (ft_putstr_fd(str, 2), ft_putendl_fd(": not a valid identifier", 2), -1);
		else if (str[i] == '=')
			break ;
		++i;
	}
	return (i);
}

int	change_concat(t_env *env, char *key, char *cmd)
{
	char	*value;
	
	value = NULL;
	if (ft_is_in_env(env, key))
		if (get_env_var(env, key))
			value = ft_strjoin(get_env_var(env, key), cmd + ft_strlen(key) + 2);
	if (!value)
		return (free(key), EXIT_FAILURE);
	change_value(env, key, value);
	free(key);
	return (EXIT_SUCCESS);
}

int	ft_export(t_env *env, char **cmd)
{
	char	*key;
	char	*value;

	if (!cmd[1])
		return (sort_env(env_to_char_export(env)), 0);
	if (before(cmd[1]) == -1)
		return (EXIT_FAILURE);
	key = ft_strndup(cmd[1], before(cmd[1]));
	if (!key)
		return (EXIT_FAILURE);
	if (cmd[1][ft_strlen(key)] == '=' && !(cmd[1][ft_strlen(key) + 1]))
		change_value_equal(env, key);
	else if (!cmd[1][ft_strlen(key)])
		change_value_empty(env, key);
	else
	{
		if (cmd[1][ft_strlen(key)] == '+' && cmd[1][ft_strlen(key) + 1] == '=')
			return (change_concat(env, key, cmd[1]));
		value = ft_strdup(cmd[1] + ft_strlen(key) + 1);
		if (!value)
			return (free(key), EXIT_FAILURE);
		change_value(env, key, value);
	}
	return (EXIT_SUCCESS);
}

