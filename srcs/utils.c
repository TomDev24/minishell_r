#include "minishell.h"

int	cmdlst_size(t_cmd *cmds){
        int i;

        if (!cmds)
                return (0);
        i = 1;
        while (cmds->next != NULL)
        {
                i++;
                cmds = cmds->next;
        }
        return (i);
	
}

char	**sort_array(char **s){
	return NULL;	
}
