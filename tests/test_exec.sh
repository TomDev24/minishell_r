# MINISHELL-TESTER

RESET="\033[0m"
BLACK="\033[30m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
MAGENTA="\033[35m"
CYAN="\033[36m"
WHITE="\033[37m"

BOLDBLACK="\033[1m\033[30m"
BOLDRED="\033[1m\033[31m"
BOLDGREEN="\033[1m\033[32m"
BOLDYELLOW="\033[1m\033[33m"
BOLDBLUE="\033[1m\033[34m"
BOLDMAGENTA="\033[1m\033[35m"
BOLDCYAN="\033[1m\033[36m"
BOLDWHITE="\033[1m\033[37m"

# Compile and set executable rights
make -C ../ > /dev/null
cp ../minishell .
cp minishell ./tests/

chmod 755 minishell


function exec_test()
{
	TEST1=$(echo $@ "; exit" | ./minishell 2>&-)
	ES_1=$?
	TEST2=$(echo $@ "; exit" | bash 2>&-)
	ES_2=$?
	if [ "$TEST1" == "$TEST2" ] && [ "$ES_1" == "$ES_2" ]; then
		printf " $BOLDGREEN%s$RESET" "✓ "
	else
		printf " $BOLDRED%s$RESET" "✗ "
	fi
	printf "$CYAN \"$@\" $RESET"
	if [ "$TEST1" != "$TEST2" ]; then
		echo
		echo
		printf $BOLDRED"Your output : \n%.20s\n$BOLDRED$TEST1\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
		printf $BOLDGREEN"Expected output : \n%.20s\n$BOLDGREEN$TEST2\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
	fi
	if [ "$ES_1" != "$ES_2" ]; then
		echo
		echo
		printf $BOLDRED"Your exit status : $BOLDRED$ES_1$RESET\n"
		printf $BOLDGREEN"Expected exit status : $BOLDGREEN$ES_2$RESET\n"
	fi
	echo
	sleep 0.1
}

#TEST WITH NO EXIT STATUS !!!!! !!!! !!!
function test(){
	MY=$(./minishell -c "$1")
	#BASH=$($1)
	BASH=$(echo $@ "; exit" | bash 2>&-)
		
	printf $BOLDGREEN"TEST CASE:$RESET $CYAN$1$RESET\n"
	#echo "$MY"
	#echo "$BASH"
	if [ "$MY" == "$BASH" ]; then
		for i in {1..20}
		do
		printf " $BOLDGREEN%s$RESET" "✓ "
		done
	else
		for i in {1..20}
		do
		printf " $BOLDRED%s$RESET" "✗ "
		done
	fi 
	echo
}

test 'pwd'
test 'echo'
test 'echo this text yes'
test 'echo -n'
test 'echo -n some val'
test "pwd | echo"
test 'pwd | echo | wc'
test 'pwd | echo | wc | wc'
test 'ls'
test 'whoami'
test 'ls | head'
test 'ls | head | wc'
test 'pwd | cat'
test 'pwd | cat | echo yes'
test 'echo | pwd | echo | pwd | env | pwd | echo | env | head | echo | pwd'
test 'env | head -10 | head -10 | echo some text val yes | pwd | env | grep P | wc | wc'
test 'env'



printf " $BOLWHTIE-----------------------QUTATION MARKS TESTS-----------------------$RESET\n"

test "echo \"some va '\""
#test "echo some va '\""
test "echo \"'\" \"val '\""
test "echo \"''\"" 
test "echo 'ad e' 'ads \" s \"\"'"

test "echo \"\""

#echo "         ' ' ' fsdf dsaf '''  d
#test "echo '     \"\" d\" dsd '  'ss ss \"  \" \"'"

test "echo 'as 'a''"
test "echo 'as 'a'a'"
test "echo \"asd '\"  \"ad\""
test "echo \"< no pipe | or semicolon will ; stop me >\""
test "echo \"some ' text\" 'a d' 'sd\"ds\"\"' 'ds\"sd\" d'"

printf " $BOLWHTIE-----------------------QUTATION && \$ENV-----------------------$RESET\n"
test "echo \$USER\$PWD"
test "echo \"\$USER\$SHLVL\$\$\"" 

rm minishell
rm ./tests/minishell
