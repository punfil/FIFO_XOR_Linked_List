#include "XOR_LINKED_LIST_FIFO_Queue.h"

//This function is used in constructor, so it's before it. It's only for code size reduction purposes.
void XOR_LINKED_LIST::ResetTheList() {
	head = nullptr;
	tail = nullptr;
	queue_head = nullptr;
	queue_head_left_neighbour = nullptr;
	queue_tail = nullptr;
	queue_tail_right_neighbour = nullptr;
	head_position = 0;
}

XOR_LINKED_LIST::XOR_LINKED_LIST() {
	ResetTheList();
	//Some might say unnecessary, but definitely makes the code easier to understand and makes hardly any performance difference.
	elements_count.list_count = 0;
	elements_count.queue_count = 0;
	//We need to know where the queue head is, 0 means it's tail of the list (we use it later on in ADD_BEG and ADD_END)
	head_position = 0;
}
//Returns XOR of two adresses
two_direct* XOR_LINKED_LIST::XOR(two_direct* x, two_direct* y) const {
	return (two_direct*)((uintptr_t)(x) ^ (uintptr_t)(y));
}
//Adds element to the beggining of the list.
void XOR_LINKED_LIST::ADD_BEG(const int add_value) {
	two_direct* temp = (two_direct*)malloc(sizeof(two_direct));
	//If allocation was unsuccessful then exit
	if (temp == nullptr) {
		printf("Error allocating memory. Please try again later :/");
		return;
	}
	temp->value = add_value;
	temp->nap = XOR(head, nullptr);
	//If there are elements on the list then move current head to the second position, so we asing XOR of new head and the element following the current head.
	if (elements_count.list_count) {
		head->nap = XOR(temp, head->nap);
	}
	//If there are no elements then set all variables to the current element.
	else if (!elements_count.list_count) {
		tail = temp;
	}
	//If queue head was list head then we need to change the queue_head left neighbour.
	if (queue_head == head && queue_head != nullptr)
		queue_head_left_neighbour = temp;
	//If queue tail was list tail than it's right neighbour was head. We need to adjust it as well.
	if (queue_tail_right_neighbour == head && queue_tail_right_neighbour != nullptr)
		queue_tail_right_neighbour = temp;
	//If list was cycled - i.e. We have a list 14578 and for instance 4 is queue_head and 7 is queue_tail, new element is added to the queue!
	if (elements_count.list_count - head_position < elements_count.queue_count) {
		elements_count.queue_count++;	
	}
	elements_count.list_count++;
	head = temp;
}
//Adds element to the end of the list. It uses ADD_BEG
void XOR_LINKED_LIST::ADD_END(const int add_value) {
	//If the list is empty we utilisise the base function and quit the function. It makes no difference.
	if (!elements_count.list_count) {
		ADD_BEG(add_value);
		return;
	}
	two_direct* temp = (two_direct*)malloc(sizeof(two_direct));
	if (temp == nullptr) {
		printf("Error allocating memory. Please try again later :/");
		return;
	}
	temp->value = add_value;
	temp->nap = XOR(tail, nullptr);
	//If there are elements on the list, then we need to move tail to the pre-last position.
	if (elements_count.list_count) {
		tail->nap = XOR(temp, tail->nap);
	}
	//If list was cycled - i.e. We have a list 14578 and for instance 4 is queue_head and 7 is queue_tail, new element is added to the queue!
	if (elements_count.list_count - head_position < elements_count.queue_count) {
		elements_count.queue_count++;
	}
	if (queue_head != nullptr)
		head_position++;
	//If queue tail was list tail then we need to change pointer to it's right neighbour.
	if (queue_tail == tail && queue_tail != nullptr)
		queue_tail_right_neighbour = temp;
	//If queue was head then we need to change pointer to it's left neighbour.
	if (queue_head_left_neighbour == tail && queue_head_left_neighbour != nullptr)
		queue_head_left_neighbour = temp;
	elements_count.list_count++;
	tail = temp;
}
//Deletes current head of the list and moves elements. It's a base function, so contains all possibilities.
void XOR_LINKED_LIST::DEL_BEG() {
	//Nothing to do if list is empty :)
	if (head!=nullptr && elements_count.queue_count) {
		//If it was list head was queue head then we need to adjust the queue pointers. We cannot cause a situation where queue_head == queue_tail!
		if (queue_head == head) {
			if (XOR(head->nap, nullptr) == queue_tail) {
				queue_head = tail;
				queue_head_left_neighbour = XOR(tail->nap, nullptr);
			}
			else
				queue_head = XOR(queue_head->nap, nullptr);
		}
		//If left neighbour of queue head was head then we need to move it to tail.
		else if (queue_head_left_neighbour == head) {
			queue_head_left_neighbour = tail;
		}
		//If queue tail was list head than we need to move it further to right.
		else if (queue_tail == head) {
			queue_tail_right_neighbour = XOR(queue_tail, queue_tail_right_neighbour->nap);
			queue_tail = XOR(head->nap, nullptr);
		}
		//If queue tail right neigbour was list head then we need to move it further to right
		else if (queue_tail_right_neighbour == head) {
			queue_tail_right_neighbour = XOR(head->nap, nullptr);
		}
		//If element belonged to queue...
		if (head_position + elements_count.queue_count >= elements_count.list_count) elements_count.queue_count--;
	}
	if (elements_count.list_count && head!=nullptr) {
		two_direct* head_next = XOR(head->nap, nullptr);
		if (head_next!=nullptr)
			head_next->nap = XOR(head_next->nap, head);
		free(head);
		//Move head to new head.
		head = head_next;
		elements_count.list_count--;
		//Utilisation of this function just makes everything easier. Reset's the list to base state (all pointers = nullptr) if there are no elements.
		if (!elements_count.list_count)
			ResetTheList();
	}
}
//Deletes last element on the list.
void XOR_LINKED_LIST::DEL_END() {
	if (tail != nullptr && elements_count.queue_count) {
		//If queue head was tail then we need to adjust it and it's left neighbour
		if (queue_head == tail) {
			queue_head_left_neighbour = XOR(queue_head_left_neighbour->nap, queue_head);
			queue_head = XOR(tail->nap, nullptr);
		}
		//If queue head left neighbour was tail then we need to move it further
		else if (queue_head_left_neighbour == tail) {
			queue_head_left_neighbour = XOR(tail->nap, nullptr);
		}
		//If queue tail was tail then it needs proper adjustment. We need to keep it mind queue_head != queue_tail
		else if (queue_tail == tail) {
			if (XOR(tail->nap, nullptr) == queue_head) {
				queue_tail = head;
				queue_tail_right_neighbour = XOR(head->nap, nullptr);
			}
			else
				queue_tail = XOR(tail->nap, nullptr);
		}
		//If was tail then we need to move both of them further to left
		else if (queue_tail_right_neighbour == tail) {
			queue_tail = XOR(queue_tail->nap, queue_tail_right_neighbour);
			queue_tail_right_neighbour = XOR(tail->nap, nullptr);
		}
		//If the element belonged to queue we need to decrease count of queue elements.
		if (head_position + elements_count.queue_count >= elements_count.list_count) elements_count.queue_count--;
		if (queue_head != nullptr && tail!=queue_head)
			head_position--;
	}
	if (elements_count.list_count && tail!=nullptr) {
		two_direct* tail_prev = XOR(tail->nap, nullptr);
		//If there is more than one element then we need to change to XOR of the previous previous element.
		if (tail_prev!=nullptr)
			tail_prev->nap = XOR(tail_prev->nap, tail);
		free(tail);
		tail = tail_prev;
		elements_count.list_count--;
		//Utilisation of this function just makes everything easier. Reset's the list to base state (all pointers = nullptr) if there are no elements.
		if (!elements_count.list_count)
			ResetTheList();
	}
}
//Prints the list forward or backwards, depending on the given argument.
void XOR_LINKED_LIST::PRINT(which_way frontorback) const {
	two_direct* current;
	//Depending on direction we set the first element to be printed appropriately
	if (frontorback == which_way::forward)
		current = head;
	else
		current = tail;
	two_direct* previous = nullptr;
	two_direct* next = nullptr;
	while (current != nullptr) {
		printf("%d ", current->value);
		next = XOR(previous, current->nap);
		previous = current;
		current = next;
	}
	//If we didn't print anything there is no need to print new line sign
	if (elements_count.list_count) {
		printf("\n");
		return;
	}
	printf("%s\n", "NULL");
}
//This function is only for easier setting up Queue Initial Variables. It is only used when the queue needs to be created.
void XOR_LINKED_LIST::SetInitialQueueVariables() {
	queue_head = tail;
	queue_head_left_neighbour = nullptr;
	queue_tail = tail;
	queue_tail_right_neighbour = nullptr;
	head_position = 0;
}
//Function that is only used in Push function
void XOR_LINKED_LIST::ADD_BEFORE_QUEUE_HEAD() {
	two_direct* temp = (two_direct*)malloc(sizeof(two_direct));
	if (temp == nullptr) {
		printf("Error allocating memory. Please try again later :/");
		return;
	}
	temp->nap = XOR(queue_tail, queue_head);
	//Adjust queue_head pointers - we cannot allow to add xor to nap of a list head or list tail
	if (queue_head_left_neighbour == tail) {
		queue_head->nap = XOR(temp, nullptr);
	}
	else {
		queue_head->nap = XOR(queue_head_left_neighbour, temp);
	}
	if (queue_tail_right_neighbour == head) {
		queue_tail->nap = XOR(temp, nullptr);
	}
	else {
		queue_tail->nap = XOR(temp, queue_tail_right_neighbour);
	}
	two_direct* temp2 = queue_tail;
	queue_tail = temp;
	queue_tail_right_neighbour = temp2;
	elements_count.list_count++;
}

//Prints size of the list
void XOR_LINKED_LIST::SIZE() {
	printf("%d\n", elements_count.list_count);
}
//Pushes element to the beggining of the queue
void XOR_LINKED_LIST::PUSH(const int add_value) {
	//If there are no elements on the list we can use ADD_BEG function and then set variables of the queue
	if (!elements_count.list_count) {
		ADD_BEG(add_value);
		SetInitialQueueVariables();
	}
	//If there were no elements in queue we just set the initial variables
	else if (!elements_count.queue_count) {
		SetInitialQueueVariables();
	}
	//If we need the list is full and queue_tail is head we can utilise ADD_BEG function and move queue pointers.
	else if (queue_tail == head && elements_count.queue_count == elements_count.list_count) {
		ADD_BEG(add_value);
		queue_tail = head;
		queue_tail_right_neighbour = XOR(head->nap, nullptr);
	}
	//If the list is full then we need to add before tail. It handles the situation, when tail = queue_tail.
	else if (elements_count.queue_count == elements_count.list_count) {
		ADD_BEFORE_QUEUE_HEAD();
	}
	//If queue tail is head, but there are still element to overwrite
	else if (queue_tail == head) {
		queue_tail_right_neighbour = head;
		queue_tail = tail;
	}
	//Same here...
	else if (queue_tail == tail) {
		queue_tail_right_neighbour = tail;
		queue_tail = XOR(tail->nap, nullptr);
	}
	//Else means the tail is elsewhere, but there are still elements on the list to be overwritten.
	else {
		two_direct* temp = queue_tail_right_neighbour;
		queue_tail_right_neighbour = queue_tail;
		queue_tail = XOR(temp, queue_tail->nap);
	}
	//This allows us to move the pointers when second element is added 
	if (queue_head_left_neighbour == nullptr && elements_count.queue_count == 1) {
		queue_head_left_neighbour = queue_tail;
		queue_tail_right_neighbour = queue_head;
	}
	elements_count.queue_count++;
	queue_tail->value = add_value;

}
//Reads the last element of the queue and deletes it from the queue
void XOR_LINKED_LIST::POP() {
	//If there are elements then read them.
	if (elements_count.queue_count > 0) {
		printf("%d\n", queue_head->value);
	}
	else
		printf("%s\n", "NULL");
	//If there is more than one element in the queue then we need to deal with pointers
	if (elements_count.queue_count > 1) {
		//Changing the owner of the element
		//If the queue head left neighbour was head it goes to the tail and the tail of the queue goes to head of the list.
		if (queue_head_left_neighbour == head) {
			queue_head_left_neighbour = tail;
			queue_head = head;
			head_position++;
		}
		//If it queue head is head then it becomes tail of the list and it's left neighbour is pre-last element on the list.
		else if (queue_head_left_neighbour == tail){
			queue_head = tail;
			queue_head_left_neighbour = XOR(tail->nap, nullptr);
			head_position=0;
		}
		//If not condition is matched we move the head and it's neighbour to the left.
		else {
			two_direct* temp = queue_head_left_neighbour;
			queue_head_left_neighbour = XOR(queue_head, queue_head_left_neighbour->nap);
			queue_head = temp;
			head_position++;
		}
	}
	if (elements_count.queue_count > 0)
		elements_count.queue_count--;
}
//Prints all_elements that are in queue
void XOR_LINKED_LIST::PRINT_QUEUE() {
	if (!elements_count.queue_count) {printf("%s\n", "NULL"); return;}
	//We start from the queue_head and go till we reach the head of the list or we print all elements.
	two_direct* current = queue_head;
	two_direct* previous = XOR(queue_head->nap, queue_head_left_neighbour);
	if (current == head) {
		previous = XOR(queue_head->nap, nullptr);
	}
	two_direct* next = nullptr;
	int printed = 0;
	while (current != nullptr && printed++<elements_count.queue_count) {
		printf("%d ", current->value);
		next = XOR(previous, current->nap);
		previous = current;
		current = next;
	}
	//Next if we didn't print all the elements that means queue tail is later in the list then the queue head. We print from back of the list till we print all of the elements.
	current = tail;
	previous = nullptr;
	next = nullptr;
	while (current != nullptr && printed++ < elements_count.queue_count) {
		printf("%d ", current->value);
		next = XOR(previous, current->nap);
		previous = current;
		current = next;
	}
	if (elements_count.queue_count) {
		printf("\n");
	}
}
//Prints count of elements that belong to the queue
void XOR_LINKED_LIST::COUNT() {
	printf("%d\n", elements_count.queue_count);
}
//Write 0 to all list elements (that does not belong to queue)
void XOR_LINKED_LIST::GARBAGE_SOFT() const {
	two_direct* current = head;
	two_direct* previous = nullptr;
	two_direct* next = nullptr;
	for (int i=0; current != nullptr ;i++){
		if (!elements_count.queue_count || (i < elements_count.list_count - head_position - elements_count.queue_count|| i< 2*elements_count.list_count - elements_count.queue_count - head_position && i>=elements_count.list_count - head_position))
			current->value = 0;
		next = XOR(previous, current->nap);
		previous = current;
		current = next;
	}
}
//Delete all list elements (that does not belong to queue)
void XOR_LINKED_LIST::GARBAGE_HARD() {
	//No elements = no work to do
	if (!elements_count.list_count) return;
	//These two while loops makes the task easier - code redundancy. We don't have to check conditions later on.
	while (head != nullptr && (!(head_position + elements_count.queue_count >= elements_count.list_count)||!elements_count.queue_count))
		DEL_BEG();
	while (tail !=nullptr && (!(head_position + elements_count.queue_count >= elements_count.list_count)||!elements_count.queue_count))
		DEL_END();
	two_direct* current = head;
	two_direct* previous = nullptr;
	two_direct* next = nullptr;
	for (int i = 0; current != nullptr; i++) {
		next = XOR(previous, current->nap);
		//If it belongs to list than we can delete it.
		if ((!elements_count.queue_count || (i < elements_count.list_count - head_position - elements_count.queue_count || i < 2 * elements_count.list_count - elements_count.queue_count - head_position && i >= elements_count.list_count - head_position)) && previous!=nullptr){
			previous->nap = XOR(XOR(previous->nap, current), next);
			next->nap = XOR(XOR(next->nap, current), previous);
			free(current);
			elements_count.list_count--;
			current = next;
			head_position--;
			i--;
			continue;
		}
		previous = current;
		current = next;
	}
}
//Depending on the input command returns value from the command enum class
command XOR_LINKED_LIST::GetMyCommand(char* order) {
	if (!strcmp(order, "ADD_BEG"))
		return command::add_beg;
	if (!strcmp(order, "ADD_END"))
		return command::add_end;
	if (!strcmp(order, "DEL_BEG"))
		return command::del_beg;
	if (!strcmp(order, "DEL_END"))
		return command::del_end;
	if (!strcmp(order, "PRINT_FORWARD"))
		return command::print_forward;
	if (!strcmp(order, "PRINT_BACKWARD"))
		return command::print_backward;
	if (!strcmp(order, "SIZE"))
		return command::size;
	if (!strcmp(order, "PUSH"))
		return command::push;
	if (!strcmp(order, "POP"))
		return command::pop;
	if (!strcmp(order, "PRINT_QUEUE"))
		return command::print_queue;
	if (!strcmp(order, "COUNT"))
		return command::count;
	if (!strcmp(order, "GARBAGE_SOFT"))
		return command::garbage_soft;
	if (!strcmp(order, "GARBAGE_HARD"))
		return command::garbage_hard;
	if (!strcmp(order, ""))
		return command::mistake_enter;
	return command::not_found;
}

XOR_LINKED_LIST::~XOR_LINKED_LIST() {
	two_direct* current = head;
	two_direct* previous = nullptr;
	two_direct* next = nullptr;
	while (current != nullptr) {
		next = XOR(previous, current->nap);
		if (previous != nullptr) free(previous);
		previous = current;
		current = next;
	}
}