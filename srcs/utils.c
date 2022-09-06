#include "minishell.h"

int	get_last_token_i(t_token *tokens){
	t_token *tkns;
	
	tkns = tokens;
	while(tkns->next)
		tkns = tkns->next;
	return tkns->i;
}

//Needs refactor // I have some questions about it
t_token	*get_token_by_addr(char *addr, t_token *tokens, int prev_flag){
	t_token *res;
	t_token *prev;
	t_token *next;
	
	res = NULL;
	prev = NULL;
	while(tokens){
		next = tokens->next ? tokens->next : NULL; 
		if (tokens->addr == addr){
			if (prev_flag == 1)
				return prev;
			if (prev_flag == 0)
				return tokens;
			if (prev_flag == 2)
				return next;
		}
		prev = tokens;
		tokens = tokens->next;
	}
	return res;
}

char	*tkn_eof(t_token *tkn){
	int	i;

	i = 0;
	while(tkn->value[i] && tkn->addr[i] == tkn->value[i])
		i++;
	return tkn->addr + i - 1;
}

char	type_to_char(int Q){
	if (Q == Q1)
		return '\'';
	else
		return '"';
}

void	python_test(char *line){
	/*
	t_token		*tokens;
	t_cmd		*cmds;

	tokens = NULL;
	cmds = NULL;
	tokens = lexer(line);
	cmds = parser(&tokens);	
	cmds++;
	//executor(cmds);
	*/
	t_token		*tokens;
	t_token		*cur_tokens;
	t_token		*tmp;
	t_cmd		*cmds;
	t_exec		exec;	
	int		cmd_amount;

	tokens = NULL;
	cmds = NULL;
	tokens = lexer(line);
	cmd_amount = get_last_token_i(tokens) + 1;
	cur_tokens = tokens;
	pre_process(&exec, cmd_amount);
	while(cur_tokens){
		if (cur_tokens->next && cur_tokens->next->type == PIPE){
			tmp = cur_tokens->next;
			cur_tokens->next = NULL;
			cmds = parser(&tokens);	
			//print_cmds(cmds);
			executor(cmds, &exec, cmd_amount, cur_tokens->i);
			free_tokens(tokens);
			free_cmds(cmds);
			tokens = tmp->next;
			cur_tokens = tmp->next;
			free_tkn(&tmp);
			continue;
		} else if(!cur_tokens->next){
			cmds = parser(&tokens);	
			executor(cmds, &exec, cmd_amount, cur_tokens->i);
			free_tokens(tokens);
			free_cmds(cmds);
		}
		cur_tokens= cur_tokens->next;
	}		
	post_process(&exec, cmd_amount);

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

