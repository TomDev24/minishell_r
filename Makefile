# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cgregory <cgregory@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/09 18:33:04 by cgregory          #+#    #+#              #
#    Updated: 2022/09/09 20:20:31 by cgregory         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	minishell
LIBFT_DIR		=	./libft/
LIBFT_NAME		=	$(LIBFT_DIR)libft.a
SRCS_DIR		=	./srcs/
SRCS			=	executor/executor.c			executor/free_executor.c\
					executor/redirects.c		executor/utils_executor.c\
					executor/builtins/cd.c		executor/builtins/echo.c\
					executor/builtins/env.c		executor/builtins/exit.c\
					executor/builtins/export.c	executor/builtins/pwd.c\
					executor/builtins/unset.c\
					lexer/lexer.c				lexer/utils_lexer.c\
					main/hash_t.c				main/hash_t2.c\
					main/main.c					main/env.c\
					main/signals.c\
					parser/free_parser.c		parser/parse_env.c\
					parser/parse_qs.c			parser/parser.c\
					parser/utils_parser.c\
					utils/debug.c				utils/errors.c\
					utils/memfree.c				utils/utils.c
HDR_DIR			=	./includes/
M_SHELL			=	minishell.h					hash_t.h
HDR				=	$(addprefix $(HDR_DIR), $(M_SHELL))
OBJ				=	$(addprefix $(SRCS_DIR), $(SRCS:%.c=%.o))
CC				=	gcc
CFLAGS			=	-Wall -Wextra -Werror
.PHONY			:	all clean fclean re
all				:	$(LIBFT_NAME) $(NAME)
$(LIBFT_NAME)	:	
					make all bonus -C $(LIBFT_DIR)
$(NAME)			:	$(OBJ) $(LIBFT_NAME) $(HDR)
					$(CC) $(OBJ) $(LIBFT_NAME) -lreadline -o $(NAME)
%.o				:	%.c $(HDR)
					$(CC) $(CFLAGS) -I$(HDR_DIR) -I$(LIBFT_DIR) -c $< -o $@
clean			:	
					rm -rf $(OBJ)
					make clean -C $(LIBFT_DIR)
fclean			:	clean
					rm -f $(NAME)
					make fclean -C $(LIBFT_DIR)
re				:	fclean all
