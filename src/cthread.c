#include <string.h>
#include <stdlib.h>
#include "../include/cthread.h"
#include "../include/cdata.h"
#include "../src/insert.c"

#define bool 			int
#define false			0
#define true			1

#define CODE_SUCCESS    0
#define CODE_ERROR      -1
#define NOT_IMPLEMENTED -1

#define STACK_SIZE 		16384
bool retcode = false;
int guid = 0;
static	TCB_t* mainThread = NULL;
static	TCB_t* currentThread = NULL;

FILA2 filaAptos;
FILA2 filaBloqueados;
FILA2 filaJoin;


ucontext_t unblockContext;
ucontext_t context_threadended;
void printQueuejoin(PFILA2 fila)
{
	FirstFila2(fila);
	int* t = (int*) GetAtIteratorFila2(fila);
	//printf("TID Queuejoin:\t");
	while(t != NULL)
	{
		//printf("rule %d->%d ", t->waiting, t->join);
		NextFila2(fila);
		t = (int*) GetAtIteratorFila2(fila);
	}
	//printf("\n");
}
int findJoin(int id)
{

	int *t;
	FirstFila2(&filaJoin);

	//printf("was joining for thread %d\n",id);
	do
	{

		////printf("wololo\n");
		t = (int*) GetAtIteratorFila2(&filaJoin);
		if(t == NULL)
		{

			////printf("wololosuper\n");
			return -1;
		}

		////printf("wololo2\n");
		if (t->join == id)
		{

			////printf("wololo3\n");
			DeleteAtIteratorFila2(&filaJoin);
			//printf("was waiting for thread %d\n",t->waiting);
			return t->waiting;
		}

		////printf("wololox\n");
		NextFila2(&filaJoin);
	}while(GetAtIteratorFila2(&filaJoin) != NULL);

	return -1;
}
void insertInJoin(int waiting, int join)
{
	////printf("Inserindo thread id %d na fila de aptos.\n", int.waiting);
	int* new = (int*) malloc(sizeof(int));
	new->waiting = waiting;
	new->join = join;
	//FirstFila2(&filaJoin);
	AppendFila2(&filaJoin, new);

	//printf("inserting waiting %d, join %d in joinfila\n", new->waiting,new->join);
	printQueuejoin(&filaJoin);
	return ;	
}

TCB_t* findTCB(PFILA2 fila, int thread_id)
{
	TCB_t* t;
	FirstFila2(fila);

	if (fila == NULL)
		return NULL;
	do
	{
		t = (TCB_t*) GetAtIteratorFila2(fila);
		if (t == NULL)
			{return NULL;}
		if (t->tid == thread_id)
		{
			return t;
		}
		NextFila2(fila);
	}while(GetAtIteratorFila2(fila) != NULL);

	return NULL;
}

int isMainThread(int thread_id)
{
	return thread_id == 0;
}

void printQueue(PFILA2 fila)
{
	FirstFila2(fila);
	TCB_t* t = (TCB_t*) GetAtIteratorFila2(fila);
	//printf("TID Queue:\t");
	while(t != NULL)
	{
		//printf("%d, ", t->tid);
		NextFila2(fila);
		t = (TCB_t*) GetAtIteratorFila2(fila);
	}
	//printf("\n");
}

int insertInAptos(TCB_t *t)
{
	//printf("Inserindo thread id %d na fila de aptos.\n", t->tid);
	int temp = 0;
	if(isMainThread(t->tid))
	{
		//printf("inserting main in aptos\n");
		temp = FirstFila2(&filaAptos);
		if(temp != 0)
		{
			////printf("first didn't work\n");
			AppendFila2(&filaAptos, t);
			printQueue(&filaAptos);
			return 0;
		}
		temp = InsertBeforeIteratorFila2(&filaAptos, t);
		return 0;
		//if(temp != NULL)
			////printf("insert didn't work\n");
	}
	else
		InsertByPrio(&filaAptos, t);
	t->state = PROCST_APTO;
	printQueue(&filaAptos);
	return 0;
}

TCB_t* removeFromBloqueados(tid)
{
	TCB_t* fila;
	fila = findTCB(&filaBloqueados, tid);
	DeleteAtIteratorFila2(&filaBloqueados);
	//printf("removing %d from bloqueados\n", tid);
	return fila;
}

//Forward declaration??
static void threadUnblock();

bool initialized = false;
void Initialize()
{
	if(!initialized)
	{
		initialized = true;
		//printf("initialize begin\n");
		guid = 1;

		// Inicializa Filas
		CreateFila2(&filaAptos);
		CreateFila2(&filaBloqueados);
		// Inicializa Main Thread
		mainThread = (TCB_t*) malloc(sizeof(TCB_t));
		mainThread->tid = 0;
		mainThread->state = PROCST_CRIACAO;
		mainThread->prio = 0;
		mainThread->context.uc_link = NULL;
		mainThread->context.uc_stack.ss_sp = malloc(STACK_SIZE);
		mainThread->context.uc_stack.ss_size = STACK_SIZE;
		getcontext(&mainThread->context);
		currentThread = mainThread;
		currentThread->state = PROCST_EXEC;
		// Seta flag de inicialização
		//printf("initialize end\n");

		unblockContext.uc_link = NULL;
		unblockContext.uc_stack.ss_sp = malloc(STACK_SIZE);
		unblockContext.uc_stack.ss_size = STACK_SIZE;
		getcontext(&unblockContext);
		makecontext(&unblockContext, threadUnblock, 0);
	}
}



int insertInBloqueados(TCB_t *t)
{
	t->state = PROCST_BLOQ;
	AppendFila2(&filaBloqueados, t);
	return 0;
}

int cidentify (char *name, int size)
{
	char *nome_grupo = "Jessica Lorencetti - 228342, Lucas Pizzo - 220484\n";
	strncpy(name, nome_grupo, size);
    return 0;
}

TCB_t* GetNextThread()
{
	FirstFila2(&filaAptos);
	TCB_t* next = (TCB_t*) GetAtIteratorFila2(&filaAptos);
	DeleteAtIteratorFila2(&filaAptos);
	return next;
}

static void schedule(void)
{
	//printf("Escalonador...\n");
	TCB_t* temp = currentThread;
	currentThread = GetNextThread();
	if(currentThread == NULL)
	{
		//printf("Finalizado por fila de aptos vazia\n");
		//return;
		exit(0);
	}

	currentThread->state = PROCST_EXEC;
	//printf("vai executar thread %d \n",currentThread->tid);
	if(&temp->context != NULL)
	{
		swapcontext(&temp->context, &currentThread->context);
		startTimer();
	}
	else
	{
		setcontext(&currentThread->context);
		startTimer();
	}
	retcode = true;
	//setcontext(&currentThread->context);
	////printf("nunca sera executado da schedule");
	//printf("retorna a executar thread %d \n",currentThread->tid);
	return;
}


int cyield(void)
{
	unsigned int time;
	Initialize();
	time = stopTimer();
	currentThread->prio += time;
	insertInAptos(currentThread);
	schedule();
	//printf("cyeld finalizando\n");
	return CODE_SUCCESS;
}

static void threadUnblock()
{
	//printf("thread finished\n");
	//schedule();
	TCB_t* t;// = removeFromBloqueados(tid);
	//insertInAptos(unblockedThread);
	int temp = findJoin(currentThread->tid);
	if (temp !=-1)
	{
		t = removeFromBloqueados(temp);
		insertInAptos(t);
		//printf("thread desbloqueada\n");
		schedule();
	}
	//printf("no thread waiting for this thread\n");
	schedule();
	//printf("nunca sera executado da threadUnblock\n");
}

int ccreate (void* (*start)(void*), void *arg, int prio)
{
	Initialize();


	/*TCB_t* t2 = (TCB_t*) malloc(sizeof(TCB_t));
	t2->tid = -1;
	t2->state = PROCST_CRIACAO;
	t2->prio = 0;
	t2->context.uc_link = NULL;
	t2->context.uc_stack.ss_sp = malloc(STACK_SIZE);
	t2->context.uc_stack.ss_size = STACK_SIZE;
	getcontext(&t2->context);
	
	ucontext_t temp_context;
	getcontext(&temp_context);
	temp_context.uc_link = 0;
	temp_context.uc_stack.ss_sp = malloc(STACK_SIZE);
	temp_context.uc_stack.ss_size = STACK_SIZE;
	temp_context.uc_stack.ss_flags=0;
	makecontext(&temp_context, (void*) &threadUnblock, 0);*/

	TCB_t* t = (TCB_t*) malloc(sizeof(TCB_t));
	t->tid = guid++;
	t->state = PROCST_CRIACAO;
	t->prio = 0;
	t->context.uc_link = &unblockContext;
	t->context.uc_stack.ss_sp = malloc(STACK_SIZE);
	t->context.uc_stack.ss_size = STACK_SIZE;
	getcontext(&t->context);
	makecontext(&t->context, (void*) start, 1, arg);
	insertInAptos(t);
	return t->tid;
}
int cjoin(int tid)
{
	unsigned int time;
	Initialize();
	time = stopTimer();
	currentThread->prio += time;

	retcode = false;
	TCB_t* joinThread = findTCB(&filaAptos, tid);
	if(joinThread == NULL) 
	{
		joinThread = findTCB(&filaBloqueados,tid);
		if(joinThread == NULL)
			return CODE_ERROR;
	}

	currentThread->state = PROCST_BLOQ;
	insertInBloqueados(currentThread);
/*
	ucontext_t* uc_unblock = (ucontext_t*) malloc(sizeof(ucontext_t));
	uc_unblock->uc_link = NULL;
	uc_unblock->uc_stack.ss_sp = malloc(STACK_SIZE);
	uc_unblock->uc_stack.ss_size = STACK_SIZE;
	uc_unblock->uc_stack.ss_flags=0;
	getcontext(uc_unblock);
	makecontext(uc_unblock, (void (*)(void)) threadUnblock, 1, currentThread->tid);
	//printf("joinThread->context.uc_link antes de mudar uclink: %p\n", joinThread->context.uc_link);
	joinThread->context.uc_link = uc_unblock;
*/
	insertInJoin(currentThread->tid,tid);
	//printf(" tid  : %d esperando tid  : %d add : %p\n", currentThread->tid,joinThread->tid, joinThread->context.uc_link);
	schedule();
	//printf("nunca sera executado da cjoin \n");
	return CODE_SUCCESS;
}

int csem_init(csem_t *sem, int count)
{
	Initialize();

	sem->count = count;
	sem->fila = (PFILA2)malloc(sizeof(PFILA2));
	return CODE_SUCCESS;
}

int cwait(csem_t *sem)
{
	unsigned int time;
	Initialize();

	sem->count -= 1;
	if (sem->count < 0)
	{
		
		time = stopTimer();
		currentThread->prio += time;
		insertInBloqueados(currentThread);
		schedule();

	}
	////////////// scalonate();
	return CODE_SUCCESS;
}

int csignal(csem_t *sem)
{
	Initialize();

	FirstFila2(sem->fila);

	TCB_t * t = (TCB_t*) GetAtIteratorFila2(sem->fila);

	if (t != NULL)
	{

		t = removeFromBloqueados(t);
		insertInAptos(t);
		schedule();
	}


	return CODE_SUCCESS;
}
