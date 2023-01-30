/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 10:00:52 by axlamber          #+#    #+#             */
/*   Updated: 2023/01/30 10:04:26 by axlamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** Function to check if the env struct contains the key
**
** @param env: env struct
** @param str: key to check
**
** @return: true if found, false if not
*/
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

/*
** Function to change the give key-value if the input is of the form key=value
** or to add the key-value to the env struct if it doesn't exist
**
** @param env: env struct
** @param key: key to change or add
** @param value: value to change or add
**
** @return: nothing	
*/
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
				free(key);
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
			return (free(key), free(value));
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
			return (free(key));
		new->code = 2;
		ft_env_add_back(&env, new);
	}
}

void	error_cd(char *str)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd(": No such file or directory", 2);
	free(str);
}
