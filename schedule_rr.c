// ROUND ROBIN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

struct node *head = NULL;
int numTasks=0, totWait=0, totTurn=0, totResp=0;
int currTurn=0, currWait=0, currResp=0, lastTurn=0;
int i=0;
double avgWait, avgTurn, avgResp;

void order();

void add(char *name, int priority, int burst)
{
	Task *newTask = (Task *) malloc(sizeof(Task));
	
	newTask->name = name;
	newTask->priority = priority;
	newTask->burst = burst;
	
	insert(&head, newTask);
	
	numTasks++;
}

void order()
{
	struct node *temp;
	struct node *temp2;
	
	temp = head;
	
	while (temp != NULL)
	{
		temp2 = temp->next;
		
		delete(&head, temp->task);
		insert(&head, temp->task);
		
		temp = temp2;
	}
}

void schedule()
{
	order();
	
	Task *curr;
	char *tempName; int tempPriority; int tempBurst;
	
	while (head != NULL)
	{
		curr = head->task;
		
		if (curr->burst <= QUANTUM) 
		{
			if (i < numTasks)
			{
				currResp = lastTurn;
				
				totResp += currResp;
			}
			
			totWait += lastTurn;
			lastTurn += (curr->burst);
			
			totTurn += lastTurn;
			
			run(curr, curr->burst);
			
			delete(&head, curr);
		}
		else
		{
			if (i < numTasks)
			{
				currResp = lastTurn;
				
				totResp += currResp;
			}
			
			lastTurn += QUANTUM;
			
			totWait -= QUANTUM;
			
			tempName = curr->name;
			tempPriority = curr->priority;
			tempBurst = curr->burst - QUANTUM;
			
			run(curr, QUANTUM);
			
			delete(&head, curr);
			order();
			add(tempName, tempPriority, tempBurst);
			order();
			
			numTasks--;
		}
		
		i++;
	}
	
	avgWait = (double) totWait / numTasks;
	avgTurn = (double) totTurn / numTasks;
	avgResp = (double) totResp / numTasks;
	
	printf("\nAverage waiting time = %.2lf\n", avgWait);
	printf("Average turnaround time = %.2lf\n", avgTurn);
	printf("Average response time = %.2lf\n", avgResp);
}
