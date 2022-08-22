# MINISHELL-TESTER

RESET="\033[0m"; RED="\033[31m"; GREEN="\033[32m"; YELLOW="\033[33m"; BLUE="\033[34m"; CYAN="\033[36m"; WHITE="\033[37m";
BOLDRED="\033[1m\033[31m"; BOLDGREEN="\033[1m\033[32m"; BOLDYELLOW="\033[1m\033[33m"; BOLDMAGENTA="\033[1m\033[35m" BOLDCYAN="\033[1m\033[36m"; BOLDWHITE="\033[1m\033[37m"

make -C ../ > /dev/null
cp ../minishell .
cp minishell ./tests/ chmod 755 minishell 

#TEST WITH NO EXIT STATUS !!!!! !!!! !!!
COUNT=0
PASS=0
EXPAND=$1
SHOW=$2
BUFF=0
BUFF_PASS=0

function res(){
	printf "$1"
	printf $BOLDYELLOW"PASSED $RESET $BUFF_PASS / $BUFF  [$PASS] / [$COUNT] $RESET\n"
	printf "^^^^^^^^^^^^^^^^\n"
	BUFF=0
	BUFF_PASS=0
}

function full_p(){
	for i in {1..20}
	do
	printf $1
	done
}

function test_file(){
	while IFS= read -r line
	do
		test "$line" $2
	done < $1
}

#function id is 0
function output_test(){
	MY=$(./minishell -c "$1")
	BASH=$(echo $1 "; exit" | bash )

	if [ "$SHOW" == "-s" ]; then
		echo "$MY# < MY" ; echo "$BASH# < BASH"
	fi
	if [ "$MY" == "$BASH" ]; then return 1; else return 0; fi 
}

function redir_test(){
        cp minishell ./main_tester/my_out/
        cd ./main_tester/my_out;
        $(./minishell -c "$1")
        rm minishell

        cd ../bash_out;
        $(echo $1 "; exit" | bash )
        cd ../..;
	
	MY=$(ls -l ./main_tester/my_out | awk '{print $5, $6, $9}')
	BASH=$(ls -l ./main_tester/bash_out | awk '{print $5, $6, $9}')

	if [ "$SHOW" == "-s" ]; then
		printf "$MY# < MY" ; printf "$BASH# < BASH\n"
	fi
	rm ./main_tester/my_out/*;
        rm ./main_tester/bash_out/*;
	if [ "$MY" == "$BASH" ]; then return 1; else return 0; fi 
}

function test(){	
	(( COUNT++ ))
	(( BUFF++ ))
	if [ "$EXPAND" == "-e" ]; then
		printf $BOLDGREEN"TEST CASE $COUNT:$RESET $CYAN$1$RESET\n"

		if [ "$2" == 0 ]; then output_test "$1"; else redir_test "$1"; fi 	
		RES=$?
		if [ "$RES" == 1 ]; then
			full_p " $BOLDGREEN%s$RESET ✓ "
			((PASS=PASS+1))
			(( BUFF_PASS++ ))
		else
			full_p " $BOLDRED%s$RESET ✗ "
		fi 
		echo
	else
		printf "|$i"
		if [ "$2" == 0 ]; then output_test "$1"; else redir_test "$1"; fi 	
		RES=$?
		if [ "$RES" == 1 ]; then
			printf " $BOLDGREEN%s ✓ $RESET"
			((PASS=PASS+1))
			(( BUFF_PASS++ ))
		else
			printf " $BOLDRED%s ✗ $RESET"
		fi 
		printf "|"
	fi	
}

printf "\n$BOLWHTIE-----------------------CMDS and PIPES-----------------------$RESET\n"
test_file "main_tester/cmd_pipe" 0
res "\n$BOLDMAGENTA/////RESULTS OF: CMDS and PIPES/////$RESET\n"

printf "\n$BOLWHTIE-----------------------QUTATION MARKS TESTS-----------------------$RESET\n"
test_file "main_tester/quotes" 0
res "\n$BOLDMAGENTA/////RESULTS OF: Qutations/////$RESET\n"

printf "\n$BOLWHTIE-----------------------QUTATION && \$ENV-----------------------$RESET\n"
test_file "main_tester/env" 0
res "\n$BOLDMAGENTA/////RESULTS OF: Env & Qutation/////$RESET\n"

printf "\n$BOLWHTIE-----------------------REDIRECTS<<<-----------------------$RESET\n"
test_file "main_tester/redir" 0
res "\n$BOLDMAGENTA/////RESULTS OF: Redirects < /////$RESET\n"

printf "\n$BOLWHTIE-----------------------REDIRECTS>>>-----------------------$RESET\n"
test_file "main_tester/redir2" 1
res "\n$BOLDMAGENTA/////RESULTS OF: Redirects > /////$RESET\n"

res "\n$BOLDYELLOW/////RESULTS/////$RESET\n"
rm minishell; rm ./tests/minishell
