#include "minishell.h"

char	type_to_char(int Q){
	if (Q == Q1)
		return '\'';
	else
		return '"';
}

void	python_test(char *line, char **envp){
	t_token		*tokens;
	t_cmd		*cmds;

	tokens = NULL;
	cmds = NULL;
	tokens = lexer(line);
	cmds = parser(&tokens);	
	executor(cmds);
	envp++;
}

int	cmdlst_size(t_cmd *cmds){
        int i;

	i = 1;
        if (!cmds)
                return (0);
        while (cmds->next != NULL)
        {
                i++;
                cmds = cmds->next;
        }
        return (i);
	
}

int	array_size(char **s){
	int	i;

	i = 0;
	while(s[i])
		i++;
	return i;
}

char	**sort_array(char **s){
	int	i;
	int	j;
	int	len_array;
	char	*tmp;
	int	offset;

	i = -1;
	j = 0;
	offset = 0;
	len_array = array_size(s);
	while(++i < len_array - 1){
		while(++j < len_array){
			while(s[i][offset] && s[j][offset] && s[i][offset] == s[j][offset])
				offset++;
			if ( s[i][offset] && s[j][offset] && s[i][offset] > s[j][offset]){
				tmp = s[i];
				s[i] = s[j];
				s[j] = tmp;
			}
			offset = 0;
		}
		j = i + 1;
	}
	return	s;
}

