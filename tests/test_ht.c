#include "minishell.h"

void	test(t_ht *ht){
	int	i = 0;
	int 	max = 500;
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

int	main(int argc, char **argv){
	t_ht	*ht = ht_create();

	//ht_set(ht, "path", "/eys/fdss:/fsefw/dds/:ds");
	//printf("Get by key path %s\n", ht_get(ht, "path")); 
	test(ht);	
	return 0;
}
