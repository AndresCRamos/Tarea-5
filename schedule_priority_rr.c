#include<string.h>
#include<stdlib.h>

#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"

#include <stdio.h>

struct node *taskList[MAX_PRIORITY + 1];
struct node *next_node;

void print_struct_elements(Task *T) {
    printf("\nContents of a structure %s are:\n", T->name);
    printf("thread_id: %d\n",T->tid);
}

// add a task to its priority list
void add(char *name, int priority, int burst) {
    Task *t = malloc(sizeof(Task));
    // allocate memory and then copy the name
    t->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(t->name, name);
    // priority and burst
    t->priority = priority;
    t->burst = t->remaining_burst = burst;
    // insert into task list

    // Al insertarlos de esta forma, quedan ordenados por prioridad
    Task* parent = &taskList[priority]->task;
    print_struct_elements(parent);
    print_struct_elements(t);
    insert(&taskList[priority], t);
}

/* pickNextTask: pick the next task to execute with Priority + Round Robin
 * taskList must not be empty!
 */
Task *pickNextTask(struct node *tl) {
    // trae la cabeza de la lista
    Task *ret = next_node->task;
    /*
    ? es un operador ternario de C
    Evalua la primera expresion
    Si es verdadera, ejecuta la segunda, si es falsa, retorna la tercera
    
    Es un if en una linea, basicamente

    En este caso, retorna un nodo especifico
    */
    next_node = (next_node -> next) ? next_node->next : tl;
    return ret;
}

// invoke the scheduler
void schedule() {
    // from higher priority to lower priority
    // Itera por todas las prioridades, desde la menos hasta la mayor, desde 10 hasta 1
    for(size_t p = MAX_PRIORITY; p >= MIN_PRIORITY; --p) {
        // Toma una tarea de la cola de ejecucion con prioridad p
        next_node = taskList[p];
        //mientras existan tareas en la cola de ejecucion con prioridad p
        while(taskList[p]) {
            /*
            Este bucle es un scheduling por rr, donde se ejecuta el proceso un tiempo determinado, se actualiza su tiempo restante de ejecucion, y se pasa al siguiente, asi hasta acabar con los procesos en la cola

            En este caso, se agrupan los procesos por prioridad, y se aplica Round Robin sobre dicho grupo, antes de pasar al siguiente grupo, con menor prioridad.
            */
            Task *t = pickNextTask(taskList[p]);
            int slice = QUANTUM < t->remaining_burst ? QUANTUM : t->remaining_burst;
            run(t, slice);
            t->remaining_burst -= slice;
            if(!t->remaining_burst) {
                delete(&taskList[p], t);
            }
        }
    }
}
