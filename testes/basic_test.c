/*
 *	Programa de exemplo de uso da biblioteca cthread
 *
 *	Versão 1.0 - 14/04/2016
 *
 *	Sistemas Operacionais I - www.inf.ufrgs.br
 *
 */

#include "../include/support.h"
#include "../include/cthread.h"
#include "ucontext.h"
#include <stdio.h>

void* func0(void *arg) {
	printf("Func0 antes da cyield\n");
	cyield();
	printf("Eu sou a thread ID0 imprimindo %d\n", *((int *)arg));
	return NULL;
}

void* func1(void *arg) {
	printf("Func1 antes da cyield\n");
	cyield();
	printf("Eu sou a thread ID1 imprimindo %d\n", *((int *)arg));
	//exit(0);
	return NULL;
}

void* func2(void *arg) {
	printf("Eu sou a thread ID2 imprimindo %d\n", *((int *)arg));
	return NULL;
}

int main(int argc, char *argv[]) {

	int i = 12345;
	char  yay[60];

	cidentify (yay, 60);
	printf("%s", yay);

	int id1 = ccreate(func0, (void *)&i, 0);
	int id2 = ccreate(func1, (void *)&i, 0);
	//int id3 = ccreate(func2, (void *)&i, 0);
	cjoin(id2);
	printf("\nEu sou a thread main depois do cjoin id2 \n");
	cjoin(id1);
	printf("\nEu sou a thread main terminando \n");

	//printf("idthread: %d\n", id0);

	return 0;
}
