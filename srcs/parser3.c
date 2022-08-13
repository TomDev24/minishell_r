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


t_token		*resolve_context(t_stack *context){
	//delete all from context
	context++;
	t_token	*new;

	new = (t_token*)malloc(sizeof(t_token));	
	new->type = 2;
	new->value = "AWESEMOO";
	new->addr= NULL; 
	new->next = NULL;

	return new;
}

t_token	*manage_context(t_token *current, t_token *prev, t_token *next, t_stack **context, t_token **tokens){
	t_token		*replacer;

	if (*context == NULL)
		*context = init_context();

	if ((*context)->q_type == -1 && (current->type == Q1 || current->type == Q2)){ 
		(*context)->q_type = current->type;
		if (prev)
			ft_lstadd_back(&(*context)->elements, ft_lstnew(prev));
		ft_lstadd_back(&(*context)->elements, ft_lstnew(current));
		//if (prev)
		//	tokens_push(&(*context)->elements, prev->type, prev->value, prev->addr);
		//tokens_push(&(*context)->elements, current->type, current->value, current->addr);	
	}
	else if ((*context)->q_type != -1 && (*context)->q_type != current->type)
		ft_lstadd_back(&(*context)->elements, ft_lstnew(current));
	else if((*context)->q_type == current->type){
		ft_lstadd_back(&(*context)->elements, ft_lstnew(current));
		if (next)
			ft_lstadd_back(&(*context)->elements, ft_lstnew(next));
			//tokens_push(&(*context)->elements, next->type, next->value, next->addr);

		//resolve context -> t_token
		replacer = resolve_context(*context);	
		//replace context [PREV, Q1 ... Q1] with token
		replace_token(replacer, context, current, next, tokens);
		//print_tokens((*context)->elements);
		(*context)->q_type = 0;
		*context = NULL;
		return replacer;
	}
	return current;
}

////context LIST[PREV, Q1, ARG, ARG, Q2, Q1, NEXT]
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
