INCLUDES = -I./includes/ -I./libft
SRCS = ./srcs/*.c
LIBFT = ./libft/libft.a 
MAC = -L/Users/dbrittan/.brew/Cellar/readline/8.1.2/lib/ -I/Users/dbrittan/.brew/Cellar/readline/8.1.2/include/readline

all:
	gcc -Wall -g $(INCLUDES) $(SRCS) $(LIBFT) -lreadline -o minishell 
 
test_lexer:
	gcc tests/lexer.c srcs/lexer.c srcs/debug.c $(INCLUDES) $(LIBFT) -lreadline -o test_lexer
	#[[ (./test_lexer "ls > file | cat") == "some"  ]]  && echo "Equal" || echo "Not equal"
	echo "---------SECOND FILE IS ETALON"
	bash ./tests/test_lexer.sh
	rm test_lexer;	

test_parser:
	gcc tests/parser.c srcs/lexer.c srcs/parser.c srcs/debug.c $(INCLUDES) $(LIBFT) -lreadline -o test_parser
	./test_parser;
	echo "\n------FINISH-------\n";
	rm test_parser;	

test_ht:
	gcc tests/test_ht.c srcs/hash_t.c $(INCLUDES) $(LIBFT) -lreadline -o test_ht
	./test_ht
	rm ./test_ht;

clean:
	rm a.out;
	rm -rf a.out.dSYM;
