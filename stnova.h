/*****************************************************************/
/*   Estrutura nova a implementar | PROG2 | MIEEC | 2019/20   */      
/*****************************************************************/

#ifndef _STNOVA_H_
#define _STNOVA_H_

#include "tabdispersao.h"

/* podem criar mais struct que achem necessárias*/


typedef struct {
    /* quantidade de mensagens (enviadas e recebidas) que o remetente da heap tem com o destinatário em causa*/
	int quantidade;
    /* destinatário das mensagens enviadas pelo remetente contidas em *mensagens */
	char destinatario[TAMANHO_CHAVE];
    /* mensagens enviadas pelo remetente da heap para o destinatário em causa neste heap_elemento*/
    elemento *mensagens;
} heap_elemento;


typedef struct {
    /* remetente envolvido em todas as mensagens na heap correspondente*/
    char remetente[TAMANHO_CHAVE];  
	/* numero de nos da heap */
	int tamanho;
	/* numero maximo de nos da heap */
	int capacidade;
	/* vetor de apontadores para heap_elementos */
	heap_elemento** elementos;
} heap;

typedef struct st_elem
{
	/* apontador para a heap correspondente a cada remetente*/
	heap *hp;
	/* apontador para o proximo elemento */
	struct st_elem * proximo;
} st_elemento;

typedef struct
{
	/*tamanho da tabela de dispersao*/
    int tamanho;
	/*apontador para a lista de st_elemento contida em cada posicao da tabela de dispersao*/
    st_elemento **elems;
	/*hash da tabela de dispersao*/
    hash_func *hash;
} estrutura;

/* * * * * * * * * * * * * */

/* 
	*Cria nova heap ;
	*Parâmetro capacidade: nº de nós máximos que a heap poderá ter ;
	*Parâmetro remetente: remetente característico da heap em causa ;
	*Return: em caso de erro NULL, se não retorna o apontador para a heap criada ;
*/
heap* heap_nova(int capacidade,char* remetente);

/*
	*Cria novo heap_elemento referente às mensagens entre o remetente da heap e destinatario ;
	*Parâmetro destinatario: destinatario para o qual o novo heap_elemento se refere ;
	*Return: em caso de erro NULL, senão retorna o apontador para o no criado

heap_elemento* cria_novo_heap_elemento(char* destinatario);*/

/*
	*Insere elemento* com uma mensagem enviada pelo remetente da heap para o destinatario correspondente ;
	*Parâmetro no: elemento da heap onde será inserida em mensagens a mensagem ;
	*Parâmetro el: elemento* com as informações que serão inseridas num elemento de mensagens de no, exceto a componento el->proximo ;
	*Return: em caso de erro NULL, senão retorna o apontador para o elemento adicionado a mensagens de no ;
*/
elemento* insere_mensagem(heap_elemento* no,elemento* el);

/*
	*Apaga a heap 
*/
void heap_apaga(heap *h);

/*
	*Verifica se já existe algum nó referente àquele destinatário ;
	*Parâmetro h: heap onde se irá procurar o no correspondente a destinatario ;
	*Parâmetro destinatario: nome do destinatario das mensagens que estão no no procurado ;
	*Return: em caso de erro -1, caso não encontre 0, senao retorna o indíce do no pretendido ;
*/
int encontra_heap_no(heap* h,char* destinatario);

/* 
	*Insere el na heap ;
	*Parâmetro h: heap onde se vai inserir o no referente a destinatario ;
	*Parâmetro destinatario: destinatario referente ao no que será acrescentado a h ;
	*Parâmetro quantidade: número de mensagens trocadas entre o destinatario e o remetente da heap ;
	*Return: em caso de erro -1, de já existência do no 0, senão retorna o indice onde foi inserido;
*/
int heap_insere(heap *h,char* destinatario,int quantidade);

/*
	*Remove heap_elemento que se encontra na raiz da heap e reestrutura a heap ;
	*Parâmetro h: heap à qual irá ser retirado a raíz (heap_elemento com mais mensagens) ;
	*Return: em caso de erro NULL, senão apontador para a lista de mensagens contida na heap_elemento removido ; 
*/
elemento* heap_remove(heap *h);

/*
	compara o tamanho de mensagens de 2 nós para definir as posições na max heap
	caso e1->quantidade > e2->quantidade retorna 1, se nao retorna 0 
*/
int maior_que(heap_elemento * e1, heap_elemento * e2);


/*
	*Conta quantos destinatários receberam mensagens de remetente;
	*Parâmetro td: tabela de dispersão de onde a informação está a ser retirada ;
	*Parâmetro remetente: remetente para o qual se conta para quantos destinatários enviou mensagens ;
	*Retorno: em caso de erro -1, senão o nº de destinatários de mensagens de remetente ;
*/
int heap_capacidade(tabela_dispersao* td,char* remetente);

/* * * * * * * * * * * * * */


/**
 * cria e inicializa a estrutura criada
 */
estrutura* st_nova();



/**
 * Tenta inserir o elemento elem na estrutura st, garantindo acesso mais rápido aos elementos 
 * com maior número total de mensagens trocadas.
 * parametro st - apontador para a estrutura criada 
 * parametro elem - elemento que se pretende inserir na estrutura
 * Retorna 0 se bem-sucedida ou -1 em contrário.
 * */
int st_insere(tabela_dispersao *td, estrutura *st, elemento *elem);


/*
* Importa todo o conteúdo da tabela td para o novo formato de acesso. 
* parametro st - apontador para a estrutura criada 
* parametro td - apontador para a tabela de dispersao
* Retorna 0 se bem-sucedido ou -1 em contrário.
*/
int st_importa_tabela(estrutura *st, tabela_dispersao *td);


/*
* Extrai a instância do par remetente-destinatário com maior soma de mensagens enviadas e recebidas entre eles, 
* levando juntamente todas as mensagens enviadas. 
* parametro st - apontador para a estrutura criada 
* parametro remetente - apontador para o utilizador em que vai procurar o que tem mais ligacoes 
* Retorna apontador para o primeiro elemento de uma lista ligada de ‘elemento’,
* terminada com o campo ‘proximo’ do  último elemento apontando para NULL. Retorna NULL se não for encontrado ou em caso de erro.
* Obs : A instância retornada deverá ser consumida, i.e. não deverá ficar uma cópia dela em ‘st’. 
*/
elemento *st_remove(estrutura *st,char *remetente);

/*
*  Elimina todas as instâncias presentes na estrutura st e desaloca toda a memória da estrutura.
*  parametro st - apontador para a estrutura criada 
*  Retorna 0 se bem-sucedido e -1 se ocorrer algum erro.  
*/
int st_apaga(estrutura *st);

#endif