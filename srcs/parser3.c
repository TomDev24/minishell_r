#include "minishell.h"

t_stack		*init_context(t_token **tokens){
	t_stack	*res;
	
	res = malloc(sizeof(t_stack));
	res->q_type = -1;
	res->temp_type = '0';
	res->replace = 0;
	res->prev = NULL;
	res->next = NULL;
	res->elements = NULL;
	res->tokens = tokens;

	return res;
}

void	free_context(t_stack **context){
	(*context)->q_type = 0;
	*context = NULL;
}

//Needs refactor // I have some questions about it
t_token	*get_token_by_addr(char *addr, t_token *tokens, int prev_flag){
	t_token *res;
	t_token *prev;
	
	res = NULL;
	prev = NULL;
	while(tokens){
		if (tokens->addr == addr)
			return prev_flag == 1 ? prev : tokens;
		prev = tokens;
		tokens = tokens->next;
	}
	return res;
}

void	replace_token(t_token *replacer, t_stack **context, t_token *current, t_token **tokens){
	t_token		*context_el;

	//first el of context could be PREV or Q1
	context_el = (*context)->elements->content;
	replacer->next = (*context)->next;

	if ((*context)->prev)
		(*context)->prev->next = replacer;
	else if ((*context)->replace == 1){
		*tokens = replacer;
		replacer->type = CMD;
	}
	else if (context_el->type != current->type)
		context_el->next = replacer;
	else
		*tokens = replacer;
}

//st_token=Q1/Q2/PREV en_token=Q1/Q2
int	determine_type(t_token *st_token){
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
	char		*str;

	if (context->q_type != (*s_token)->type)
		*s_token = (*s_token)->next;

	//it will be either way
	//if ((*s_token)->type == context->q_type){
	str = (*s_token)->addr;	
	while(str > (*tokens)->addr && *str != ' ')
		str--;
	if (*str == ' ')
		str++;
	//setting start addres to first TOKEN after ' '(space)
	*s_token = get_token_by_addr(str, *tokens, 0);
	if (*s_token){
		context->prev = get_token_by_addr((*s_token)->addr, *tokens, 1);
		if (!context->prev)
			context->replace = 1; // total replace of tokens
	}	
	//e_token first points to currrent
	while ((*e_token)->next && (*e_token)->next->addr - tkn_eof(*e_token) == 1)
		*e_token = (*e_token)->next; 	
	context->next = (*e_token)->next;
}

t_token	*create_replacer(t_token *st_token){
	t_token	*new;

	new = (t_token*)malloc(sizeof(t_token));
	new->type = determine_type(st_token);
	new->addr= NULL; 
	new->next = NULL;
	return new;
}

void	set_replacer_value(t_stack *context, t_token *st_token, t_token *en_token, char *value){
	int	i;
	char	*en_addr;

	i = 0;
	en_addr = tkn_eof(en_token);
	while(st_token->addr <= en_addr){
		if (context->temp_type == '0' && (*st_token->addr == '"' || *st_token->addr == '\'')){
			context->temp_type = *st_token->addr; 
			st_token->addr++;
		}

		if (*st_token->addr == context->temp_type){
			context->temp_type = '0';
			st_token->addr++;
		}
		//else if(en_addr == st_token->addr && *en_addr == ' ')
		//	st_token->addr++;
		else
			value[i++] = *st_token->addr++;
	}
}

//SHOULD CHOSE CORRECT TYPE AND PARSE VALUE RIGHT
t_token		*resolve_context(t_stack *context, t_token *current, t_token **tokens){
	//delete all from context
	t_token	*new;
	char	*value;
	t_token	*st_token;
	t_token	*en_token;

	st_token = context->elements->content;
	en_token = current;
	new = create_replacer(st_token);
	prep_end_start_addr(context, &st_token, &en_token, tokens);
	value = (char*)ft_calloc((size_t)en_token->addr - (size_t)st_token->addr, sizeof(char));
	if (!value)
		exit(2);

	//can catch error here
	set_replacer_value(context, st_token, en_token, value);
	new->value = value;
	return new;
}

//context LIST[PREV, Q1, ARG, ARG, Q2, Q1, NEXT]
//context LIST[PREV, Q1, ..., Q1,(nospace)NEXT, (nospace)NEXT); should go as one context
t_token	*manage_context(t_token *current, t_token *prev, t_token *next, t_stack **context){
	t_token		*replacer;
	t_token		**tokens;

	replacer = NULL;
	tokens = (*context)->tokens;
	if((*context)->q_type == current->type){
		ft_lstadd_back(&(*context)->elements, ft_lstnew(current));
		if (next)
			ft_lstadd_back(&(*context)->elements, ft_lstnew(next));

		replacer = resolve_context(*context, current, tokens);
		replace_token(replacer, context, current, tokens);
		free_context(context);
	}
	else if ((*context)->q_type == -1 && (current->type == Q1 || current->type == Q2)){ 
		(*context)->q_type = current->type;
		if (prev)
			ft_lstadd_back(&(*context)->elements, ft_lstnew(prev));
		ft_lstadd_back(&(*context)->elements, ft_lstnew(current));
	}
	else if ((*context)->q_type != -1)
		ft_lstadd_back(&(*context)->elements, ft_lstnew(current));
	return replacer != NULL ? replacer : current;
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
		if (context == NULL)
			context = init_context(tokens);
		context->next = next;
		current = manage_context(current, prev, next, &context);
		prev = current;
		current = current->next;
	}
}
