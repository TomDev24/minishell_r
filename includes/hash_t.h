/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_t.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgregory <cgregory@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 20:09:49 by cgregory          #+#    #+#             */
/*   Updated: 2022/09/07 20:24:11 by dbrittan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASH_T_H
# define HASH_T_H

# define SLOT_AMOUNT 5

typedef struct s_entry{
	char			*key;
	char			*value;
	struct s_entry	*next;
}					t_entry;

typedef struct s_ht{
	t_entry			**entries;
}					t_ht;

/* CORE	*/
unsigned int	hash(char *key);
t_entry			*ht_pair(char *key, char *value);
t_ht			*ht_create(void);
void			ht_delete(t_ht **ht);
void			ht_set(t_ht *hashtable, char *key, char *value);
char			*ht_get(t_ht *hashtable, char *key);
void			ht_del(t_ht *ht, char *key);

/* UTILS */
unsigned int	ht_size(t_ht *ht);
char			**hash_to_array(t_ht *ht, int size_ht);
void			print_ht(t_ht *ht);

#endif
