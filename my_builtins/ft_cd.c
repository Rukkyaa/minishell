/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axlamber <axlamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 23:15:08 by rukkyaa           #+#    #+#             */
/*   Updated: 2023/01/11 13:57:39 by axlamber         ###   ########.fr       */
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
	else
		printf("Error update env\n");
}

char	*go_home(t_env *env)
{
	if (ft_is_in_env(env, "HOME"))
		return (get_in_env(env, "HOME"));
	return (NULL);
}

char	*go_back(t_env *env)
{
	if (ft_is_in_env(env, "OLDPWD"))
		return (get_in_env(env, "OLDPWD"));
	printf("Error go back\n");
	return (NULL);
}

int	ft_cd(t_env *env, char *new_cd)
{
	char	**split;
	char	*oldpwd;
	char	*pwd;
	
	pwd = NULL;
	split = ft_split(new_cd, ' ');
	if (!split)
		return (EXIT_FAILURE);
	if (ft_is_in_env(env, "PWD"))
		oldpwd = ft_strdup(get_in_env(env, "PWD"));
	else
		oldpwd = ft_strdup("error");
	if (!(split[1]))
		pwd = ft_strdup(go_home(env));
	else if (!ft_strcmp(split[1], "-"))
		pwd = ft_strdup(go_back(env));
	printf("Update OLDPWD : %s\n", oldpwd);
	update_env(env, "OLDPWD", oldpwd);
	printf("pwd : %s\n", pwd);
	if (!chdir(pwd))
	{
		printf("Update PWD : %s\n", pwd);
		update_env(env, "PWD", pwd);
	}
	else
		printf("Error\n");
	return (0);
}