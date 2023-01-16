/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduhau <gduhau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 23:15:08 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/16 21:32:23 by gduhau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_in_env(t_env *env, char *key)
{
	if (!ft_is_in_env(env, key))
		return (NULL);
	while (ft_strncmp(env->key, key, ft_strlen(key)))
		env = env->next;
	return (env->value);
}

static void	update_env(t_env *env, char *old, char *new)
{
	if (ft_is_in_env(env, old))
	{
		while (ft_strcmp(env->key, old))
			env = env->next;
		free(env->value);
		env->value = new;
	}
}

static void	update_pwd(t_env *env)
{
	char	buffer[PATH_MAX];
	char	*pwd;

	if (getcwd(buffer, PATH_MAX))
	{
		pwd = ft_strdup(buffer);
		if (!pwd)
			return ;
		update_env(env, "PWD", pwd);
	}
}

char	*go_option(t_env *env, int flag)
{
	if (flag == 1)
	{
		if (ft_is_in_env(env, "HOME"))
			return (get_in_env(env, "HOME"));
		else
			ft_putendl_fd("cd : HOME not set", 2);
	}
	else
	{
		if (ft_is_in_env(env, "OLDPWD"))
			return (get_in_env(env, "OLDPWD"));
		else
			ft_putendl_fd("cd : OLDPWD not set", 2);
	}
	return (NULL);
}

int	ft_cd(t_env *env, char **split)
{
	char	*oldpwd;
	char	*pwd;

	if (length_tab(split) > 2)
		return (ft_putendl_fd("cd : too many arguments", 2), 1);
	if (ft_is_in_env(env, "PWD"))
		oldpwd = ft_strdup(get_in_env(env, "PWD"));
	else
		oldpwd = ft_strdup("error");
	if (split[1] == NULL)
		pwd = ft_strdup(go_option(env, 1));
	else if (!ft_strcmp(split[1], "-"))
		pwd = ft_strdup(go_option(env, 42424242));
	else
		pwd = ft_strdup(split[1]);
	if (!pwd)
		return (1);
	if (!chdir(pwd))
	{
		update_pwd(env);
		update_env(env, "OLDPWD", oldpwd);
	}
	else
		return (ft_putendl_fd("cd : No such file or directory", 2), 1);
	return (free(pwd), 0);
}
