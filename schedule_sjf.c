// SHORTEST-JOB-FIRST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

struct node *head = NULL;
int numTasks=0, totWait=0, totTurn=0;
int currTurn=0, currWait=0, lastTurn=0;
double avgWait, avgTurn;

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
	
	while (head != NULL)
	{
		curr = pickNextTask();
		
		currWait = lastTurn;
		lastTurn += (curr->burst);
		
		totWait += currWait;
		totTurn += lastTurn;
		
		run(curr, curr->burst);
		
		delete(&head, curr);
	}
	
	avgWait = (double) totWait / numTasks;
	avgTurn = (double) totTurn / numTasks;
	
	printf("\nAverage waiting time = %.2lf\n", avgWait);
	printf("Average turnaround time = %.2lf\n", avgTurn);
	printf("Average response time = %.2lf\n", avgWait);
}

Task *pickNextTask()
{
	struct node *temp;
	Task *picked = head->task;
	
	temp = head->next;
	
	while (temp != NULL)
	{
		if (temp->task->burst < picked->burst)
		{
			picked = temp->task;
		}
		
		temp = temp->next;
	}
	
	return picked;
}