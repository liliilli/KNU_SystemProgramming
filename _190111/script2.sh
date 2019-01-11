#!/bin/sh
# @file : script2.sh
# @brief : A real program with variables, input, and control flow.
#

BOOK=$HOME/phonebook.data
echo find what name in phonebook.
read NAME

if grep "^$NAME" $BOOK > ./pb.tmp
then
	echo Entries for $NAME
	cat ./pb.tmp
else
	echo No entries for $NAME
fi

rm ./pb.tmp
