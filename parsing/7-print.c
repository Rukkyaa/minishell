/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   7-print.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gatsby <gatsby@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 12:24:51 by gabrielduha       #+#    #+#             */
/*   Updated: 2023/01/26 12:07:34 by gatsby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	print_tree(t_tree *start, int i)
// {
// 	printf("B%d       %s\n", i, start->cmd);
// 	if (start->and != NULL)
// 	{
// 		printf("AND\n");
// 		print_tree(start->and, ++i);
// 	}
// 	if (start->or != NULL)
// 	{
// 		printf("OR\n");
// 		print_tree(start->or, ++i);
// 	}
// }

// void	print_pipe(t_minishell *elem, int i)
// {
// 	int			d;
// 	t_infile	*f1;
// 	t_outfile	*f2;

// 	d = 0;
// 	printf("\n	|PIPE %d\n", i);
// 	while (elem->cmd[d] != NULL)
// 	{
// 		printf("	|cmd[%d]: %s\n", d, elem->cmd[d]);
// 		d++;
// 	}
// 	d = 0;
// 	f1 = elem->file_in;
// 	if (f1 != NULL)
// 	{
// 		while (f1 != NULL)
// 		{
// 			printf("	|Infile[%d] : %s\n", d++, f1->file_in);
// 			f1 = f1->next;
// 		}
// 	}
// 	f2 = elem->file_out;
// 	if (f2 != NULL)
// 	{
// 		while (f2 != NULL)
// 		{
// 			printf("	|Outfile[%d] : %s", d++, f2->file_out);
// 			if (f2->created == 1)
// 				printf("	*created*");
// 			if (f2->append == 1)
// 				printf("	*append*");
// 			printf("\n");
// 			f2 = f2->next;
// 		}
// 	}
// 	printf("\n");
// 	if (elem->next != NULL)
// 		print_pipe(elem->next, ++i);
// }

// void	print_cmd(t_tree *start, int i)
// {
// 	printf("	|OPERATION N^%d---------- (%s)\n", i, start->cmd);
// 	print_pipe(start->first_elem, 1);
// 	printf("	------------------------ \n\n");
// 	if (start->and != NULL)
// 	{
// 		printf("AND\n");
// 		print_cmd(start->and, ++i);
// 	}
// 	if (start->or != NULL)
// 	{
// 		printf("OR\n");
// 		print_cmd(start->or, ++i);
// 	}
// }

// void	print_here_doc(char **here_docs)
// {
// 	int	i;

// 	i = 0;
// 	if (here_docs == NULL)
// 		return ;
// 	printf("-------HERE_DOC-------\n");
// 	while (here_docs[i] != NULL)
// 	{
// 		printf("here_doc[%d] = %s\n", i, here_docs[i]);
// 		i++;
// 	}
// 	printf("here_doc[%d] = %s\n", i, here_docs[i]);
// 	printf("----------------------\n");
// }

// void print_all(t_all *p)
// {
// 	if (p->start == NULL)
// 		return ;
// 	printf("-------AFFICHAGE DES DIFFERENTES BRANCHES-------\n");
// 	print_tree(p->start, 1);
// 	printf("------------------------------------------------\n");
// 	print_cmd(p->start, 1);
// 	print_here_doc(p->here_docs);
// }
