#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));

    if (!q)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    if (!q)
        return;

    while (q->head) {
        list_ele_t *tmp = q->head;
        q->head = q->head->next;
        free(tmp->value);
        free(tmp);
    }
    /* Free queue structure */
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    int length = strlen(s) + 1;
    newh->value = (char *) malloc(length * sizeof(char));
    if (!newh->value) {
        free(newh);
        return false;
    }
    snprintf(newh->value, length, "%s", s);
    newh->next = NULL;
    q->size++;
    /*newEle is the first element in queue*/
    if (!q->tail) {
        q->tail = newh;
    }
    newh->next = q->head;
    q->head = newh;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */

bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newEle;
    newEle = malloc(sizeof(list_ele_t));
    if (!newEle)
        return false;

    int length = strlen(s) + 1;
    newEle->value = (char *) malloc(length * sizeof(char));
    if (!newEle->value) {
        free(newEle);
        return false;
    }
    snprintf(newEle->value, length, "%s", s);
    newEle->next = NULL;
    q->size++;
    // newEle is the first element in queue
    if (!q->tail) {
        q->head = newEle;
    } else {
        q->tail->next = newEle;
    }
    q->tail = newEle;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;

    snprintf(sp, bufsize, "%s", q->head->value);
    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    free(tmp->value);
    free(tmp);
    q->size--;

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q || !q->head)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head)
        return;

    q->tail = q->head;
    list_ele_t *cursor = NULL;
    list_ele_t *head = q->head;
    while (head) {
        list_ele_t *next = head->next;
        head->next = cursor;
        cursor = head;
        head = next;
    }
    q->head = cursor;
}

void MoveNode(list_ele_t **dest, list_ele_t **src)
{
    list_ele_t *tmp = *src;
    *src = tmp->next;  // src sublist goes forward
    tmp->next = *dest;
    *dest = tmp;
}

list_ele_t *merge(list_ele_t *ll, list_ele_t *rl)
{
    list_ele_t *res = NULL;
    list_ele_t **indirect = &res;

    while (1) {
        if (!ll) {
            *indirect = rl;
            break;
        } else if (!rl) {
            *indirect = ll;
            break;
        }
        if (strcmp(ll->value, rl->value) <= 0)
            MoveNode(indirect, &ll);
        else
            MoveNode(indirect, &rl);

        indirect = &((*indirect)->next);
    }
    return res;
}

list_ele_t *mergeSort(list_ele_t *head)
{
    if (!head || !head->next)
        return head;

    // divide into two halves
    list_ele_t *twoStep = head->next;
    list_ele_t *oneStep = head;

    while (twoStep) {
        twoStep = twoStep->next;
        if (twoStep) {
            twoStep = twoStep->next;
            oneStep = oneStep->next;
        }
    }
    twoStep = oneStep->next;
    oneStep->next = NULL;

    // recursively call mergeSort
    list_ele_t *ll = mergeSort(head);
    list_ele_t *rl = mergeSort(twoStep);
    // merge two ordered lists
    return merge(ll, rl);
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head || q->size == 1)
        return;

    q->head = mergeSort(q->head);
    // update q->tail
    while (q->tail->next)
        q->tail = q->tail->next;
}
