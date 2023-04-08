/*****************************************************************/
/*   Estrutura nova a implementar | PROG2 | MIEEC | 2019/20   */      
/*****************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stnova.h"

#define RAIZ 		(1)
#define PAI(x) 		(x/2)
#define FILHO_ESQ(x) 	(x*2)
#define FILHO_DIR(x) 	(x*2+1)



estrutura* st_nova()
{
    
    estrutura *td = (estrutura *)malloc(sizeof(estrutura));
    if (td == NULL)
    {
        return NULL;
    }

    td->tamanho=0;
    td->hash = NULL;
    return td;
}




int st_insere(tabela_dispersao *td, estrutura *st, elemento *elem)
{
    if (st == NULL || elem == NULL || td==NULL)
    {
        return -1;
    }
   
    int i = st->hash(elem->msg->remetente, st->tamanho);
    int tot[2];
    ligacao2(td, elem->msg->remetente, elem->msg->destinatario, tot);
    int qnt ;
    if(tot[1]==-1) qnt=tot[0];
    else qnt = tot[0]+tot[1];
    st_elemento *aux;
    aux = st->elems[i];

    /*procurar a heap correspondente ao remetente que enviou a mensagem contida em elem*/
    while(aux != NULL)
    {
        /*se encontrar a heap, então irá inserir o elem num nó dessa heap*/
        if (strcmp(elem->msg->remetente, aux->hp->remetente) == 0)
        {
            int a = heap_insere(aux->hp, elem->msg->destinatario, qnt);
            int pos;
            switch (a)
            {
            case -1:/*houve erro na inserção da heap*/
                return -1;
                break;
            case 0: /*no ja existia*/
                pos = encontra_heap_no(aux->hp, elem->msg->destinatario);
                if (pos==-1) return -1;
                if  (insere_mensagem(aux->hp->elementos[pos], elem) == NULL) return -1;
                return 0;
                break;
            default:/*no foi criado*/
                if  (insere_mensagem(aux->hp->elementos[a], elem) == NULL) return -1;
                return 0;
                break;
            }            
        }
        aux = aux->proximo;
    }
    /*se chegar aqui é porque a heap referente àquele remetente não existia e terá de ser criada*/

    int cap=heap_capacidade(td, elem->msg->remetente);
    if(cap==-1) return -1;
    st_elemento *el=(st_elemento*)calloc(1, sizeof(st_elemento));
    if(el==NULL) return -1;
    heap *hp_nova = heap_nova(cap, elem->msg->remetente);
    if(hp_nova == NULL){
        free(el);
        return -1;
    }


    el->hp = hp_nova;

    /*se não existia nenhuma heap nesta posição da td , então a nova heap será , por agora, o unico elemento naquela posição*/
    if (st->elems[i] == NULL)
    {
        el->proximo = NULL;
        st->elems[i] = el;
    }
    /*se já existiam mais heaps naquela posição da td, a nova heap irá para primeira posição da lista daquela posição da td*/
    else
    {
        aux = st->elems[i];
        el->proximo = aux;
        st->elems[i] = el;
    }
    /*inserir o nó correspondente ao destinatário da mensagem em elem na heap acabada de criar*/
    int b = heap_insere(el->hp, elem->msg->destinatario, qnt);
    if (b == -1)
    {
        return -1;
    }
    /*se b!=-1 então b!=0 , pois a heap foi criada , por isso, é impossivel ja haver algum nó criado na heap*/
    if  (insere_mensagem(el->hp->elementos[b], elem) == NULL){
        return -1;
    }
    //printf("\ninsere10: %d\n",b);

    return 0;
}


int st_importa_tabela(estrutura *st, tabela_dispersao *td)
{
    if(st==NULL || td==NULL) return -1;
    st->tamanho=td->tamanho;
    st->elems=(st_elemento**)calloc(st->tamanho,sizeof(st_elemento*));
    st->hash=td->hfunc;
    elemento* aux;
    for(int i=0;i<td->tamanho;i++){
        aux=td->elementos[i];
        while(aux!=NULL){
            if(st_insere(td,st,aux)==-1)   return -1;
            aux=aux->proximo;
        }
    }


    return 0;    

}

elemento *st_remove(estrutura *st,char *remetente)
{
    if(st==NULL || remetente==NULL) return NULL;


    int i = st->hash(remetente, st->tamanho);
    st_elemento *aux = st->elems[i];
    while (aux != NULL)
    {
        if (strcmp(aux->hp->remetente, remetente)==0)
        {
            elemento *remover = heap_remove(aux->hp);
            if (remover == NULL) return NULL;
            return remover; 
        }
        aux = aux->proximo;
    }
    /* caso chegue aqui, nao foi encontrada nenhuma heap correspondente ao remetente */
    return NULL;
}


int st_apaga(estrutura *st)
{
    if (st == NULL)
    {
        return -1;
    }
    st_elemento *h,*aux ;
    for (int i = 0; i < st->tamanho; i++)
    {
        h = st->elems[i];
        while (h!=NULL)
        {
            heap_apaga(h->hp);
            aux=h->proximo;
            free(h);
            h=aux;
        }
        st->elems[i]=NULL;
    }
    free(st->elems);
    free(st);
    return 0;
}


/*********************/

heap* heap_nova(int capacidade,char* remetente){

    if(capacidade==0 || remetente==NULL) return NULL;

    heap * h = (heap *) malloc(sizeof(heap));

	if (!h) return NULL;

	h->tamanho = 0;
	h->capacidade = capacidade;
	h->elementos = (heap_elemento **) calloc(capacidade+1, sizeof(heap_elemento *));
    strcpy(h->remetente,remetente);

	if (!h->elementos) {
		free(h);
		return NULL;
	}

	return h;
}


elemento* insere_mensagem(heap_elemento* no,elemento* el){

    if(no==NULL || el==NULL) return NULL;

    elemento* elem = (elemento*)calloc(1, sizeof (elemento)),*aux;
    if (elem == NULL) return NULL;

    /* aloca memoria para o mensagem*/
    elem->msg = (mensagem*) malloc(sizeof (mensagem));
    if (elem->msg == NULL)
    {   
        free(elem);
        return NULL;
    }
    /* aloca memoria para o texto da mensagem*/
    elem->msg->texto = (char*)malloc(strlen(el->msg->texto)+1);
    if (elem->msg->texto == NULL)
    {   
        free(elem->msg);
        free(elem);
        return NULL;
    }
  
    strcpy(elem->msg->remetente, el->msg->remetente);
    strcpy(elem->msg->destinatario, el->msg->destinatario);
    strcpy(elem->msg->texto,el->msg->texto);

    elem->proximo=NULL;
    if(no->mensagens==NULL){
        no->mensagens=elem;
        return elem;
    }
    aux=no->mensagens;
    while(aux->proximo!=NULL) aux=aux->proximo;

    aux->proximo=elem;
    return elem;

}

void heap_apaga(heap *h)
{
	int i,tam=h->tamanho;
    if(h==NULL) return;
    /* apaga todos os elementos e respetivas strings */
    elemento *el, *aux;
    for(i=RAIZ; i<=tam; i++)
    {
        el = heap_remove(h);
        while (el!=NULL)
        {
            aux = el->proximo;
            free(el->msg->texto);
            free(el->msg);
            free(el);
            el=aux;
        }
    }
    free(h->elementos);
    free(h);
}

int maior_que(heap_elemento * e1, heap_elemento * e2){
    
    if (e1 == NULL || e2 == NULL) return 0;

	return e1->quantidade > e2->quantidade;
}

int encontra_heap_no(heap* h,char* destinatario){

    if (h==NULL || destinatario==NULL) return -1 ;

    for(int i=1;i<=h->tamanho;i++){
        if(strcmp(h->elementos[i]->destinatario,destinatario)==0) return i ;
    }

    return 0;

}

int heap_insere(heap *h, char* destinatario,int quantidade)
{
    //printf("\nentrou heap_insere : %d %d\n",h->tamanho,h->capacidade);

    if (h==NULL || destinatario==NULL || encontra_heap_no(h,destinatario)==-1 || h->tamanho>h->capacidade) return -1;

    /*caso o nó correspondente àquele destinatario já exista*/
    if( encontra_heap_no(h,destinatario)!=0 ) return 0;
    
    int i;

    heap_elemento* el = (heap_elemento*)calloc(1, sizeof (heap_elemento)),*aux;
    if (el==NULL) return -1;

    strcpy(el->destinatario,destinatario);
    el->quantidade=quantidade;
    /* coloca elemento no fim da heap */
	h->tamanho++;
	i = h->tamanho;
	h->elementos[i] = el;

 	/* insercao do novo no na posicao adequada */
	while (i != RAIZ && maior_que(h->elementos[i], h->elementos[PAI(i)]))
	{
		aux = h->elementos[PAI(i)];
		h->elementos[PAI(i)] = h->elementos[i];
		h->elementos[i] = aux;
		i = PAI(i);
	}
	return i;

}
int heap_capacidade(tabela_dispersao* td,char* remetente){
    if(td==NULL || remetente==NULL) return -1;

    int total=tabela_existe(td,remetente),totaldestinatarios=0,verif;
    if (total==-1 || total==0) return total;

    mensagem** vetor=tabela_listagem(td,remetente);
    if (vetor==NULL) return -1;

    /*considerando que no maximo cada nome tem no maximo 20 caracteres*/
    char** vetornomes=(char**)malloc(20);

    for(int count=0;vetor[count]!=NULL;count++){
        verif=0;
        for(int h=0;h<totaldestinatarios;h++){
            if(strcmp(vetor[count]->destinatario,vetornomes[h])==0){
                verif=1;
                break;
            }
        }
        if(verif==0){
            totaldestinatarios++;
            /*"abrir" espaço para mais um nome a guardar em vetornomes, considerando que, no maximo, cada nome tem 20 caracteres*/
            vetornomes=(char**)realloc(vetornomes,20*totaldestinatarios);
            if(vetornomes==NULL) return -1;
            /*requesitar o espaço em vetornomes[totaldestinatarios-1] que será realmente utilizado*/
            vetornomes[totaldestinatarios-1]= (char *) malloc(strlen(vetor[count]->destinatario)+1);
            strcpy(vetornomes[totaldestinatarios-1],vetor[count]->destinatario);
        }
    }

    for(int h=0;h<totaldestinatarios;h++){
        free(vetornomes[h]);
    }
    free(vetornomes);
    free(vetor);
    return totaldestinatarios;

}

elemento* heap_remove(heap *h){
    int i, filho_maior;
	heap_elemento * aux;
	elemento* el;

	/* se heap estiver vazia, nao remove elemento */
	if (h==NULL || h->tamanho <= 0) return NULL;

	el = h->elementos[RAIZ]->mensagens;
	free(h->elementos[RAIZ]);

	/* coloca ultimo elemento da heap na raiz */
	h->elementos[RAIZ] = h->elementos[h->tamanho];
	h->elementos[h->tamanho] = NULL;
	h->tamanho--;

 	i = RAIZ;

	/* reestrutracao da heap */
	while(FILHO_ESQ(i) <= h->tamanho)
	{
		filho_maior = FILHO_ESQ(i);
        
        /*caso o filho direito seja maior que o filho esquerda, filho_maior passara a ser o filho direito*/
		if (FILHO_DIR(i) <= h->tamanho && maior_que(h->elementos[FILHO_DIR(i)], h->elementos[FILHO_ESQ(i)]))
			filho_maior = FILHO_DIR(i);

		/* caso o filho maior seja maior que o pai, troca-os */
		if (maior_que(h->elementos[filho_maior], h->elementos[i]))
		{
			aux = h->elementos[filho_maior];
			h->elementos[filho_maior] = h->elementos[i];
			h->elementos[i] = aux;
			i = filho_maior;
		}
		else break;
	}

	return el;
}
/********************/
