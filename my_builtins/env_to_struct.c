/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_struct.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukkyaa <rukkyaa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 00:49:44 by rukkyaa           #+#    #+#             */
/*   Updated: 2022/12/27 02:01:58 by rukkyaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_env_add_back(t_env **lst, t_env *new)
{
	t_env	*tmp;

	if (*lst)
	{
		tmp = ft_envlast(*lst);
		tmp->next = new;
	}
	else
		*lst = new;
}

t_env	*ft_envlast(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

t_env	*ft_envnew(char *key, char *value)
{
	t_env	*list;

	list = malloc(sizeof(t_env));
	if (!list)
		return (NULL);
	list->key = key;
	list->value = value;
	list->next = NULL;
	return (list);
}

char	*ft_strndup(char *str, int n)
{
	char	*dup;
	int		i;

	i = 0;
	dup = malloc(sizeof(char) * (n + 1));
	if (!dup)
		return (NULL);
	while (i < n)
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

t_env	*env_to_struct(char **env, t_env *env_struct)
{
	char			*tmp;
	char			*value;
	t_env			*tmp_env;
	unsigned int	i;

	tmp_env = NULL;
	while (*env)
	{
		i = -1;
		while ((*env)[++i] && (*env)[i] != '=')
			;
		tmp = ft_strndup(*env, i);
		if (!tmp)
			return (NULL); // FREE
		value = ft_strdup(*env + i + 1);
		if (!value)
			return (NULL); // FREE
		tmp_env = ft_envnew(tmp, value);
		if (!tmp_env)
			return (NULL); // FREE
		ft_env_add_back(&env_struct, tmp_env);
		env++;
	}
	return (env_struct);
}
