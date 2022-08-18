#include "minishell.h"


t_stack		*init_context(){
	t_stack	*res;
	
	res = malloc(sizeof(t_stack));
	res->q_type = -1;
	res->temp_type = '0';
	res->replace = 0;
	res->prev = NULL;
	res->next = NULL;
	res->elements = NULL;

	return res;
}

void	print_list(t_list *lst){
	t_token	*tkn;

	while(lst){
		tkn = lst->content;
		printf("Token value %s\n", tkn->value);
		lst = lst->next;
	}
}

//Needs refactor // I have some questions about it
t_token	*get_prev_token_by_addr(char *addr, t_token *tokens){
	t_token *res;
	t_token *prev;
	
	res = NULL;
	prev = NULL;
	//printf("ADDR%s\n", addr);
	while(tokens){
		//printf("TOKEN ADDR %s\n", tokens->addr);
		//if (ft_strncmp(tokens->addr, addr, ft_strlen(addr)) == 0){ //(tokens->addr == addr)
		if (tokens->addr == addr){
			return prev;
		}
		prev = tokens;
		tokens = tokens->next;
	}
	return res;
}

t_token	*get_token_by_addr(char *addr, t_token *tokens){
	t_token *res;
	
	res = NULL;
	//printf("ADDR%s\n", addr);
	while(tokens){
		//printf("TOKEN ADDR %s\n", tokens->addr);
		//if (ft_strncmp(tokens->addr, addr, ft_strlen(addr)) == 0){ //(tokens->addr == addr)
		if (tokens->addr == addr){
			return tokens;
		}
		tokens = tokens->next;
	}
	return res;
}

void	replace_token(t_token *replacer, t_stack **context, t_token *current, t_token **tokens){
	t_token		*context_el;

	//first el of context could be PREV or Q1
	context_el = (*context)->elements->content;
	//print_list((*context)->elements);
	if ((*context)->next)
		replacer->next = (*context)->next;

	/*
	printf("c_el val:%s\n",context_el->value);
	printf("diff: %lu\n", context_el->next->addr - context_el->addr );
	printf("f addr %s\n", context_el->addr);
	printf("f type %d\n", context_el->type);	
	printf("f value %s\n", context_el->value);	
	printf("s addr %s\n", context_el->next->addr);	
	printf("s value %s\n", context_el->next->value);
	printf("s type %d\n", context_el->next->type);	
	*/
	//raise(SIGTRAP);
	if ((*context)->prev){
		(*context)->prev->next = replacer;
	}
	else if ((*context)->replace == 1){
		*tokens = replacer;
		replacer->type = CMD;
	}
	else if (context_el->type != current->type){
		//printf("Prev %s\n", context_el->value);
		printf("prev->next = \n");
		//printf("Prev->next %s\n", replacer->value);
		context_el->next = replacer;
	}
	else{
		*tokens = replacer;
	}
}

//st_token=Q1/Q2/PREV en_token=Q1/Q2
int	determine_type(t_token *st_token){
	// DELIMITER FILEN CMD ARG
	if (st_token->type == ININ)
		return DELIMITER;
	if (st_token->type == IN || st_token->type == OUT || st_token->type == OUTOUT)
		return FILEN;
	if ((st_token->type == Q1 || st_token->type == Q2))
		return CMD;
	if (st_token->type == PIPE)
		return CMD;
	return ARG;
}

char	*tkn_eof(t_token *tkn){
	int	i;

	i = 0;
	while(tkn->value[i] && tkn->addr[i] == tkn->value[i])
		i++;
	return tkn->addr + i - 1;
}

void	prep_end_start_addr(t_stack *context, t_token **s_token, t_token **e_token, t_token **tokens){
	t_token		*prev_s_token;
	char		*str;

	prev_s_token = NULL;	
	if (context->q_type != (*s_token)->type){
		prev_s_token = *s_token;
		*s_token = (*s_token)->next;
	}

	//it will be either way
	if ((*s_token)->type == context->q_type){
		str = (*s_token)->addr;	
		while(str > (*tokens)->addr && *str != ' ')
			str--;
		//while(str > (*tokens)->addr && *str == ' ')
		//	str--;
		if (*str == ' ')
			str++;
		//printf("str %s\n", str);
		*s_token = get_token_by_addr(str, *tokens);
		if (*s_token){
			context->prev = get_prev_token_by_addr((*s_token)->addr, *tokens);
			if (!context->prev)
				context->replace = 1;
		}
	}
	
	prev_s_token++;
	//s_token++;
	//s_token we dont change
	//e_token first points to currrent
	while ((*e_token)->next && (*e_token)->next->addr - tkn_eof(*e_token) == 1)
		*e_token = (*e_token)->next; 	
	context->next = (*e_token)->next;
	/*
	while(*(*e_token)->addr && *(*e_token)->addr !=  ' ')
		(*e_token)->addr++; 
	while(*(*e_token)->addr && *(*e_token)->addr == ' ')
		(*e_token)->addr++; 
	if (*(*e_token)->addr) 
		*e_token = get_prev_token_by_addr((*e_token)->addr, *tokens); 
		*e_token = e_token->next;
	//else e_token stays with adress to \0
	*/
}

char	type_to_char(int Q){
	if (Q == Q1)
		return '\'';
	else
		return '"';
}

//SHOULD CHOSE CORRECT TYPE
//SHOULD PARSE VALUE RIGHT
t_token		*resolve_context(t_stack *context, t_token *current, t_token **tokens){
	//delete all from context
	t_token	*new;
	char	*value;
	t_token	*st_token;
	t_token	*en_token;
	char	*en_addr;
	int	i;

	i = 0;
	st_token = context->elements->content;
	en_token = current;
	new = (t_token*)malloc(sizeof(t_token));
	new->type = determine_type(st_token);
	
	prep_end_start_addr(context, &st_token, &en_token, tokens);
	/*while(*en_token->addr && *en_token->addr !=  ' ')
		en_token->addr++; 
	while(*en_token->addr && *en_token->addr == ' ')
		en_token->addr++; 
	if (*en_token->addr){ en_token = get_prev_token_by_addr(en_token->addr, *tokens); 
		en_token = en_token->next;
		context->next = en_token->next;
	}else { context->next = NULL;} */
	//printf("val %lu\n", (size_t)en_token->addr - (size_t)st_token->addr);
	//raise(SIGTRAP);
	value = (char*)ft_calloc((size_t)en_token->addr - (size_t)st_token->addr, sizeof(char));
	if (!value)
		exit(2);
	//cpy from Q to Q
	en_addr = tkn_eof(en_token);
	while(st_token->addr <= en_addr){
		if (context->temp_type == '0' && (*st_token->addr == '"' || *st_token->addr == '\'')){
			context->temp_type = *st_token->addr; 
			st_token->addr++;
		}

		//printf("temp_type %c Char %c\n", context->temp_type, *st_token->addr);
		if (*st_token->addr == context->temp_type){
			context->temp_type = '0';
			st_token->addr++;
		}
		else if(en_addr == st_token->addr && *en_addr == ' ')
			st_token->addr++;
		else
			value[i++] = *st_token->addr++;


	}
	//printf("Value %s$\n", value);
	new->value = value;
	new->addr= NULL; 
	new->next = NULL;

	return new;
}

//LEFT MOST PREV and RIGHT MOST NEXT, determine type of token
//context LIST[PREV, Q1, ARG, ARG, Q2, Q1, NEXT]
//context LIST[PREV, Q1, ..., Q1,(nospace)NEXT, (nospace)NEXT); should go as one context
t_token	*manage_context(t_token *current, t_token *prev, t_token *next, t_stack **context, t_token **tokens){
	t_token		*replacer;

	if (*context == NULL)
		*context = init_context();

	if ((*context)->q_type == -1 && (current->type == Q1 || current->type == Q2)){ 
		(*context)->q_type = current->type;
		if (prev)
			ft_lstadd_back(&(*context)->elements, ft_lstnew(prev));
		ft_lstadd_back(&(*context)->elements, ft_lstnew(current));
	}
	else if ((*context)->q_type != -1 && (*context)->q_type != current->type)
		ft_lstadd_back(&(*context)->elements, ft_lstnew(current));
	else if((*context)->q_type == current->type){
		ft_lstadd_back(&(*context)->elements, ft_lstnew(current));
		if (next)
			ft_lstadd_back(&(*context)->elements, ft_lstnew(next));


		(*context)->next = next;
		//resolve context -> t_token
		replacer = resolve_context(*context, current, tokens);
		//replace context [PREV, Q1 ... Q1] with token
		replace_token(replacer, context, current, tokens);
		//print_tokens((*context)->elements);
		(*context)->q_type = 0;
		*context = NULL;
		return replacer;
	}
	return current;
}

void	unquote(t_token **tokens){
	t_token	*current;
	t_token	*prev;
	t_token	*next;
	t_stack *context;

	current = *tokens;
	context = NULL;
	prev = NULL;
	while(current){
		next = current->next ? current->next : NULL; 
		current = manage_context(current, prev, next, &context, tokens);
		prev = current;
		current = current->next;
	}
}
