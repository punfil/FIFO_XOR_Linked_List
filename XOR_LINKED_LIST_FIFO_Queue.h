#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum class command {
	add_beg,
	add_end,
	del_beg,
	del_end,
	print_forward,
	print_backward,
	size,
	push,
	pop,
	print_queue,
	count,
	garbage_soft,
	garbage_hard,
	mistake_enter,
	not_found,
};

enum class which_way {
	forward,
	backwards,
};

struct two_direct {
	int value;
	two_direct* nap; //next and previous
};

struct elements_count_struct {
	int list_count;
	int queue_count;
};

class XOR_LINKED_LIST {
	two_direct* head;
	two_direct* tail;
	two_direct* queue_head;
	two_direct* queue_head_left_neighbour;
	two_direct* queue_tail;
	two_direct* queue_tail_right_neighbour;
	elements_count_struct elements_count;
	int head_position;
	//This function is only for easier setting up Queue Initial Variables. It is only used when the queue needs to be created.
	void SetInitialQueueVariables();
	//Function that is only used in Push function
	void ADD_BEFORE_QUEUE_HEAD();
	//This function is used in constructor, so it's before it. It's only for code size reduction purposes.
	void ResetTheList();
public:
	XOR_LINKED_LIST();
	//Returns XOR of two adresses
	two_direct* XOR(two_direct* x, two_direct* y) const;
	//Adds element to the beggining of the list. It's a base for other functions, so includes all possibilities.
	void ADD_BEG(const int add_value);
	//Adds element to the end of the list. It uses ADD_BEG
	void ADD_END(const int add_value);
	//Deletes current head of the list and moves elements. It's a base function, so contains all possibilities.
	void DEL_BEG();
	//Deletes last element on the list.
	void DEL_END();
	//Prints the list forward or backwards, depending on the given argument.
	void PRINT(which_way frontorback) const;

	//Prints size of the list
	void SIZE();
	//Pushes element to the beggining of the queue
	void PUSH(const int add_value);
	//Reads the last element of the queue and deletes it
	void POP();
	//Prints all_elements that are in queue
	void PRINT_QUEUE();
	//Prints count of elements that belong to the queue
	void COUNT();
	//Write 0 to all list elements (that does not belong to queue)
	void GARBAGE_SOFT() const;
	//Delete all list elements (that does not belong to queue)
	void GARBAGE_HARD();
	//Depending on the input command returns value from the command enum class
	static command GetMyCommand(char* order);
	~XOR_LINKED_LIST();
};