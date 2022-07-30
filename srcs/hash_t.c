#include "minishell.h"

unsigned int hash(char *key){
	unsigned long int value;
	unsigned int i;
	unsigned int key_len;

	i = -1;
	value = 0;
	key_len = ft_strlen(key);
	while(++i < key_len)
		value = value * 21 + key[i];

	value = value % SLOT_AMOUNT;	
	return value;
}

t_entry	*ht_pair(char *key, char *value) {
	t_entry *entry;

	entry = (t_entry*)malloc(sizeof(t_entry));
	if (!entry)
		exit(1); //make better free
	entry->key = ft_strdup(key); 
	entry->value = ft_strdup(value); 
	entry->next = NULL;	
	return entry;
}


t_ht	*ht_create(){
	t_ht	*ht;
	int	i;

	i = 0;	
	ht = (t_ht*)malloc(sizeof(t_ht));
	ht->entries = (t_entry**)malloc(sizeof(t_entry*) * SLOT_AMOUNT);
	if (!ht->entries)
		exit(1); //better free

	while (i < SLOT_AMOUNT)
		ht->entries[i++] = NULL;
	return ht;
}

void	ht_set(t_ht *ht, char *key, char *value) {
	unsigned int	slot;
	t_entry		*entry;
	t_entry		*prev;

	slot = hash(key);
	entry = ht->entries[slot];
	if (entry == NULL) {
		ht->entries[slot] = ht_pair(key, value);
		return;
	}
	
	while (entry) {
		if (ft_strncmp(entry->key, key, ft_strlen(key) + 1) == 0) {
			free(entry->value);
			entry->value = ft_strdup(value);
			return;
		}
		prev = entry;
		entry = prev->next;
	}
	
	prev->next = ht_pair(key, value);
}

char	*ht_get(t_ht *ht, const char *key) {
	unsigned int	slot;
	t_entry		*entry;  

	slot = hash(key);
	entry = ht->entries[slot];	
	if (entry == NULL)
		return NULL;
	
	while (entry != NULL) {
		if (ft_strncmp(entry->key, key, ft_strlen(key) + 1) == 0)
			return entry->value;
		entry = entry->next;
	}
	return NULL;
}


void		print_ht(t_ht *ht){
	unsigned int	i;

	i = -1;
	while(++i < SLOT_AMOUNT)
		if (ht->entries[i])
			printf("Slot: %d|Key %s|Value: %s\n", i, ht->entries[i]->key, ht->entries[i]->value);
}
