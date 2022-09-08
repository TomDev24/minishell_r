/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_t2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbrittan <dbrittan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 20:07:43 by dbrittan          #+#    #+#             */
/*   Updated: 2022/09/08 13:23:25 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ht_del(t_ht *ht, char *key)
{
	unsigned int	slot;
	t_entry			*entry;
	t_entry			*prev;
	t_entry			*next;

	slot = hash(key);
	entry = ht->entries[slot];
	prev = NULL;
	while (entry)
	{
		next = entry->next;
		if (ft_strncmp(entry->key, key, ft_strlen(key) + 1) == 0)
		{
			free(entry->key);
			free(entry->value);
			free(entry);
			if (prev)
				prev->next = next;
			else if (!prev)
				ht->entries[slot] = next;
		}
		prev = entry;
		entry = entry->next;
	}
}

void	ht_delete(t_ht **ht)
{
	unsigned int	i;
	t_entry			*tmp;
	t_entry			*tmp2;

	i = -1;
	while (++i < SLOT_AMOUNT)
	{
		if ((*ht)->entries[i])
		{
			tmp = (*ht)->entries[i];
			while (tmp)
			{
				tmp2 = tmp;
				tmp = tmp->next;
				free(tmp2->key);
				free(tmp2->value);
				free(tmp2);
			}
		}
	}
	free((*ht)->entries);
	free(*ht);
}

//calculated amount of existing keys (not just slots)
unsigned int	ht_size(t_ht *ht)
{
	unsigned int	i;
	unsigned int	count;
	t_entry			*tmp;

	i = -1;
	count = 0;
	while (++i < SLOT_AMOUNT)
	{
		if (ht->entries[i])
		{
			tmp = ht->entries[i];
			while (tmp)
			{
				count++;
				tmp = tmp->next;
			}
		}
	}
	return (count);
}

char	**hash_to_array(t_ht *ht, int size_ht)
{
	unsigned int	i;
	char			**res;
	int				j;
	t_entry			*tmp;
	char			*tmp2;

	i = -1;
	j = 0;
	res = (char **)malloc(sizeof(char *) * (size_ht + 1));
	if (!res)
		m_error(1, "");
	while (++i < SLOT_AMOUNT)
	{
		tmp = ht->entries[i];
		while (tmp)
		{
			tmp2 = ft_strjoin(tmp->key, "=");
			res[j++] = ft_strjoin(tmp2, tmp->value);
			free(tmp2);
			tmp = tmp->next;
		}
	}
	res[j] = NULL;
	return (res);
}

void	print_ht(t_ht *ht)
{
	unsigned int	i;
	t_entry			*tmp;

	i = -1;
	while (++i < SLOT_AMOUNT)
	{
		if (ht->entries[i])
		{
			tmp = ht->entries[i];
			while (tmp)
			{
				printf("Slot: %d|Key %s|Value: %s\n", i, tmp->key, tmp->value);
				tmp = tmp->next;
			}
		}
	}
}
