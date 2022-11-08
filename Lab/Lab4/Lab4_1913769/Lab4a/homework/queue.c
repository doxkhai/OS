
#include <stdlib.h>
#include "queue.h"
#include <pthread.h>
#include <stdio.h>

/* Remember to initilize the queue before using it */
void initialize_queue(struct pqueue_t *q)
{
	q->head = q->tail = NULL;
	pthread_mutex_init(&q->lock, NULL);
}

/* Return non-zero if the queue is empty */
int empty(struct pqueue_t *q)
{
	return (q->head == NULL);
}

/* Get PCB of a process from the queue (q).
 * Return NULL if the queue is empty */


struct pcb_t *de_queue(struct pqueue_t *q)
{
	struct pcb_t *proc;
	// TODO: return q->head->data and remember to update the queue's head
	// and tail if necessary. Remember to use 'lock' to avoid race
	// condition

	// YOUR CODE HERE
	pthread_mutex_lock(&(q->lock));
	if (empty(q))
	{
		proc = NULL;
	}
	else
	{
		struct qitem_t *delPtr = q->head;
		proc = delPtr->data;
		q->head = q->head->next;
		if (q->head == NULL)
			q->tail = NULL;
		free(delPtr);
	}
	pthread_mutex_unlock(&(q->lock));
	return proc;
}

/* Put PCB of a process to the queue. */
void en_queue(struct pqueue_t *q, struct pcb_t *proc)
{
	// TODO: Update q->tail.
	// Remember to use 'lock' to avoid race condition

	// YOUR CODE HERE
	struct qitem_t *qitem = (struct qitem_t *)malloc(sizeof(struct qitem_t));
	qitem->data = proc;
	qitem->next = NULL;

	pthread_mutex_lock(&(q->lock));
	if (empty(q))
	{
		q->head = q->tail = qitem;
		pthread_mutex_unlock(&(q->lock));
		return;
	}

	q->tail->next = qitem;
	q->tail = qitem;
	pthread_mutex_unlock(&(q->lock));
	return;
}

void en_priority_queue(struct pqueue_t *q, struct pcb_t *proc)
{
	struct qitem_t *qitem = (struct qitem_t *)malloc(sizeof(struct qitem_t));
	qitem->data = proc;
	qitem->next = NULL;

	pthread_mutex_lock(&(q->lock));
	if (empty(q))
	{
		q->head = q->tail = qitem;
	}
	else
	{
		struct qitem_t *wlk = q->head;
		// If head has less priority then proc
		if (q->head->data->priority > proc->priority)
		{
			qitem->next = q->head;
			q->head = qitem;
		}
		else
		{
			while (wlk->next != NULL && wlk->next->data->priority < proc->priority)
			{
				wlk = wlk->next;
			}
			qitem->next = wlk->next;
			wlk->next = qitem;
		}
	}
	pthread_mutex_unlock(&(q->lock));
	return;
}
