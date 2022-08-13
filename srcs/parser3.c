#include "minishell.h"


t_stack		*init_context(){
	t_stack	*res;
	
	res = malloc(sizeof(t_stack));
	res->q_type = -1;
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

void	replace_token(t_token *replacer, t_stack **context, t_token *current, t_token *next, t_token **tokens){
	t_token		*context_el;

	//first el of context could be PREV or Q1
	context_el = (*context)->elements->content;
	//print_list((*context)->elements);
	if (next)
		replacer->next = next;
	if (context_el->type != current->type)
		context_el->next = replacer;
	else
		*tokens = replacer;

	/*
	if (prev && next && (current->type == Q1 || current->type == Q2)){
		printf("1\n");
		prev->next = next;
		current = prev;
	}
	else if(prev && !next && (current->type == Q1 || current->type == Q2)){ 
		printf("2\n");
		prev->next = NULL;
		return;
	}
	else if (!prev && next && (current->type == Q1 || current->type == Q2)){ 
		//INTERESTING CASE
		printf("3\n");
		*tokens = next;
		current = next;
		continue;
	}
	else if (!prev && !next && (current->type == Q1 || current->type == Q2)){ 
		printf("4\n");
		*tokens = NULL; //free then NULL
		return;
	}*/	
}

//st_token=Q1/Q2/PREV en_token=Q1/Q2
int	determine_type(t_token *st_token, t_token *en_token){
	en_token++;
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

//SHOULD CHOSE CORRECT TYPE
//SHOULD PARSE VALUE RIGHT
t_token		*resolve_context(t_stack *context, t_token *current){
	//delete all from context
	t_token	*new;
	char	*value;
	t_token	*st_token;
	t_token	*en_token;
	int	i;

	i = 0;
	st_token = context->elements->content;
	en_token = current;
	new = (t_token*)malloc(sizeof(t_token));
	new->type = determine_type(st_token, en_token);

	while(context->q_type != st_token->type)
		st_token = st_token->next;
	st_token = st_token->next;
	value = (char*)ft_calloc(en_token->addr - st_token->addr, sizeof(char));		
	//cpy from Q to Q
	while(st_token->addr < en_token->addr)
		value[i++] = *st_token->addr++;

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

		//resolve context -> t_token
		replacer = resolve_context(*context, current);
		//replace context [PREV, Q1 ... Q1] with token
		replace_token(replacer, context, current, next, tokens);
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
