/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 00:00:45 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/25 10:16:47 by axlamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
			return (ft_putstr_fd(str, 2),
				ft_putendl_fd(": not a valid identifier", 2), -1);
		else if (str[i] == '=')
			break ;
		++i;
	}
	return (i);
}

int	change_concat(t_env *env, char *key, char *cmd)
{
	char	*value;
	char	*key_not_exist;

	value = NULL;
	if (ft_is_in_env(env, key))
	{
		if (get_env_var(env, key))
			value = ft_strjoin(get_env_var(env, key), cmd + ft_strlen(key) + 2);
		if (!value)
			return (free(key), EXIT_FAILURE);
		change_value(env, key, value);
	}
	else
	{
		key_not_exist = ft_strdup(key);
		if (!key_not_exist)
			return (free(key), EXIT_FAILURE);
		value = ft_strdup(cmd + ft_strlen(key) + 2);
		if (!value)
			return (free(key), free(key_not_exist), EXIT_FAILURE);
		change_value(env, key_not_exist, value);
		free(key);
	}
	return (EXIT_SUCCESS);
}

int	do_export(t_env *env, char *key, char *cmd)
{
	char	*value;

	if (cmd[ft_strlen(key)] == '+'
		&& cmd[ft_strlen(key) + 1] == '=')
		return (change_concat(env, key, cmd));
	value = ft_strdup(cmd + ft_strlen(key) + 1);
	if (!value)
		return (free(key), 1);
	change_value(env, key, value);
	return (0);
}

int	ft_export(t_env *env, char **cmd)
{
	char	*key;
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
			return (do_export(env, key, cmd[i]));
	}
	return (0);
}
