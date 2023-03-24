#include <stdlib.h>
#include <stdio.h>
#define INFINITO 10000000 // Definindo um valor alto para facilitar a resolução;

typedef struct
{
    int numero;    // Numero do voo.
    int partida;   // Cidade de partida.
    int chegada;   // Cidade de chegada.
    int tempo; // Tempo de uma cidade para a outra.

} Dados;

typedef struct
{
    int n;
    int **Adj;
} Grafo;

typedef struct
{
    int n;
    Dados **Adj;
} GrafoStructs;


Dados lerVoo(Grafo *G, int v)
{
    Dados Voo;
    Voo.numero = G->Adj[v][0];
    Voo.partida = G->Adj[v][1];
    Voo.chegada = G->Adj[v][2];
    Voo.tempo = G->Adj[v][3];
    return Voo;
}

Dados VooVazio()
{
    Dados Voo;
    Voo.numero = 0;
    Voo.chegada = 0;
    Voo.partida = 0;
    Voo.tempo = 0;
    return Voo;
}

void CriaGrafoVazio(Grafo *, int);
void InsereAresta(Grafo, int, int, int);
int ExisteAresta(Grafo, int, int);
void RemoveAresta(Grafo, int, int);
void LiberaGrafo(Grafo *);
void ImprimeGrafo(Grafo);
void LeGrafoArquivo(Grafo *, char *);
void ImprimeGrafoArquivo(GrafoStructs, char *);
void imprimirVoo(Dados);
void CriaGrafoDeStructs(GrafoStructs *, int );
void preencherGrafoStrucs(GrafoStructs *, Grafo *, int );
void ImprimeGrafoStructs(GrafoStructs);
int menorTempo(float *, int *, int);
void menorCaminho(GrafoStructs *, int, int *, float *, int, int);



int main(){
    int quantidadeCidades = 9;
    Grafo G;
    Dados voo;
    GrafoStructs GS;
    int i = 0;
    int predecessor[quantidadeCidades]; float tempo[quantidadeCidades];
    int resp; char c, nomeArq[50]; int destino;
    do{
		printf("\n\n----- COMPANHIA AEREA GOOD FEELINGS ----- \n\n");
        printf("Escolha uma opcao:\n");
		printf("(1) Realizar a leitura do grafo com dados da companhia aerea;\n");
		printf("(2) Escolher destino\n");
        printf("(3) Salvar grafo em um arquivo\n");
		printf("(0) Sair do programa.\n");
		printf("\nOpcao: ");
		scanf("%d", &resp);

		switch(resp) 
		{
			case (1):
				printf("\n\nDigite o nome do arquivo que contem o grafo: ");
				fflush(stdin);
				scanf("%s", nomeArq);
				LeGrafoArquivo (&G, nomeArq);
                CriaGrafoDeStructs(&GS, quantidadeCidades);
                preencherGrafoStrucs(&GS, &G, G.n);
				break;
				
			case (2):
                printf("Escolha um destino partindo da cidade Joy\n");
                printf("(1) Hope\n");
                printf("(2) Friendship\n");
                printf("(3) Surprise\n");
                printf("(4) Honesty\n");
                printf("(5) Wonderland\n");
                printf("(6) Optismism\n");
                printf("(7) Confidence\n");
                printf("(8) Happiness\n");
                scanf("%i", &destino);
                if (destino >= 1 && destino <= 8){
                    menorCaminho(&GS, 0, predecessor, tempo, destino, quantidadeCidades);  }
                else{
                    printf("\nDigite uma opção válida\n");}
                break;
			case (3):
				printf("\n\nDigite o nome do arquivo onde deseja salvar o grafo:");
				fflush(stdin);
				scanf("%s", nomeArq);
				ImprimeGrafoArquivo(GS, nomeArq);
				break;
			case (0):
				printf("Fim do programa!");
				break;
			
			default :
				printf("Opcao Invalida!");
			break;
		}

		//getch();
	}while(resp != 10);
}




void CriaGrafoVazio(Grafo *G, int n)
{
    int u;
    int v;

    G->n = n;
    /* alocando espaço de memória para a matriz de adjacência */

    G->Adj = (int **)malloc(n * sizeof(int *));
    for (u = 0; u < n; u++)
        G->Adj[u] = (int *)malloc(n * sizeof(int));
    /* ===================================================== */

    /* inicializando o grafo vazio */
    for (u = 0; u < n; u++)
        for (v = 0; v < n; v++)
            G->Adj[u][v] = 0;
}

/* libera o espaço de memória alocado para o grafo g */
void LiberaGrafo(Grafo *G)
{
    int u;

    for (u = 0; u < G->n; u++)
        free(G->Adj[u]);
    free(G->Adj);
    G->n = 0;
}

/* imprime na tela uma lista de adjacência de cada vértice */
void ImprimeGrafo(Grafo G)
{
    int u, v;

    for (u = 0; u < G.n; u++)
    {
        {
            for (v = 0; v < G.n; v++)
                printf("%d ", G.Adj[u][v]);
        }
        printf("\n");
    }
    printf("\n");
}

void LeGrafoArquivo(Grafo *G, char *arq)
{
    int n, i, j;
    FILE *f;
    f = fopen(arq, "r");
    fscanf(f, "%d", &n);
    CriaGrafoVazio(G, n);
    for (i = 0; i < G->n; i++)
        for (j = 0; j < G->n; j++)
            fscanf(f, "%d", &G->Adj[i][j]);
    fclose(f);
}

void imprimirVoo(Dados voo)
{
    printf("\tNumero: %d\n", voo.numero);
    printf("\tPartida: %d\n", voo.partida);
    printf("\tChegada: %d\n", voo.chegada);
    printf("\tTempo: %d\n", voo.tempo);
}

void CriaGrafoDeStructs(GrafoStructs *G, int n)
{
    int u;
    int v;

    G->n = n;
    /* alocando espaço de memória para a matriz de adjacência */

    G->Adj = (Dados **)malloc(n * sizeof(Dados *));
    for (u = 0; u < n; u++)
        G->Adj[u] = (Dados *)malloc(n * sizeof(Dados));
    /* ===================================================== */

    /* inicializando o grafo vazio */
    for (u = 0; u < n; u++)
        for (v = 0; v < n; v++)
            G->Adj[u][v] = VooVazio();
}

void preencherGrafoStrucs(GrafoStructs *G, Grafo *G1, int n)
{ 
    int i = 0;
    Dados voo;
    for (i = 0; i < n; i++)
    {
        voo = lerVoo(G1, i);
        G->Adj[voo.partida][voo.chegada] = voo;
    }
}


void ImprimeGrafoStructs(GrafoStructs G)
{
    int u, v;

    for (u = 0; u < G.n; u++)
    {
        {
            for (v = 0; v < G.n; v++)
                printf("%d ", G.Adj[u][v].tempo);
        }
        printf("\n");
    }
    printf("\n");
}

int menorTempo(float *tempo, int *visitado, int quantidadeCidades){

    int i;
    int menor = INFINITO;
    int primeiro = 1;

    for(i = 0; i < quantidadeCidades; i++){
        if(tempo[i] < menor && visitado[i] == 0){
            if(primeiro){
                menor = i;
                primeiro = 0;
            }else{
                if(tempo[menor] > tempo[i])
                    menor = i; }
        }
    }
    return menor;
}

void menorCaminho(GrafoStructs *G, int inicial, int *predecessor, float *tempo, int destino, int n){

    int i, cont = 0, NaoVisitado = 0, ind = 0, *visitado, vertice_processado = 0;
    int *cidadesCaminho ;
    cont = n;
    NaoVisitado = n;
    int j = -1;
    cidadesCaminho = (int*) malloc(n * sizeof(int));

    visitado = (int*) malloc(NaoVisitado * sizeof(int));

    for(i = 0; i < NaoVisitado; i++){

        predecessor[i] = -1;
        tempo[i] = 999999;
        visitado[i] = 0;
    }
    

    tempo[inicial] = 0;

    while(cont > 0){

        vertice_processado = menorTempo(tempo, visitado, NaoVisitado);
        int i = 0;
        visitado[vertice_processado] = 1;
        cont--;
        int j;
        for(j = 0; j < n; j++){
            if(G->Adj[vertice_processado][j].tempo > 0){
                ind = G->Adj[vertice_processado][j].tempo + tempo[vertice_processado];
                if(ind < tempo[j]){
                    predecessor[j] = vertice_processado;
                    tempo[j] = ind;
                }
                ind = 0;
            }
        }
        

    }

    if (tempo[destino] != 999999){
         printf("\n\nO tempo da cidade %d ate a cidade %d eh: %.0f \n", inicial, destino, tempo[destino]);
         printf("Para chegar ate o destino o caminho eh:\n ");
         if (predecessor[destino] == 0){
            printf("%i", destino);
         }
         else{
            j = predecessor[destino];
            printf("Codigo da cidade: ", j);
            while (j != inicial){
                for(i = 0; i < n; i++){
                    if(j != -1){
                    printf(" %i --> ", j);
                    j = predecessor[j]; 
                    }
                }
            }
            printf("\n");
         }
    }
    else{
        printf("\n\n Nao existe um voo disponivel para esta cidade no momento! \n\n");
    }
   
    free(visitado);

}


void ImprimeGrafoArquivo (GrafoStructs G, char *arq)
{  
	int i, j;
	FILE *f;
	f = fopen (arq, "w");
	fprintf (f, "%d\n", G.n);
	for (i = 0; i < G.n; i++)
	{
	    for (j = 0; j < G.n; j++)
	      	fprintf (f, "%d ", G.Adj[i][j].tempo);
	    fprintf (f, "\n");
	}
  fclose (f);
}

// Referencias: Canal no Youtube "Programação Descomplicada"