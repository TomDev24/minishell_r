# MINISHELL-TESTER

RESET="\033[0m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
CYAN="\033[36m"
WHITE="\033[37m"

BOLDRED="\033[1m\033[31m"
BOLDGREEN="\033[1m\033[32m"
BOLDYELLOW="\033[1m\033[33m"
BOLDMAGENTA="\033[1m\033[35m"
BOLDCYAN="\033[1m\033[36m"
BOLDWHITE="\033[1m\033[37m"

# Compile and set executable rights
make -C ../ > /dev/null
cp ../minishell .
cp minishell ./tests/ chmod 755 minishell 

#TEST WITH NO EXIT STATUS !!!!! !!!! !!!
COUNT=0
PASS=0
EXPAND=$1
SHOW=$2

function full_p(){
	for i in {1..20}
	do
	printf $1
	done
}

function test_file(){
	while IFS= read -r line
	do
		test "$line"
	done < $1
}

function test(){
	MY=$(./minishell -c "$1")
	#MY=$(echo $@ "; exit" | ./minishell 2>&-)
	BASH=$(echo $1 "; exit" | bash )
	#BASH=$($1)
		
	(( COUNT++ ))
	if [ "$EXPAND" == "-e" ]; then
		printf $BOLDGREEN"TEST CASE $COUNT:$RESET $CYAN$1$RESET\n"
		if [ "$MY" == "$BASH" ]; then
			full_p " $BOLDGREEN%s$RESET ✓ "
			((PASS=PASS+1))
		else
			full_p " $BOLDRED%s$RESET ✗ "
		fi 
		echo
	else
		printf "|$i"
		if [ "$MY" == "$BASH" ]; then
			printf " $BOLDGREEN%s ✓ $RESET"
			((PASS=PASS+1))
		else
			printf " $BOLDRED%s ✗ $RESET"
		fi 
		printf "|"
	fi	
	if [ "$SHOW" == "-s" ]; then
		echo "$MY% < MY" ; echo "$BASH% < BASH"
	fi
}

printf "\n$BOLWHTIE-----------------------CMDS and PIPES-----------------------$RESET\n"
test_file "main_tester/cmd_pipe"

printf "\n$BOLWHTIE-----------------------QUTATION MARKS TESTS-----------------------$RESET\n"
test_file "main_tester/quotes"

printf "\n$BOLWHTIE-----------------------QUTATION && \$ENV-----------------------$RESET\n"
test_file "main_tester/env" 

printf "\n$BOLDYELLOW/////RESULTS/////$RESET\n"
printf $BOLDYELLOW"PASSED $RESET $PASS / $COUNT $RESET\n"
printf "^^^^^^^^^^^^^^^^\n"

rm minishell; rm ./tests/minishell
