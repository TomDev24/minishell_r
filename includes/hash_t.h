#ifndef HASH_T_H
# define HASH_T_H

#define SLOT_AMOUNT 100

typedef struct s_entry {
	char *key;
	char *value;
	struct s_entry *next;
}		t_entry;

typedef struct s_ht {
	t_entry **entries;
}		t_ht;


unsigned int	hash(char *key);
t_entry		*ht_pair(char *key, char *value);
t_ht		*ht_create();
void		ht_set(t_ht *hashtable, char *key, char *value);
char		*ht_get(t_ht *hashtable, const char *key);
void		ht_del(t_ht *ht, const char *key);
unsigned int	ht_size(t_ht *ht);
char		**hash_to_array(t_ht *ht);
void		print_ht(t_ht *ht);


#endif
