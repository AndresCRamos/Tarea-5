#include<string.h>
#include<stdlib.h>

#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"

struct node *taskList = NULL;
struct node *next_node;

// add a task to the list
void add(char *name, int priority, int burst) {
    Task *t = malloc(sizeof(Task));
    // allocate memory and then copy the name
    t->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(t->name, name);
    // priority and burst
    t->priority = priority;
    t->burst = t->remaining_burst = burst;
    // insert into task list
    insert(&taskList, t);
}

/* pickNextTask: pick the next task to execute with Round Robin
 * taskList must not be empty!
 */
Task *pickNextTask() {
    Task *ret = next_node->task;
    /*
    ? es un operador ternario de C
    Evalua la primera expresion
    Si es verdadera, ejecuta la segunda, si es falsa, retorna la tercera
    
    Es un if en una linea, basicamente
    */
    next_node = (next_node -> next) ? next_node->next : taskList;
    return ret;
}

// invoke the scheduler
void schedule() {
    next_node = taskList;
    // Mientras taskList no sea nulo
    while(taskList) {
        // trae la siguiente taera
        Task *t = pickNextTask();
        /*
        slice representa el tiempo del siguiente ciclo de ejecucion, 
        Si aun queda mas tiempo del especificado por QUANTUM, slice se vuelve el valor de QUANTUM. En caso de que sea menos, slice es el tiempo restante de ejecucion
        */
        int slice = QUANTUM < t->remaining_burst ? QUANTUM : t->remaining_burst;
        // corre el proceso t, en slice ciclos
        run(t, slice);
        // actualiza el tiempo restante del proceso t
        t->remaining_burst -= slice;
        // si tiempo restante del proceso es 0, entonces este se elimina de la cola de ejecucion
        if(!t->remaining_burst) {
            delete(&taskList, t);
        }
    }
}
