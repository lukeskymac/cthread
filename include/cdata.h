/*
 * cdata.h: arquivo de inclus�o de uso apenas na gera��o da libpithread
 *
 * Esse arquivo pode ser modificado. ENTRETANTO, deve ser utilizada a TCB fornecida.
 * Se necess�rio, pode-se ACRESCENTAR campos ao TCP
 *
 * VERS�O: 17/03/2018
 *
 */
#ifndef __cdata__
#define __cdata__
#include "ucontext.h"

#define	PROCST_CRIACAO	0
#define	PROCST_APTO	1
#define	PROCST_EXEC	2
#define	PROCST_BLOQ	3
#define	PROCST_TERMINO	4
#define	PROCST_APTO_SUS	5
#define	PROCST_BLOQ_SUS	6

/* Os campos "tid", "state" e "context" dessa estrutura devem ser mantidos e usados convenientemente
   Pode-se acrescentar outros campos AP�S os campos obrigat�rios dessa estrutura
*/
typedef struct s_TCB { 
	int		tid; 		// identificador da thread
	int		state;		// estado em que a thread se encontra
					// 0: Cria��o; 1: Apto; 2: Execu��o; 3: Bloqueado; 4: T�rmino; 5: Apto-Suspenso; 6: Bloqueado-Suspenso
	int 		prio;		// Prioridade associada a thread N�O USADO NESSA IMPLEMENTA��O
	ucontext_t 	context;	// contexto de execu��o da thread (SP, PC, GPRs e recursos) 
	
	/* Se necess�rio, pode-se acresecentar campos nessa estrutura A PARTIR DAQUI! */
		ucontext_t on_thread_end;
	
	
} TCB_t; 
typedef struct s_int2{ 
	int		waiting; 		// thread that is blocked waiting for another thread
	int		join;		// thread that will unblock the other thread 
} int2;
void Initialize();
static void schedule(void);
static void threadUnblock(void);


#endif
