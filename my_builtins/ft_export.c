/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 00:00:45 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/20 15:09:03 by gduhau           ###   ########.fr       */
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
	int		alert;

	alert = 0;
	if (ft_is_in_env(env, key))
	{
		while (env)
		{
			if (!ft_strncmp(env->key, key, ft_strlen(key)))
			{
				free(env->value);
				env->value = ft_trim_quotes(value, &alert);
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
	int		alert;
	
	value = NULL;
	alert = 0;
	if (ft_is_in_env(env, key))
		if (get_env_var(env, key))
			value = ft_strjoin(get_env_var(env, key), ft_trim_quotes(cmd + ft_strlen(key) + 2, &alert));
	if (!value)
		return (free(key), EXIT_FAILURE);
	change_value(env, key, value);
	free(key);
	return (EXIT_SUCCESS);
}

//trim les quotes en sortie des value
//gerer plusieurs export en une cmd 
//+= n'est pas gere si variable non existante

int	ft_export(t_env *env, char **cmd)
{
	char	*key;
	char	*value;
	int		i;
	
	if (!cmd[1])
		return (sort_env(env_to_char_export(env)), 0);
	i = 0;
	while (cmd[++i])
	{
		if (before(cmd[i]) == -1)
			return (1);
		key = ft_strndup(cmd[i], before(cmd[i]));
		if (!key)
			return (1);
		if (cmd[i][ft_strlen(key)] == '=' && !(cmd[i][ft_strlen(key) + 1]))
			change_value_equal(env, key);
		else if (!cmd[i][ft_strlen(key)])
			change_value_empty(env, key);
		else
		{
			if (cmd[i][ft_strlen(key)] == '+' && cmd[i][ft_strlen(key) + 1] == '=')
				return (change_concat(env, key, cmd[i]));
			value = ft_strdup(cmd[i] + ft_strlen(key) + 1);
			if (!value)
				return (free(key), 1);
			change_value(env, key, value);
		}
	}
	return (0);
}
