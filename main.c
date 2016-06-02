#include<stdlib.h>
#include<stdio.h>
#define UNVISITED 0
#define VISITED 1
#define CLOSED 3
#define INFINITE 9
#define OPEN 1
#define EXAUSTED 0

typedef struct list{
	int id;
    struct list * next;
} * list;

typedef struct caminho{
	int id;
	int flow;
    struct caminho * next;
} * caminho;

int found = 0;
list stack_S = NULL;
int * G_s;
void Push (int ID);
int Pop();
list Insere (list head, int ID);
caminho New_share (caminho head, int ID);
void Reset_Unvisited();
void DFS_Visit (caminho * Graph, int id, int destino);
int ford_felkurson (caminho * Graph, int n, int s, int t);
void Exaust_Paths (caminho * G);
void Replenish_Paths (caminho * G, int N);
int HasDirectPath(caminho * Graph, int id, int destino);

int main(){

    int i, j, temp, id1, id2, N, P, PC, PC2, scans, min_paths;
    scans = scanf ("%d %d", &N, &P);
    if (scans != 2) exit(0);
    caminho G[N];
    list pontos_criticos=NULL, aux = NULL;
    G_s = calloc (N, sizeof(int));

	for (i = 0; i < N; i++){
        G[i] = NULL;
        G_s[i] = UNVISITED;
    }

    for (i = 0; i < P; i++){
        scans = scanf ("%d %d", &id1, &id2);
        if (scans != 2) exit(0);
        G[id1] = New_share (G[id1], id2);
        G[id2] = New_share (G[id2], id1);
    }

    scans = scanf ("%d", &PC);
    if (scans != 1) exit(0);

    for (i = 0; i < PC; i++){
        pontos_criticos = NULL;

        min_paths=P;

        scans = scanf ("%d", &PC2);
        if (scans != 1) exit(0);
        for (j=1; j<= PC2; j++){
            scans = scanf ("%d", &id1);
            if (scans != 1) exit(0);
            pontos_criticos = Insere(pontos_criticos, id1);
        }

	while(pontos_criticos != NULL && pontos_criticos->next != NULL){
		aux=pontos_criticos->next;
		while(aux!=NULL){
			temp = ford_felkurson(G, N, pontos_criticos->id, aux->id);

			if (temp<min_paths) min_paths=temp;
			aux = aux->next;
			Replenish_Paths(G, N);
			Reset_Unvisited(N);
		}
		pontos_criticos = pontos_criticos->next;
	}
	printf("%d\n", min_paths);
    }
    return 0;
}

int ford_felkurson (caminho * Graph, int n, int s, int t){
	if (Graph[s]==NULL) return 0;
	int counter=0;
	caminho next = Graph[s];
	while (next != NULL){
		G_s[s] = VISITED;

		if (G_s[next->id] == UNVISITED && next->flow == OPEN){
			DFS_Visit (Graph, next->id, t);
			Reset_Unvisited(n);
			Push(s);
			Exaust_Paths(Graph);
		}
	if (found == 1){
		counter++;

		found=0;
	}
        next = next->next;
    }

	return counter;
}

void DFS_Visit (caminho * Graph, int id, int destino){
    caminho next = Graph[id];
    G_s[id] = VISITED;
    if (id == destino) found=1;

if (HasDirectPath(Graph, id, destino)) {
	found = 1;
	DFS_Visit (Graph, destino, destino);
}

    while (next != NULL && found == 0){

        if (G_s[next->id] == UNVISITED && next->flow==OPEN){
		DFS_Visit (Graph, next->id, destino);
	}
        next = next->next;
    }
    Push (id);
    G_s[id] = CLOSED;
}

list Insere (list head, int ID){
    list temp;
    temp = malloc (sizeof (struct list));
    temp->id = ID;
    temp->next = head;
    return temp;
}

caminho New_share (caminho head, int ID){
    caminho temp;
    temp = malloc (sizeof (struct caminho));
    temp->id = ID;
    temp->flow = OPEN;
    temp->next = head;
    return temp;
}

void Push (int ID){
    stack_S = Insere (stack_S, ID);
}

int Pop (){
    int temp;
    if (stack_S != NULL) {
            temp = stack_S->id;
            stack_S = stack_S->next;
            return temp;
    }
    else return -1;
}

void Reset_Unvisited (int N){
    int i;
    for (i=0; i<N; i++){
        G_s[i] = UNVISITED;
    }
}

void Exaust_Paths(caminho * G){
	int id1, id2;
	id1 = Pop();
	caminho aux;
	while(stack_S != NULL){
	aux = G[id1];
        id2=stack_S->id;
        while (aux != NULL){
            if (aux->id == id2){
                aux->flow = EXAUSTED;
                aux = G[id2];
                while (aux->id != id1) aux=aux->next;
		aux->flow = EXAUSTED;
            }
	
            aux = aux->next;
        }
	id1 = Pop();
	}
}

void Replenish_Paths(caminho * G, int N){
    caminho aux;
    int i;
    for (i = 0; i < N; i++){
        aux = G[i];
        while (aux!=NULL){
            aux->flow = OPEN;
            aux = aux->next;
        }
    }
}

int HasDirectPath(caminho * Graph, int id, int destino){
	caminho next = Graph[id];
	while (next != NULL){
	if (next->id == destino) return 1;
	next = next->next;
	}
	return 0;
}
