#include "minishell.h"

int	tokens_push(t_token **tokens, int type, char *val, char *addr){
	t_token	*new;
	t_token *tmp;

	//printf("CReating token\n");
	tmp = *tokens;
	new = (t_token*)malloc(sizeof(t_token));
	if (!new)
		exit(1); // make better error
	
	new->type = type;
	new->value = val;
	new->addr= addr; 
	new->next = NULL;
	if (*tokens == NULL){
		*tokens = new;
		return 1;
	}
	while (tmp->next){	
		tmp = tmp->next;
	}
	tmp->next = new;	
	return 1;
}

static	char *select_min(char *s){
	char *min;

	min = ft_strchr(s, '\0'); //actually the biggest
	if (ft_strchr(s, ' ') < min && ft_strchr(s, ' ') != NULL)
		min = ft_strchr(s, ' '); 
	if (ft_strchr(s, '"') < min && ft_strchr(s, '"') != NULL)
		min = ft_strchr(s, '"'); 
	if (ft_strchr(s, '\'') < min && ft_strchr(s, '\'') != NULL)
		min = ft_strchr(s, '\''); 

	return min;
}

static int is_char(char *s){
	int res;

	res = 0;
	if (ft_isprint(*s))
		res=1;
	if (*s == '<' || *s == '>' || *s == '|' || *s =='"' || *s == '\'')
		res = 0;
	//if ()	res = 0; //should i cmp >> and << ?
	return res;	
}

int	inspect_string(char *line, int i, int type, t_token **tokens){
	char	*tmp; 
	char	*val;
	char	c;

	tmp = "";
	val = "";
	c = ' ';

	while(line[i] == ' ')
		i++;
	//should check on ft_isprint, but exclude <>|
	//ft_isalnum(line[i])
	if (is_char(line+i)) { 
		tmp = select_min(line+i);
		c = *tmp;
		*tmp = '\0';
		val = ft_strdup(line + i); // needs to be free
		//printf("VAL:%s\n", val);
		*tmp = c; 
		tokens_push(tokens, type, val, line + i);
		if (type == CMD || type == ARG){
			//printf("ones\n");
			return inspect_string(line, i + ft_strlen(val), ARG, tokens);
		}
	}
	return (i + ft_strlen(val));
}

int	get_next_token(char *line, t_token **tokens){
	int	i;
	
	i = 0;
	if (line[i] == '\'')
		return tokens_push(tokens, Q1, "'", line + i)
			* inspect_string(line, ++i, ARG, tokens);
	if (line[i] == '"')
		return tokens_push(tokens, Q2, "\"", line + i)
			* inspect_string(line, ++i, ARG, tokens);
	if (ft_strncmp(line+i, "<<", 2) == 0)
		return tokens_push(tokens, ININ, "<<", line + i++)
			* inspect_string(line, ++i, DELIMITER, tokens);
	if (ft_strncmp(line+i, ">>", 2) == 0)
		return tokens_push(tokens, OUTOUT, ">>", line + i++)
			* inspect_string(line, ++i, FILEN, tokens);
	if (line[i] == '>')
		return tokens_push(tokens, OUT, ">", line + i)
			* inspect_string(line, ++i, FILEN, tokens);
	if (line[i] == '<')
		return tokens_push(tokens, IN, "<", line + i)
			* inspect_string(line, ++i, FILEN, tokens);
	if (line[i] == '|')
		return (i+1) * tokens_push(tokens, PIPE, "|", line + i);
	return inspect_string(line, i, CMD, tokens);
}

t_token	*lexer(char *line){
	int		i;
	t_token		*tokens; 

	i = 0;
	tokens = NULL;
	while(line[i]){
		while(line[i] == ' ')
			i++;
		i += get_next_token(line + i, &tokens);
	}
	return tokens;
}
