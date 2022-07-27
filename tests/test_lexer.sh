#!/bin/bash
input="./tests/cases"

while IFS= read -r line
do
  prog_out=$(./test_lexer "$line")
  echo "$prog_out" >> ./tests/res
done < "$input"

diff ./tests/res ./tests/lex_res;
echo "EVERYTHING IS PERFECT=========================IF NO DIFF HAVE SHOWN========"
rm ./tests/res;


  #echo CASE: "$line"
  #s2="CMD "
  #if [ "$prog_out" == "$s2" ]
  #then
  #  echo match
  #else
  #  echo fanked
  #fi	
