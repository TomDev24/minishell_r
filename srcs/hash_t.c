/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_t.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgregory <cgregory@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 18:28:30 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/06 18:46:25 by cgregory         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned int	hash(char *key)
{
	unsigned long int	value;
	unsigned int		i;
	unsigned int		key_len;

	i = -1;
	value = 0;
	key_len = ft_strlen(key);
	while (++i < key_len)
		value = value * 21 + key[i];
	value = value % SLOT_AMOUNT;
	return (value);
}

t_entry	*ht_pair(char *key, char *value)
{
	t_entry	*entry;

	entry = (t_entry *)malloc(sizeof(t_entry));
	if (!entry)
		m_error(1);
	entry->key = ft_strdup(key);
	entry->value = ft_strdup(value);
	entry->next = NULL;
	return (entry);
}

t_ht	*ht_create(void)
{
	t_ht	*ht;
	int		i;

	i = 0;
	ht = (t_ht *)malloc(sizeof(t_ht));
	if (!ht)
		m_error(1);
	ht->entries = (t_entry **)malloc(sizeof(t_entry *) * SLOT_AMOUNT);
	if (!ht->entries)
		m_error(1);
	while (i < SLOT_AMOUNT)
		ht->entries[i++] = NULL;
	return (ht);
}

void	ht_set(t_ht *ht, char *key, char *value)
{
	unsigned int	slot;
	t_entry			*entry;
	t_entry			*prev;

	slot = hash(key);
	entry = ht->entries[slot];
	if (entry == NULL)
	{
		ht->entries[slot] = ht_pair(key, value);
		return ;
	}
	while (entry)
	{
		if (ft_strncmp(entry->key, key, ft_strlen(key) + 1) == 0)
		{
			free(entry->value);
			entry->value = ft_strdup(value);
			return ;
		}
		prev = entry;
		entry = prev->next;
	}
	prev->next = ht_pair(key, value);
}

char	*ht_get(t_ht *ht, char *key)
{
	unsigned int	slot;
	t_entry			*entry;

	slot = hash(key);
	entry = ht->entries[slot];
	if (entry == NULL)
		return (NULL);
	while (entry != NULL)
	{
		if (ft_strncmp(entry->key, key, ft_strlen(key) + 1) == 0)
			return (entry->value);
		entry = entry->next;
	}
	return (NULL);
}

void	ht_del(t_ht *ht, char *key)
{
	unsigned int	slot;
	t_entry			*entry;
	t_entry			*prev;
	t_entry			*next;

	slot = hash(key);
	entry = ht->entries[slot];
	prev = NULL;
	next = NULL;
	if (!entry)
		return ;
	while (entry)
	{
		next = entry->next;
		if (ft_strncmp(entry->key, key, ft_strlen(key) + 1) == 0)
		{
			free(entry->key);
			free(entry->value);
			free(entry);
			if (prev && next)
				prev->next = next;
			else if (prev && !next)
				prev->next = NULL;
			else if (!prev && next)
				ht->entries[slot] = next;
			else if (!prev && !next)
				ht->entries[slot] = NULL;
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

char	**hash_to_array(t_ht *ht)
{
	unsigned int	i;
	char			**res;
	unsigned int	size_ht;
	int				j;
	t_entry			*tmp;
	char			*tmp2;

	i = -1;
	j = 0;
	size_ht = ht_size(ht);
	res = (char **)malloc(sizeof(char *) * (size_ht + 1));
	if (!res)
		m_error(1);
	while (++i < SLOT_AMOUNT)
	{
		if (!ht->entries[i])
			continue ;
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
