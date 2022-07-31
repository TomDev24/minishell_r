#include "minishell.h"

#define SLOT_AMOUNT 20

void	test(t_ht *ht){
	int	i = 0;
	int 	max = 100;
	char	*str1;
	char	*str2;

	while (i < max){
		str1 = ft_itoa(i + 100);
		ht_set(ht, ft_itoa(i), str1);
		str2 = ht_get(ht, ft_itoa(i));
		//printf("str1 %s  str2 %s\n", str1, str2);
		if (ft_strncmp(str1, str2, ft_strlen(str1)) != 0)
			printf("ERROR: str1 != str2\n");
		i++;
	}
}

void	test_del(t_ht *ht){
	int	i = 50;
	int 	max = 100;
	char	*key;
	char	*res;

	key = NULL;
	res = NULL;
	while(++i < max){
		key = ft_itoa(i);

		res = ht_get(ht, key);
		//printf("Before res: %s\n", res);
		ht_del(ht, key);

		res = ht_get(ht, key);
		//printf("After res: %s\n", res);
		if (res != NULL)
			printf("ERROR: element not deleted\n");
	}
}

int	main(int argc, char **argv){
	t_ht	*ht = ht_create();
	
	printf("SLOT AMOUNT %d\n", SLOT_AMOUNT);
	//ht_set(ht, "path", "/eys/fdss:/fsefw/dds/:ds");
	//printf("Get by key path %s\n", ht_get(ht, "path")); 
	test(ht);
	test_del(ht);
	return 0;
}
