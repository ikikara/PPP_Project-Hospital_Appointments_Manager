#include <stdio.h>
#include <string.h>

#include "Links.h"

#define MAXNOMEMEDICO 100
#define MAXNOMEDOENTE 100
#define MAXNOMESAIDA 100

#define tamLinhas 100
#define tamstrings 31
#define tamNumero 13
#define NatributosMed 2
#define NatributosDoe 3

//Este programa tem como fundamento organizar as consultas num hospital num determinado dia sendo que um médico consegue
//ver 10 doentes de meia em meia hora entre as 10h e as 15h.

// --------------------------------------------------------1------------------------------------------------------------
//O programa começa por pedir ao utilizador o número de médicos e doentes que introduziu nos respetivos ficheiros, estes
//dados serão utilizados mais tarde para verificar se foram introduzidos a quantidade de médicos e doentes correta

// --------------------------------------------------------2------------------------------------------------------------
//De seguida é pedido o nome dos ficheiros de entrada dos médicos e dos doentes. Se estes ficheiros não existirem ou
//terem um nome demasiado grande ou não apresentarem extensão(.txt, .bin, ...) ou ainda não apresentarem a extensão
//será pedido de novo o nome dos ficheiros de entrada

// --------------------------------------------------------3------------------------------------------------------------
//Ir buscar o conteúdo do ficheiro e colocar em 2 arrays as entidades de cada linha. Um array terá os médicos e as suas
//caracteristicas e o outro array terá os doentes e as suas caracteristicas.
//Se os dados no ficheiros forem inválidos ou se a quantidade de médicos e doentes lidosnão for a correta o programa
//encerrará.

// --------------------------------------------------------4------------------------------------------------------------
//Ora a seguir estes dois array e os seus respetivos tamanhos(quantos medicos e quantos doentes) irão entrar numa função
//que irá organizar as consultas (função explicada em Funcoes.c). No caso de houver doentes que não forão atendidos o
//programa dará print de quem não foi atendido.

// --------------------------------------------------------5------------------------------------------------------------
//É feita impresso nos ficheiros o conteúdo pretendido (médicos, seus doentes e consultas num dos ficheiros e no outro
//ficheiro de saída os doentes e a informação da sua consulta).


int main() {
    printf("\nBem vindo ao Organizador de consultas! Antes de mais algumas instruções:\n");
    printf(" -> Os nomes no ficheiro devem ser escritos sem acentos, sem cedilhas, sem caracteres especiais e/ou sem números, o programa não está preparado para lidar com esses caracteres\n");
    printf(" -> O formato de cada linha no ficheiros do Médicos deverá ser (nome do medico)#(especialidade do médico)\n");
    printf(" -> O formato de cada linha no ficheiros do Doentes deverá ser (nome do doente)#(especialidade a ser atendido)#(grau de urgência de 1 a 4)\n");
    printf(" -> Se tentar alterar os ficheiros de entrada durante a execução poderão ocorrer erros, recomendasse que não o faça\n");
    printf("Certifique-se que segue à risca estas regras, se não fica sobre pena de o programa não correr da forma esperada\n\n");

    FILE * fichM = NULL;
    char nome_fichM[MAXNOMEMEDICO + 1];   //FICHEIRO ENTRADA MEDICOS
    char linhaM[tamLinhas];

    FILE * fichD = NULL;
    char nome_fichD[MAXNOMEDOENTE + 1];  //FICHEIRO ENTRADA DOENTES
    char linhaD[tamLinhas];


    char nMed[tamNumero];
    char nDoe[tamNumero];

// --------------------------------------------------------1------------------------------------------------------------
    while(isDigit(nMed)!=0 || (int)strlen(nMed)>tamNumero) {
        printf("Escreva apenas números neste campo. Indique quantos médicos inseriu: ");
        fgets(nMed, tamNumero, stdin);
    }


    while(isDigit(nDoe)!=0 || (int)strlen(nMed)>tamNumero) {
        printf("Escreva apenas números neste campo. Indique quantos doentes inseriu: ");
        fgets(nDoe, tamNumero, stdin);
    }

    int ncaracteresMe=50;
    int linhcorrMe=0;
    char Medicos[atOi(nMed)][NatributosMed][tamstrings];    //ARRAY DOS MEDICOS
    char elementsME[NatributosMed][tamstrings];

    int ncaracteresDo=50;
    int linhcorrDo=0;
    char Doentes[atOi(nDoe)][NatributosDoe][tamstrings];   //ARRAY DOS DOENTES
    char elementsDO[NatributosMed][tamstrings];

// --------------------------------------------------------2------------------------------------------------------------
    while(fichM == NULL) {
        printf("Nome do ficheiro de entrada dos Médicos? ");
        if (get_one_line(stdin,nome_fichM, MAXNOMEMEDICO + 1) == EOF )
            return 0;
        if (checkTXT(nome_fichM, ".txt")==0){
            fichM = fopen(nome_fichM, "r");
            if (fichM == NULL )
                printf("Ficheiro não existe ou nome do ficheiro apresenta espaços\n");
        }
        else{
            printf("Nome demasiado grande ou não apresenta extensão ou extensão inválida, verifique se a extensão do seu ficheiro é txt\n");
            continue;
        }
    }

    while(fichD == NULL) {
        printf("Nome do ficheiro de entrada dos Doentes? ");
        if (get_one_line(stdin,nome_fichD, MAXNOMEMEDICO + 1) == EOF )
            return 0;
        if (checkTXT(nome_fichD, ".txt")==0){
            fichD = fopen(nome_fichD, "r");
            if (fichD == NULL )
                printf("Ficheiro não existe ou nome do ficheiro apresenta espaços \n");
        }
        else{
            printf("Nome demasiado grande ou não apresenta extensão ou extensão inválida, verifique se a extensão do seu ficheiro é txt\n");
            continue;
        }
    }


// --------------------------------------------------------3------------------------------------------------------------
    while(fgets(linhaM, ncaracteresMe, fichM) != NULL) {
        if(atOi(nMed)<linhcorrMe){
            printf("ERRO: O número médicos que indicou é inferior ao número de médicos que está no ficheiro\nO programa será encerrado");
            return 0;
        }
        if (strcmp(linhaM, "\r\n") != 0){
            int i=0;
            split(linhaM, elementsME, NatributosMed);
            for(;i<NatributosMed;i++){
                strcpy(Medicos[linhcorrMe][i], elementsME[i]);
            }
        }
        else{
            continue;
        }
        linhcorrMe++;
    }
    if(atOi(nMed)!=linhcorrMe){
        printf("ERRO: O número médicos que indicou execede o número de médicos que está no ficheiro\nO programa será encerrado");
        return 0;
    }

    printf("\n");

    fclose(fichM);

    if(checkTEXTOeCONTEUDO(NatributosMed, Medicos, linhcorrMe)==1){
        printf("Dados introduzidos inválidos nos Médicos");
        return 0;
    }


    while(fgets(linhaD, ncaracteresDo, fichD) != NULL) {
        if(atOi(nDoe)<linhcorrDo){
            printf("ERRO: O número doentes que indicou é inferior ao número de doentes que está no ficheiro\nO programa será encerrado");
            return 0;
        }
        if (strcmp(linhaD, "\r\n") != 0){
            int i=0;
            split(linhaD, elementsDO, NatributosDoe);
            for(;i<NatributosDoe;i++){
                strcpy(Doentes[linhcorrDo][i], elementsDO[i]);
            }
        }
        else{
            continue;
        }
        linhcorrDo++;

    }
    if(atOi(nDoe)!=linhcorrDo){
        printf("ERRO: O número doentes que indicou excede o número de doentes que está no ficheiro\nO programa será encerrado");
        return 0;
    }

    fclose(fichD);

    if(checkTEXTOeCONTEUDO(NatributosDoe, Doentes, linhcorrDo)==1 ){
        printf("Dados introduzidos inválidos nos Doentes");
        return 0;
    }

// --------------------------------------------------------4------------------------------------------------------------

    OrganizarConsultas(Medicos,Doentes, linhcorrMe, linhcorrDo);

// --------------------------------------------------------5------------------------------------------------------------
    Imprimir();

    printf("\nObrigado por usar o Organizador de consultas!\nAté à próxima!\n");

    return 0;
}

