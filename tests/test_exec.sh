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
cp minishell ./tests/ chmod 755 minishell 


#TEST WITH NO EXIT STATUS !!!!! !!!! !!!
function test(){
	MY=$(./minishell -c "$1")
	#MY=$(echo $@ "; exit" | ./minishell 2>&-)
	BASH=$(echo $1 "; exit" | bash )
	#BASH=$($1)
		
	printf $BOLDGREEN"TEST CASE:$RESET $CYAN$1$RESET\n"
	#echo "$MY% < MY" ; echo "$BASH% < BASH"
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
test 'echo | pwd | echo | pwd | env | pwd | echo | env | head | echo | pwd' test 'env | head -10 | head -10 | echo some text val yes | pwd | env | grep P | wc | wc'
test 'env'



printf " $BOLWHTIE-----------------------QUTATION MARKS TESTS-----------------------$RESET\n"

test "echo \"some va '\""
#test "echo some va '\""
test "echo \"'\" \"val '\""
test "echo \"''\"" 
test "echo 'ad e' 'ads \" s \"\"'"

test "echo \"\""
test '"echo" "-n"'
test '"echo" "-n" "args"'
test '"echo" "-n" "args" | "wc"'
test '"echo" "-n" "args" | cat /bin/ls | head "-1"'
test '"echo" "-n""args"'
test '"echo" "n""args"'
test 'echo a"ba"'
test 'echo aaa"ba"'
test 'echo aaa"ba" aaa"ba"'
test 'echo "ba"c'
test 'echo "ba"ccc'
test 'echo "ba"ccc "ba"ccc'

#echo "         ' ' ' fsdf dsaf '''  d
#test "echo '     \"\" d\" dsd '  'ss ss \"  \" \"'"

test "echo 'as 'a''"
test "echo 'as 'a'a'"
test "echo \"asd '\"  \"ad\""
test "echo \"< no pipe | or semicolon will ; stop me >\""
test "echo \"some ' text\" 'a d' 'sd\"ds\"\"' 'ds\"sd\" d'"
test "e'c'h\"o\" \"hello\""

printf " $BOLWHTIE-----------------------QUTATION && \$ENV-----------------------$RESET\n"
test "echo \$USER\$PWD"
#test "echo \"\$USER\$SHLVL\$\$\"" 
test 'echo $TEST'
test 'echo "$TEST"'
test "echo '$TEST'"
test 'echo "$TEST$TEST$TEST"'
test 'echo "$TEST lol"'
test 'echo "$TEST     lol"'
test "echo \"   \$TEST lol \""
test 'echo "   $TEST lol $TEST"'
test 'echo $TEST$TEST$TEST'
test 'echo ""lol'
test 'echo $TEST$TEST=lol$TEST""lol'
test 'echo    $TEST lol $TEST'
test 'echo test "" test "" test'
test 'echo "$=TEST"'
test 'echo "$TEST$TEST=lol$TEST"'
test 'echo "$?TEST"'
test 'echo "$1TEST"'
test 'echo "$"'
test 'echo $TEST $TEST'
test 'echo "$T1TEST"'


rm minishell
rm ./tests/minishell
