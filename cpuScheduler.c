#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>

/****************
-This is CMPE 351 Project program
-This program simulates the CPU Scheduler methods
*/

/*TODO
-Need to implement a way to check if the passed input argument exists (Not gonna check if the file is txt, just
check file exists(DONE!!!)

-Need to implement a linked list structure and funtions(DONE!!!)
-Need to implement a queue structure and funtions
-Need to implement a function to read from input file and insert them to Linked List(DONE!!!)
-Need to make a menu (Main menu, Methods menu, and Preemtive Mode menu are done) (IMPLEMENT menu3 and menu4)
-FCFS is Done!!!!!!SJF-NP is half done
-Implement Bubble Sort for Non-Preemtive Priority and SJF Methods(SJF Done!! Priority not DONE!!!!)
*/

// GLOBALS//
enum CPUMethods
{
	NONE,
	FCFS,
	SJF,
	PS,
	RR
} method = NONE; // CPU Scheduling Methods Enumeration
enum PMode
{
	OFF,
	ON
} mode = OFF; // Preemtive Mode Enumeration
int time_quantum;
char *input_filename = NULL;
char *output_filename = NULL;
// GLOBALS//

struct node
{
	int process_id;
	int burst_time;
	int arrival_time;
	int priority;
	int waiting_time;
	int turnaround_time;
	int how_much_left;
	bool is_terminated;
	struct node *next;
};
struct node *header_original = NULL;

// Prototypes for Linked List
struct node *create_node(int, int, int, int);
struct node *insert_back(struct node *, int, int, int, int);
struct node *insert_back(struct node *, int, int, int, int);
struct node *delete_back(struct node *);
struct node *delete_front(struct node *);
void display_LL(struct node *);
struct node *clone_LL();
// Prototypes for Linked List

// Prototypes
void print_usage();					 // Usage showing Function
void menu();						 // Main Menu Function
void menu1();						 // Scheduling Methods Menu Function
void menu2();						 // Preemtive Mode Menu Function
void menu3();						 // Show Results Menu Function
void tq_menu();						 // Asking the user for time quantum if RR method is selected
void write_input_to_LL(char *);		 // Reading the input file and then writing it to LL
int total_burst_time(struct node *); // Getting total burst time of the input file
void fcfs();						 // FirstComeFirstServe Functions
void sjf_np();						 // Shortes-Job-First Non-Preemtive
// void sjf_p();						 //Shortes-Job-First Preemtive
void ps_np();										   // Priority Scheduling Non-Preemtive
int process_counter(struct node *);					   // Process counter
struct node *swap_nodes(struct node *, struct node *); // Swap Funcion
void bubble_sort(struct node **, int, char *);		   // Bubble Sort (SJF/PS/PID)
// Prototypes

int main(int argc, char *argv[])
{
	int options = 0;

	// Here we check if the correct options are used
	while ((options = getopt(argc, argv, "f:o:")) != -1)
	{
		switch (options)
		{
		case 'f':
			input_filename = optarg;
			break;
		case 'o':
			output_filename = optarg;
			break;
		default:
			print_usage();
			break;
		}
	}

	// Here we check if the arguments are passed for options
	if (input_filename == NULL || output_filename == NULL)
	{
		print_usage();
	}

	FILE *finput = fopen(input_filename, "r");
	if (finput == NULL) // Checking if the input file argument exists.
	{
		printf("The argument that you passed as input file does not exists.\n");
		printf("Please check the input file argument and run the program again\n");
		exit(1);
	}
	fclose(finput);

	write_input_to_LL(input_filename);

	menu();

	return 0;
}

// Creating node Function
struct node *create_node(int pid, int burst_time, int arrival_time, int priority)
{
	struct node *temp;
	temp = (struct node *)malloc(sizeof(struct node));

	temp->process_id = pid;
	temp->burst_time = burst_time;
	temp->arrival_time = arrival_time;
	temp->priority = priority;
	temp->turnaround_time = 0;
	temp->waiting_time = 0;
	temp->is_terminated = false;
	temp->how_much_left = burst_time;
	temp->next = NULL;

	return temp;
}

// Insert back to the LL Function
struct node *insert_back(struct node *header, int id, int burst_time, int arrival_time, int priority)
{
	struct node *temp = create_node(id, burst_time, arrival_time, priority);
	struct node *header_temp;

	// Check if the linked list is empty
	if (header == NULL)
	{
		header = temp;
		return header;
	}

	header_temp = header;
	while (header_temp->next != NULL) // Iterate until we reach the last node
		header_temp = header_temp->next;

	header_temp->next = temp;
	return header;
}

// Insert front of the LL Funtion
struct node *insert_front(struct node *header, int id, int burst_time, int arrival_time, int priority)
{
	struct node *temp = create_node(id, burst_time, arrival_time, priority);
	temp->next = header;
	header->next = temp;
	return header;
}

// Delete back of the LL Function
struct node *delete_back(struct node *header)
{
	struct node *temp, *header_temp;

	// Checking if the LL is empty
	if (header == NULL)
	{
		return header;
	}

	header_temp = header;
	while (header_temp->next->next != NULL)
	{
		header_temp = header_temp->next;
	}
	temp = header_temp->next;
	header_temp->next = NULL;
	free(temp);
	return header;
}

// Delete front of the LL Function
struct node *delete_front(struct node *header)
{
	struct node *temp;

	if (header == NULL)
	{
		return header;
	}

	temp = header;
	header = header->next;
	free(temp);
	return header;
}

// Displaying the Linked List Items(For Debugging Purposes Only)
void display_LL(struct node *header)
{
	struct node *temp = header;
	while (temp != NULL)
	{
		int a, b, c, d, e, f, g;
		bool t;
		a = temp->process_id;
		b = temp->burst_time;
		c = temp->arrival_time;
		d = temp->priority;
		e = temp->waiting_time;
		f = temp->turnaround_time;
		g = temp->how_much_left;

		printf("ID:%d\tBurst:%d\tArrival:%d\tPriority:%d\tWait:%d\tTurn:%d\tLeft:%d\n", a, b, c, d, e, f, g);
		temp = temp->next;
	}

	getchar();
	getchar();
}

// Cloning Main LL Function
struct node *clone_LL(struct node *header)
{
	struct node *header_temp = header;
	struct node *clone_header = NULL;

	while (header_temp != NULL)
	{
		int pid = 0, burst = 0, arrival = 0, priority = 0;
		pid = header_temp->process_id;
		burst = header_temp->burst_time;
		arrival = header_temp->arrival_time;
		priority = header_temp->priority;
		clone_header = insert_back(clone_header, pid, burst, arrival, priority);

		header_temp = header_temp->next;
	}

	return clone_header;
}

// This funtions is used to print programs usage and what arguments are needed to pass
void print_usage()
{
	printf("Usage: cmpe351 -f <*.txt> -o <*.txt>\n");
	exit(1);
}

// Main Menu Function
void menu()
{
	while (true)
	{
		system("clear");
		int option;
		printf("\t\tCPU Scheduler Simulator\n");
		switch (method)
		{
		case 0:
			printf("1-) Scheduling Method (NONE)\n");
			break;
		case 1:
			printf("1-) Scheduling Method (FCFS)\n");
			break;
		case 2:
			printf("1-) Scheduling Method (SJF)\n");
			break;
		case 3:
			printf("1-) Scheduling Method (PS)\n");
			break;
		case 4:
			printf("1-) Scheduling Method (RR)\n");
			break;
		default:
			printf("1-) Scheduling Method (NONE)\n");
			break;
		}

		switch (mode)
		{
		case 0:
			printf("2-) Preemtive Mode (OFF)\n");
			break;
		case 1:
			printf("2-) Preemtive Mode (ON)\n");
			break;
		default:
			printf("2-) Preemtive Mode (OFF)\n");
			break;
		}

		printf("3-) Show Result\n");
		printf("4-) End Program\n");
		printf("Option > ");
		scanf("%1d", &option);

		// Will implement a switch syntax here
		switch (option)
		{
		case 1:
			menu1();
			break;
		case 2:
			menu2();
			break;
		case 3:
			menu3();
			break;
		case 4:
			exit(0);
			break; // Need to change here!!!!!!!!!!!!!!!!!
		default:
			exit(0);
			break; // Need to change here too!!!!!!!!!!!!!
		}
	}
}

// Scheduling Methods Menu Function
void menu1()
{
	system("clear");
	int option = 0;
	// Modifying Menu according to if preemtive mode is on
	if (mode == 1)
	{
		printf("1-) None: None of scheduling method chosen\n");
		printf("2-) Shortest-Job-First Scheduling\n");
		printf("3-) Priority Scheduling\n");
		printf("Option > ");
		scanf("%1d", &option);

		switch (option)
		{
		case 1:
			method = NONE;
			break;
		case 2:
			method = SJF;
			break;
		case 3:
			method = PS;
			break;
		default:
			printf("Please select a valid option\n");
			system("sleep 1");
			break;
		}
	}
	else
	{
		printf("1-) None: None of scheduling method chosen\n");
		printf("2-) First Come, First Served Scheduling\n");
		printf("3-) Shortest-Job-First Scheduling\n");
		printf("4-) Priority Scheduling\n");
		printf("5-) Round-Robin Scheduling\n");
		printf("Option > ");
		scanf("%1d", &option);
		switch (option)
		{
		case 1:
			method = NONE;
			break;
		case 2:
			method = FCFS;
			break;
		case 3:
			method = SJF;
			break;
		case 4:
			method = PS;
			break;
		case 5:
			method = RR;
			tq_menu();
			break;
		default:
			printf("Please select a valid option\n");
			system("sleep 1");
			break;
		}
	}
}

// Preemtive Mode Menu Function
void menu2()
{
	system("clear");
	int option = 0;
	if (method == 0) // If any scheduling method is not selected this menu will be shown
	{
		printf("(Scheduling Methods Menu will be modified according to your preemtive mode choice.)\n");
		printf("Please! Next time when you are choosing preemtive mode, ");
		printf("Firstly select the scheduling method first.\n");
		printf("1-) OFF\n");
		printf("2-) ON\n");
		printf("Option > ");
		scanf("%1d", &option);
		switch (option)
		{
		case 1:
			mode = OFF;
			break;
		case 2:
			mode = ON;
			break;
		default:
			printf("Please select a valid option\n");
			system("sleep 1");
			break;
		}
	}
	else if (method == 2 || method == 3) // If SJF or PS is selected this menu will be shown
	{
		printf("1-) OFF\n");
		printf("2-) ON\n");
		printf("Option > ");
		scanf("%1d", &option);
		switch (option)
		{
		case 1:
			mode = OFF;
			break;
		case 2:
			mode = ON;
			break;
		default:
			printf("Please select a valid option\n");
			system("sleep 1");
			break;
		}
	}
	else // If FCFS or RR is selected user will be informed that preemtive mode is not available for those methods
	{
		printf("Preemtive mode is not available for selected Scheduling Method\n");
		system("sleep 1");
	}
}

// Show Result Menu Function
void menu3()
{
	switch (method)
	{
	case 1:
		fcfs();
		break;

	case 2:
		if (mode == 0)
		{
			sjf_np();
		}
		else
		{
			// sjf_p();
		}
		break;

	case 3:
		if (mode == 0)
		{
			ps_np();
		}
		else
		{
			// ps_p();
		}
		break;

	default:
		break;
	}
}

// Time Quantum Asking Menu
void tq_menu()
{
	while (time_quantum == 0)
	{
		system("clear");
		printf("Please enter the time quantum for Round-Robin Method\n");
		printf("Time Quantum > ");
		scanf("%d", &time_quantum);
	}
}

// Reading from Input File to Write it to LL Function
void write_input_to_LL(char *input_filename)
{
	FILE *finput = fopen(input_filename, "r");
	int id_counter = 0;
	if (feof(finput))
	{
		printf("The input file is empty\n");
		exit(1);
	}
	else
	{
		while (!feof(finput)) // Reading the input file and recording the values to Linked List
		{
			int a, b, c;
			fscanf(finput, "%d:%d:%d\n", &a, &b, &c);
			header_original = insert_back(header_original, id_counter, a, b, c);
			id_counter++;
		}
	}
	fclose(finput);
}

// Getting Total Burst Time Function
int total_burst_time(struct node *header)
{
	struct node *temp = header;
	int ret = 0;
	if (temp == NULL)
	{
		return ret;
	}

	while (temp != NULL)
	{
		ret += temp->burst_time;
		temp = temp->next;
	}

	return ret;
}

// First-Come-First-Serve Function
void fcfs()
{
	struct node *clone_header = clone_LL(header_original);
	struct node *temp = clone_header;
	int wait_time = 0;
	float average_wait = 0.00f;
	int number_of_process = process_counter(clone_header);

	while (clone_header != NULL)
	{
		clone_header->waiting_time = wait_time;
		wait_time += clone_header->burst_time;
		clone_header = clone_header->next;
	}

	system("clear");
	printf("Scheduling Method: First Come First Served\n");
	printf("Process Waiting Times:\n");
	while (temp != NULL)
	{
		int pid = temp->process_id;
		int wait = temp->waiting_time;
		average_wait += wait;
		printf("PS%d: %d ms\n", pid, wait);
		temp = temp->next;
	}
	average_wait /= number_of_process;
	printf("Average Waiting Time: %.3f ms\n\n", average_wait);
	printf("Press Enter to return to the main menu.\n");
	getchar();
	getchar();
}

// Shortes-Job-First Non-Preemtive Function
void sjf_np()
{
	// I have first tried selectiob sort but could not figure it out...
	//...(There were complications regarding to non-adjacent nodes)
	// So I changed the sorting algorithm to bubble sort
	struct node *clone_header = clone_LL(header_original);
	struct node *temp;
	int wait_time = 0;
	float average_wait = 0.00f;
	int number_of_process = process_counter(clone_header);
	bubble_sort(&clone_header, number_of_process, "SJF");
	temp = clone_LL(clone_header);
	struct node *temp1 = temp;
	while (temp != NULL)
	{
		temp->waiting_time = wait_time;
		wait_time += temp->burst_time;
		temp = temp->next;
	}

	system("clear");
	printf("Scheduling Method: Shortest Job First (Non-Preemtive)\n");
	printf("Process Waiting Times:\n");
	while (temp1 != NULL)
	{
		int pid = temp1->process_id;
		int wait = temp1->waiting_time;
		average_wait += wait;
		printf("PS%d: %d ms\n", pid, wait);
		temp1 = temp1->next;
	}
	average_wait /= number_of_process;
	printf("Average Waiting Time: %.3f ms\n\n", average_wait);
	printf("Press Enter to return to the main menu.\n");
	getchar();
	getchar();
}

// Priority Scheduling Non-Preemtive Function
void ps_np()
{
	struct node *clone_header = clone_LL(header_original);
	struct node *temp;
	int wait_time = 0;
	float average_wait = 0.00f;
	int number_of_process = process_counter(clone_header);
	bubble_sort(&clone_header, number_of_process, "PS");
	temp = clone_LL(clone_header);
	struct node *temp1 = temp;
	while (temp != NULL)
	{
		temp->waiting_time = wait_time;
		wait_time += temp->burst_time;
		temp = temp->next;
	}

	system("clear");
	printf("Scheduling Method: Priority Scheduling (Non-Preemtive)\n");
	printf("Process Waiting Times:\n");
	while (temp1 != NULL)
	{
		int pid = temp1->process_id;
		int wait = temp1->waiting_time;
		average_wait += wait;
		printf("PS%d: %d ms\n", pid, wait);
		temp1 = temp1->next;
	}
	average_wait /= number_of_process;
	printf("Average Waiting Time: %.3f ms\n\n", average_wait);
	printf("Press Enter to return to the main menu.\n");
	getchar();
	getchar();
}

// Counts How many process' are in the LL
int process_counter(struct node *header)
{
	struct node *temp = header;
	int counter = 0;
	while (temp != NULL)
	{
		counter++;
		temp = temp->next;
	}

	return counter;
}

// Swapping nodes
struct node *swap_nodes(struct node *temp1, struct node *temp2)
{
	struct node *tmp = temp2->next;
	temp2->next = temp1;
	temp1->next = tmp;
	return temp2;
}

// Sorts LL in ascending order
void bubble_sort(struct node **header, int counter, char *sort_mode)
{
	struct node **header_temp;
	int swapped;

	for (int i = 0; i < counter; i++)
	{
		header_temp = header;
		swapped = 0;

		for (int j = 0; j < counter - 1 - i; j++)
		{
			struct node *temp1 = *header_temp;
			struct node *temp2 = temp1->next;

			if (!strcmp(sort_mode, "SJF"))
			{
				if (temp1->burst_time >= temp2->burst_time)
				{
					*header_temp = swap_nodes(temp1, temp2);
					swapped = 1;
				}
				header_temp = &(*header_temp)->next;
			}

			else if (!strcmp(sort_mode, "PS"))
			{
				if (temp1->priority >= temp2->priority)
				{
					*header_temp = swap_nodes(temp1, temp2);
					swapped = 1;
				}
				header_temp = &(*header_temp)->next;
			}

			else if (!strcmp(sort_mode, "PID"))
			{
				if (temp1->process_id >= temp2->process_id)
				{
					*header_temp = swap_nodes(temp1, temp2);
					swapped = 1;
				}
				header_temp = &(*header_temp)->next;
			}
		}

		if (swapped == 0)
		{
			break;
		}
	}
}