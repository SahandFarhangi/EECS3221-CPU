// PRIORITY - ROUND ROBIN

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
int RR=0, i=0, j=0;
double avgWait, avgTurn, avgResp;

Task *pickNextTask();
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
		curr = pickNextTask();
		
		if (RR == 0)
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
			if (curr->burst <= QUANTUM)
			{
				if (j < RR)
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
				if (j < RR)
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
				
				j++;
			}
		}
	}
	
	avgWait = (double) totWait / numTasks;
	avgTurn = (double) totTurn / numTasks;
	avgResp = (double) totResp / numTasks;
	
	printf("\nAverage waiting time = %.2lf\n", avgWait);
	printf("Average turnaround time = %.2lf\n", avgTurn);
	printf("Average response time = %.2lf\n", avgResp);
}

Task *pickNextTask()
{
	struct node *temp;
	Task *picked = head->task;
	
	temp = head->next;
	
	while (temp != NULL)
	{
		if (temp->task->priority > picked->priority) 
		{
			picked = temp->task;
			RR=0;
		}
		else if (temp->task->priority == picked->priority)
		{
			RR++;
		}
		
		temp = temp->next;
	}
	
	return picked;
}