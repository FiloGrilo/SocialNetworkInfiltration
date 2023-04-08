/*****************************************************************/
/*   Grafo direcionado | PROG2 | MIEEC | 2019/20        */      
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"


grafo* grafo_novo()
{
   grafo* g=(grafo*)malloc(sizeof(grafo));
   if (g==NULL) return NULL;
   g->tamanho=0;

   g->nos=(no_grafo**)malloc(sizeof(no_grafo *));
   if(g->nos==NULL) return NULL; 

   return g;

}


void grafo_apaga(grafo* g)
{
    if(g==NULL){
        return;
    }
    else
    {
        int i;
        for (i = 0; i < g->tamanho; i++)
        {
            if (g->nos[i]!=NULL)
            {
                int j;
                for ( j = 0; j < g->nos[i]->tamanho; j++)
                {
                    free(g->nos[i]->ligacoes[j]);
                }
                free(g->nos[i]->ligacoes);
                free(g->nos[i]->nome_user);
                free(g->nos[i]);
            }
        }
    }
    free(g->nos);
    free(g);
}


no_grafo * no_insere(grafo *g, char *user)
{
    if (g == NULL || user == NULL || encontra_no(g, user) != NULL)
    {
        return NULL;
    }
    g->nos = (no_grafo**) realloc (g->nos, sizeof(no_grafo*) * (g->tamanho+1));
    if (g->nos == NULL)
    {
        return NULL;
    }

    no_grafo *aux = (no_grafo*)malloc(sizeof(no_grafo));
    if (aux == NULL)
    {
        return NULL;
    }
    aux->tamanho = 0;
    aux->nome_user = (char *) malloc (strlen(user)+1);
    if (aux->nome_user == NULL)
    {
        return NULL;
    }
    strcpy(aux->nome_user, user);
    aux->ligacoes = (ligacao **) malloc (sizeof(ligacao*));
    if (aux->ligacoes == NULL)
    {
        return NULL;
    }

    g->nos[g->tamanho] = aux;

    g->tamanho++;
    return g->nos[g->tamanho-1];
   
}

int  cria_ligacao(no_grafo *origem, no_grafo *destino, int peso)
{   
    if (origem==NULL || destino==NULL || peso<=0) return -1;
    for(int i=0;i<origem->tamanho;i++){
       if(origem->ligacoes[i]->destino==destino) return -1; /*a ligação já existe*/
    }
    /*se chega aqui é porque a ligação ainda não existe e temos de a inserir no fim do vetor ligações*/
    
    origem->ligacoes=(ligacao**)realloc(origem->ligacoes,sizeof(ligacao*)*(origem->tamanho+1));
    if (origem->ligacoes==NULL) return -1 ;
    ligacao *aux = (ligacao*)malloc(sizeof(ligacao));
    if(aux==NULL) return -1;
    aux->destino=destino;
    aux->peso_ligacao=peso;
    origem->ligacoes[origem->tamanho]=aux;
    origem->tamanho++;
    
    return 0;
}


no_grafo * encontra_no(grafo *g, char *nomeU)
{
    if (g==NULL || nomeU==NULL) return NULL;

    for(int i=0;i<g->tamanho;i++){
        if(strcmp(g->nos[i]->nome_user,nomeU)==0) return g->nos[i];
    }
    /*se chegar aqui é porque não encontrou o nó com o nome pretendido*/
    return NULL;
}



grafo * criaGrafo(tabela_dispersao *td)
{
    if(td==NULL) return NULL;
    grafo* g=grafo_novo();
    if (g==NULL) return NULL ;
    elemento* elem;
    no_grafo* no1,*no2;
    int nMsg[2]={0};
    
    for(int i=0;i<td->tamanho;i++){
        elem=td->elementos[i];
        while(elem!=NULL){
            no1=no_insere(g,elem->msg->remetente);
            no2=no_insere(g,elem->msg->destinatario);
            /*considerando que não há erros ao executar a função no_insere*/
            if(no1==NULL) no1=encontra_no(g,elem->msg->remetente); /*caso no1 já existisse*/
            if(no2==NULL) no2=encontra_no(g,elem->msg->destinatario); /*caso no2 já existisse*/
            /*agora é preciso saber quantas mensagens existem entre eles*/
            ligacao2(td,elem->msg->remetente,elem->msg->destinatario,nMsg);
            cria_ligacao(no1,no2,nMsg[0]);
            cria_ligacao(no2,no1,nMsg[1]);
            elem=elem->proximo;
        }
    }
    
    return g;
}



no_grafo **lista_amigos(grafo *g, char *nomeU, int *n)
{   
    if (g==NULL || nomeU==NULL) return NULL;

    int count=0;

    no_grafo* no1=encontra_no(g,nomeU),*no2;
    if (no1==NULL) return NULL;

    no_grafo** vetor=(no_grafo**)malloc(sizeof(no_grafo*));   
    if(vetor==NULL) return NULL ;

    for(int i=0;i<no1->tamanho;i++){
       if(no1->ligacoes[i]->peso_ligacao>=4){
           no2=no1->ligacoes[i]->destino;
           for(int j=0;j<no2->tamanho;j++){
               if(no2->ligacoes[j]->destino==no1 && no2->ligacoes[j]->peso_ligacao>=4){
                   vetor=(no_grafo**)realloc(vetor,sizeof(no_grafo*)*(count+1));
                   if(vetor==NULL) return NULL;
                   vetor[count]=no2;
                   count++;
                    break;
               }
           }
       }
   }
   *n=count ;
   /*se nao se encontrou nenhum amigo liberta-se o vetor porque está vazio */
   if(count==0) {
       free(vetor);
       return NULL;
   }
   return vetor;


}



no_grafo ** identifica_ciclo(grafo *g, char *nomeU, int M, int *n)
{
    if(g== NULL || nomeU==NULL || M<3) return NULL ;
    static int count = 1;
    static char nobase[100];
    no_grafo* aux=encontra_no(g,nomeU);
    if(aux==NULL) return NULL;
    no_grafo** vetor; 
    /*nobase terá o nome do remetente onde inicia o ciclo*/
    if(count==1) strcpy(nobase,nomeU);
    /*se count <=2 entao ainda está num dos dois primeiros elementos do ciclo (a contar com o inicio) e não pode procurar o no inicial*/
    if(count<=2){
        for(int i=0;i<aux->tamanho;i++){
            if(strcmp(aux->ligacoes[i]->destino->nome_user,nobase)==0) continue;
            count++;
            vetor=identifica_ciclo(g,aux->ligacoes[i]->destino->nome_user,M,&(*n));
            count--;
            if(vetor!=NULL){
                vetor[count-1]=aux;
                return vetor;
            }
        }
        return NULL;

    }
    /*se 2<count<=M entao pode tentar encontrar o no inicial (nobase) nos destinos das ligacoes do no que está a ser "avaliado"*/
    if(count>2 && count<=M){
        for(int i=0;i<aux->tamanho;i++){
            if(strcmp(aux->ligacoes[i]->destino->nome_user,nobase)==0){
                *n=count;
                vetor=calloc(count,sizeof(no_grafo*));
                if(vetor==NULL) return NULL;
                vetor[count-1]=aux;
                return vetor;
            }
            else{
                count++;
                vetor=identifica_ciclo(g,aux->ligacoes[i]->destino->nome_user,M,&(*n));
                count--;
                if(vetor!=NULL){
                    vetor[count-1]=aux;
                    return vetor;
                }
            }
        }
        return NULL;
    }
    /*sendo count >M então aquele "caminho" não poderá ser*/
    else return NULL;
}