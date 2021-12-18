#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>

/****************
-This is CMPE 351 Project program
-This program simulates the CPU Scheduler methods
*/

/*TODO
-Need to implement a way to check if the passed input argument exists (Not gonna check if the file is txt, just
-check file exists(DONE!!!)

-Need to implement a linked list structure and funtions
-Need to implement a queue structure and funtions
-Need to implement a custom structure for reading the input file
-Need to make a menu (Main menu, Methods menu, and Preemtive Mode menu are done) (IMPLEMENT menu3 and menu4)
*/

struct node{
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

//Prototypes for Linked List
struct node* create_node(int,int,int,int);
struct node* insert_back(struct node*,int,int,int,int);
struct node* insert_back(struct node*,int,int,int,int);
struct node* delete_back(struct node*);
struct node* delete_front(struct node*);
//Prototypes for Linked List


enum CPUMethods{NONE, FCFS, SJF, PS, RR}method=NONE;//CPU Scheduling Methods Enumeration
enum PMode{OFF,ON}mode=OFF;//Preemtive Mode Enumeration
int time_quantum;

//Prototypes
void print_usage();//Usage showing Function
void menu();//Main Menu Function
void menu1();//Scheduling Methods Menu Function
void menu2();//Preemtive Mode Menu Function
void tq_menu();//Asking the user for time quantum if RR method is selected
//Prototypes


int main(int argc, char *argv[]){
        int options = 0;
        char* input_filename = NULL;
        char* output_filename = NULL;

        //Here we check if the correct options are used
        while ((options = getopt(argc, argv, "f:o:")) != -1)
        {
                switch(options)
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

	//Here we check if the arguments are passed for options
	if (input_filename == NULL || output_filename == NULL)
	{
		print_usage();
	}

	FILE *finput = fopen(input_filename, "r");
	if (finput == NULL)//Checking if the input file argument exists.
	{
		printf("The argument that you passed as input file does not exists.\n");
		printf("Please check the input file argument and run the program again\n");
		exit(1);
	}

	menu();

	int id_counter = 0;
	struct node* header_original = NULL;
	while (!feof(finput))//Reading the input file and recording the values to Linked List
	{
		int a,b,c;
		fscanf(finput, "%d:%d:%d\n",&a,&b,&c);
		header_original = insert_back(header_original,id_counter,a,b,c);
		id_counter++;
	}
	fclose(finput);
    
	return 0;
}

//This funtions is used to print programs usage and what arguments are needed to pass
void print_usage(){
        printf("Usage: cmpe351 -f <*.txt> -o <*.txt>\n");
        exit(1);
}

//Main Menu Function
void menu(){
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
	printf("Option > "); scanf("%1d",&option);

	//Will implement a switch syntax here
	switch (option)
	{
		case 1: menu1(); break;
		case 2: menu2(); break;
	}
}

//Scheduling Methods Menu Function
void menu1(){
	system("clear");
	int option = 0;
	//Modifying Menu according to if preemtive mode is on
	if (mode == 1)
	{
		printf("1-) None: None of scheduling method chosen\n");
		printf("2-) Shortest-Job-First Scheduling\n");
		printf("3-) Priority Scheduling\n");
		printf("Option > "); scanf("%1d",&option);

		switch (option)
		{
			case 1: method = NONE; break;
			case 2: method = SJF; break;
			case 3: method = PS; break;
			default: printf("Please select a valid option\n"); system("sleep 1"); break;
		}
	}
	else
	{
		printf("1-) None: None of scheduling method chosen\n");
		printf("2-) First Come, First Served Scheduling\n");
		printf("3-) Shortest-Job-First Scheduling\n");
		printf("4-) Priority Scheduling\n");
		printf("5-) Round-Robin Scheduling\n");
		printf("Option > "); scanf("%1d",&option);
		switch (option)
		{
			case 1: method = NONE; break;
			case 2: method = FCFS; break;
			case 3: method = SJF; break;
			case 4: method = PS; break;
			case 5: method = RR; tq_menu(); break;
			default: printf("Please select a valid option\n"); system("sleep 1"); break;
		}
	}
	menu();
}

//Preemtive Mode Menu Function
void menu2(){
	system("clear");
	int option = 0;
	if (method == 0)//If any scheduling method is not selected this menu will be shown
	{
		printf("(Scheduling Methods Menu will be modified according to your preemtive mode choice.)\n");
		printf("Please! Next time when you are choosing preemtive mode, ");
		printf("Firstly select the scheduling method first.\n");
		printf("1-) OFF\n");
		printf("2-) ON\n");
		printf("Option > "); scanf("%1d",&option);
		switch (option)
		{
			case 1: mode = OFF; break;
			case 2: mode = ON; break;
			default: printf("Please select a valid option\n"); system("sleep 1"); break;
		}
	}
	else if (method == 2 || method == 3)//If SJF or PS is selected this menu will be shown
	{
		printf("1-) OFF\n");
		printf("2-) ON\n");
		printf("Option > "); scanf("%1d",&option);
		switch (option)
		{
			case 1: mode = OFF; break;
			case 2: mode = ON; break;
			default: printf("Please select a valid option\n"); system("sleep 1"); break;
		}
	}
	else//If FCFS or RR is selected user will be informed that preemtive mode is not available for those methods
	{
		printf("Preemtive mode is not available for selected Scheduling Method\n"); system("sleep 1");
	}

	menu();
}

//Time Quantum Asking Menu
void tq_menu(){
	while (time_quantum == 0)
	{
		system("clear");
		printf("Please enter the time quantum for Round-Robin Method\n");
		printf("Time Quantum > "); scanf("%d",&time_quantum);
	}
}

//Creating node Function
struct node* create_node(int id, int burst_time, int arrival_time, int priority){
	struct node* temp;
    temp = (struct node*) malloc(sizeof(struct node));

	temp->process_id = id;
	temp->burst_time = burst_time;
	temp->arrival_time = arrival_time;
	temp->priority = priority;
	temp->waiting_time = 0;
	temp->turnaround_time = 0;
	temp->is_terminated = false;
	temp->how_much_left = burst_time;
	temp->next = NULL;

	return temp;
}

//Insert back to the LL Function
struct node* insert_back(struct node* header, int id, int burst_time, int arrival_time, int priority){
	struct node* temp = create_node(id, burst_time, arrival_time, priority);
	struct node* header_temp = header;

	//Check if the linked list is empty
	if (header == NULL)
	{
		header = temp;
		return header;
	}

	header_temp = header;
	while (header_temp->next != NULL)//Iterate until we reach the last node
		header_temp = header_temp->next;

	header_temp->next = temp;
	return header;
}

//Insert front of the LL Funtion
struct node* insert_front(struct node* header, int id, int burst_time, int arrival_time, int priority){
	struct node* temp = create_node(id, burst_time, arrival_time, priority);
	temp->next = header;
	header->next = temp;
	return header;
}

//Delete back of the LL Function
struct node* delete_back(struct node* header){
	struct node* temp, *header_temp;

	//Checking if the LL is empty
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

//Delete front of the LL Function
struct node* delete_front(struct node* header){
	struct node* temp;

	if (header == NULL)
	{
		return header;
	}

	temp = header;
	header = header->next;
	free(temp);
	return header;
}

