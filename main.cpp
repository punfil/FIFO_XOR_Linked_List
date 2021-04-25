#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "XOR_LINKED_LIST_FIFO_Queue.h"

#define MAX_LINE_LENGTH 20

int main() {
	char current_order[MAX_LINE_LENGTH];
	XOR_LINKED_LIST myobject;
	//Number variable for adding to the list commands.
	int number = 0;
	char delimiter[] = " ";
	//We break the loop using CTRL+Z.
	while (fgets(current_order, MAX_LINE_LENGTH, stdin)) {
		char* order = strtok(current_order, delimiter);
		//Cutting off \n makes no difference for numbers, but makes a huge one for GetMyCommand function! (we compare string's without \n).
		if (order[strlen(order) - 1] == '\n')
			order[strlen(order) - 1] = '\0';
		char* cnumber = strtok(nullptr, delimiter);
		//If there is second word in the line it's argument for function, that should be an integer.
		if (cnumber != nullptr) {
			number = atoi(cnumber);
		}
		command com = myobject.GetMyCommand(order);
		switch (com) {
		case command::add_beg:
			myobject.ADD_BEG(number);
			break;
		case command::add_end:
			myobject.ADD_END(number);
			break;
		case command::del_beg:
			myobject.DEL_BEG();
			break;
		case command::del_end:
			myobject.DEL_END();
			break;
		case command::print_forward:
			myobject.PRINT(which_way::forward);
			break;
		case command::print_backward:
			myobject.PRINT(which_way::backwards);
			break;
		case command::size:
			myobject.SIZE();
			break;
		case command::push:
			myobject.PUSH(number);
			break;
		case command::pop:
			myobject.POP();
			break;
		case command::print_queue:
			myobject.PRINT_QUEUE();
			break;
		case command::count:
			myobject.COUNT();
			break;
		case command::garbage_soft:
			myobject.GARBAGE_SOFT();
			break;
		case command::garbage_hard:
			myobject.GARBAGE_HARD();
			break;
		case command::not_found:
			printf("Command not found\n");
			break;
		}
	}
	return 0;
}