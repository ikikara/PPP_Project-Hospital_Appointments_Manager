//
// Created by Ikiru on 5/13/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "StructsListasLigadas.c"

#define MAXNOMESAIDA 100

void colocarMe(char medi[31], char doent[][2][31],  char especiali[31], double hora, int Me);
void colocarDo(char doent[31], char medi[31],  char especiali[31], int horah, int horam, int grau);
void OrdenaDoentes(char Doents[][3][31], char DoentsF[][3][31], int linhaD);
void RetirarDoentes(char doents[][3][31], int index);
int checkTXT(char string[], char extencao[]);
int strcomp(char string1[31], const char string2[31]);
int atOi(const char numero[12]);
int isDigit(const char num[]);
int isString( char palavra[]);
int isSpace(char c);
int isCntrl(char c);
int get_one_line(FILE * fich, char *linha, int lim);

FILE * fich2 = NULL;
char nome_fich2[MAXNOMESAIDA + 1];   //FICHEIRO SAIDA CONSULTAS

FILE * fich3 = NULL;
char nome_fich3[MAXNOMESAIDA + 1];   //FICHEIRO SAIDA DOENTES E SUAS MARCAÇOES

static p_medico listaMe;
static p_doente listaDo;

p_medico LM;
p_doente LD;

//Função que como o nome indica vai organizar as consultas e dar informações dos doentes
//Parâmetros:
//1. Array dos Medicos e suas caracteristicas
//2. Array dos Doentes e suas caracteristicas
//3. Quantidade de Médicos
//4. Quantidade de Doentes
void OrganizarConsultas(char medicos[][2][31], char doentes[][3][31], int linhaM, int linhaD) {
    char doents[linhaD][3][31];
    char ListaAUX[linhaD][2][31];
    OrdenaDoentes(doentes, doents, linhaD);
    int D = linhaD;

    double hora = 10;


    for (int i = 0; i < linhaM; i++) {
        if (D == 0 || hora == 15) {
            if (hora == 15) {
                printf("Sem mais horas disponiveis hoje\n");
            }
            break;
        }
        int Md = 0;
        for (int j = 0; j < linhaD; j++) {
            if (Md % 10 == 0 && Md != 0) {
                break;
            }
            if (strcomp(medicos[i][1], doents[j][1]) == 0) {
                strcpy(ListaAUX[Md][0], doents[j][0]);
                strcpy(ListaAUX[Md++][1], doents[j][2]);
                RetirarDoentes(doents, j);
                D--;
            }
        }
        colocarMe(medicos[i][0], ListaAUX, medicos[i][1], hora, Md);
        if (i == linhaM - 1 && D > 0) {
            i = 0;
            hora += 0.5;
        }
    }


    if (D > 0) {
       printf("Os seguintes doentes não podem ser atendidos no dia de hoje devido à falta de médicos para tal,\n");
        for (int l = 0; l < linhaD; l++) {
            if (strcmp(doents[l][0], "\0") != 0) {
                printf("%s \n", doents[l][0]);
            }
        }
    }
    else {
        printf("Todos os doentes foram atendidos\n");
    }
}


//Função para colocar na lista ligada dos Médicos todas as suas informações
//Parâmetros:
//1. Nome do Médico
//2. Array dos Doentes(e suas caracteristicas) que o médico vai atender numa certa hora
//3. Especialidade do Médico
//4. Hora que o Médico vai atender os doentes
//5. Número de Doentes que o Médico vai atender
void colocarMe(char medi[31], char doent[][2][31], char especiali[31], double hora, int Me){
    p_medico novo = NULL, l = NULL; //todos os indices e numeros neste momento nao existem na lista ligada

    if(listaMe == NULL) { //se a lista estiver vazia
        novo = malloc(sizeof(struct  SMedicos)); //alocar espaço suficiente
        strcpy(novo->medico, medi);  //guarda em novo o numero
        strcpy(novo->especialidade1, especiali);
        novo->hora[0]=(int)hora;
        if(hora-0.5==(int)hora){
            novo->hora[1]=30;
        }
        else{
            novo->hora[1]=0;
        }

        for(int M=0; M<Me; M++) {
            strcpy(novo->MDoentes[M], doent[M][0]);
            colocarDo(doent[M][0], medi, especiali, novo->hora[0],novo->hora[1], atOi(doent[M][1]));
            if(M!=0 && (M+1)%10==0){
                hora+=0.5;
            }
        }
        novo->NDoentes=Me;
        novo->nextM = NULL;
        listaMe = novo;
    }

    else{
        l = listaMe;
        novo = malloc(sizeof(struct SMedicos));
        strcpy(novo->medico, medi);
        strcpy(novo->especialidade1, especiali);
        novo->hora[0]=(int)hora;
        if(hora-0.5==(int)hora){
            novo->hora[1]=30;
        }
        else{
            novo->hora[1]=0;
        }

        for(int N=0; N<Me; N++) {
            strcpy(novo->MDoentes[N], doent[N][0]);
            colocarDo(doent[N][0], medi, especiali, novo->hora[0],novo->hora[1], atOi(doent[N][1]));
            if(N!=0 && (N+1)%10==0){
                hora+=0.5;
            }

        }
        novo->NDoentes=Me;
        novo->nextM = NULL;

        while(l->nextM!=NULL)
            l = l->nextM;

        l->nextM = novo;
    }
}

//Função para colocar na lista ligada dos Doentes todas as suas informações
//Parâmetros:
//1. Nome do Doente
//2. Nome do Médico que o vai atender
//3. Especialidade a em que o Doente vai ser atendido
//4. Parte das "Horas"       <-- Horas que o doente é atendido
//5. Parte dos "Minutos"
//6. Grau de urgência dos Doentes
void colocarDo(char doent[31], char medi[31],  char especiali[31], int horah, int horam, int grau){
    static p_doente novoD = NULL, ld = NULL;

    if(listaDo == NULL){
        novoD = malloc(sizeof(struct SDoentes));
        strcpy(novoD->doente, doent);
        strcpy(novoD->medicoD, medi);
        strcpy(novoD->especialidade2, especiali);
        novoD->horaD[0]=horah;
        novoD->horaD[1]=horam;
        novoD->grau=grau;
        novoD->nextD = NULL;
        listaDo = novoD;
    }

    else{
        ld = listaDo;
        novoD = malloc(sizeof(struct SDoentes));
        strcpy(novoD->doente, doent);
        strcpy(novoD->medicoD, medi);
        strcpy(novoD->especialidade2, especiali);
        novoD->horaD[0]=horah;
        novoD->horaD[1]=horam;
        novoD->grau=grau;
        novoD->nextD = NULL;

        while(ld->nextD!=NULL)
            ld = ld->nextD;

        ld->nextD = novoD;
    }
}

//Função que apenas serve para imprimir o conteudo das listas ligadas(as consultas para o 1º ficheiro  e os doentes e as
//suas marcações no 2º ficheiro)
//Parâmetros - Não apresenta
void Imprimir(){
    while(fich2==NULL) {
        printf("\nNome do ficheiro de saída das Consultas? ");
        get_one_line(stdin, nome_fich2, MAXNOMESAIDA + 1);
        if (checkTXT(nome_fich2, ".txt") == 0) {
            fich2 = fopen(nome_fich2, "w");
        }
        else {
            printf("Não aprensenta extensão ou extensão inválida\n");
        }
    }

    fprintf(fich2, "============================================ CONSULTAS ============================================\n\n");
    while(listaMe->nextM!=NULL){
        if(strcmp(listaMe->MDoentes[0],"\0")!=0){
            fprintf(fich2,"%s --> %s\n", listaMe->medico, listaMe->especialidade1);
            fprintf(fich2,"\n");
            int z;
            for(z=0; z<listaMe->NDoentes; z++) {
                if (listaMe->hora[1] == 0) {
                    fprintf(fich2, "%s\t%dh00\n", listaMe->MDoentes[z], listaMe->hora[0]);
                } else {
                    fprintf(fich2, "%s\t%dh%d\n", listaMe->MDoentes[z], listaMe->hora[0], listaMe->hora[1]);
                }
            }
            fprintf(fich2,"\n");
        }

        LM=listaMe;
        listaMe=listaMe->nextM;
        free(LM);
    }
    if(strcmp(listaMe->MDoentes[0],"\0")!=0) {
        fprintf(fich2, "%s --> %s\n", listaMe->medico, listaMe->especialidade1);
        fprintf(fich2, "\n");
        int z;
        for (z = 0; z < listaMe->NDoentes; z++) {
            if (listaMe->hora[1] == 0) {
                fprintf(fich2, "%s\t%dh00\n", listaMe->MDoentes[z], listaMe->hora[0]);
            } else {
                fprintf(fich2, "%s\t%dh%d\n", listaMe->MDoentes[z], listaMe->hora[0], listaMe->hora[1]);
            }
        }
        fprintf(fich2, "\n");
    }
    LM=listaMe;
    free(LM);
    fclose(fich2);

    while(fich3==NULL) {
        printf("Nome do ficheiro de saída dos Doentes e suas Marcações? ");
        get_one_line(stdin, nome_fich3, MAXNOMESAIDA + 1);
        if(checkTXT(nome_fich3, ".txt")==0){
            if(strcmp(nome_fich2, nome_fich3)==0){
                printf("Escolha um nome que não seja igual ao do 1º ficheiro de saída\n");
            }
            else{
                fich3 = fopen(nome_fich3, "w");
            }
        }
        else {
            printf("Não aprensenta extensão ou extensão inválida\n");
        }
    }

    fprintf(fich3, "===================================== DOENTES E SUAS MARCAÇOES =====================================\n\n");

    while(listaDo->nextD!=NULL){
        if(listaDo->horaD[1] == 0){
            fprintf(fich3, "%s\t%s\t%dh00\t%d\t%s\n", listaDo->doente, listaDo->medicoD,  listaDo->horaD[0], listaDo->grau, listaDo->especialidade2);
        }
        else {
            fprintf(fich3, "%s\t%s\t%dh%d\t%d\t%s\n", listaDo->doente,  listaDo->medicoD, listaDo->horaD[0], listaDo->horaD[1], listaDo->grau, listaDo->especialidade2);
        }
        LD=listaDo;
        listaDo=listaDo->nextD;
        free(LD);
    }

    if(listaDo->horaD[1] == 0){
        fprintf(fich3, "%s\t%s\t%dh00\t%d\t%s\n", listaDo->doente, listaDo->medicoD,  listaDo->horaD[0], listaDo->grau, listaDo->especialidade2);
    }
    else {
        fprintf(fich3, "%s\t%s\t%dh%d\t%d\t%s\n", listaDo->doente,  listaDo->medicoD, listaDo->horaD[0], listaDo->horaD[1], listaDo->grau, listaDo->especialidade2);
    }
    LD=listaDo;
    free(LD);
    fclose(fich3);
}

//Função que segue para ordenar os Doentes por grau de urgência
//Parâmetros:
//1. Array inicial dos Doentes
//2. Array que sairá ordenado
//3. Número de Doentes
void OrdenaDoentes(char Doents[][3][31], char DoentsF[][3][31], int linhaD) {
    char esp4[linhaD][3][31];
    char esp3[linhaD][3][31];
    char esp2[linhaD][3][31];
    char esp1[linhaD][3][31];

    int e4=0, e3=0, e2=0, e1=0;
    for (int i = 0; i < linhaD; i++) {
        if (strcomp(Doents[i][2], "4") == 0) {
            strcpy(esp4[e4][0], Doents[i][0]);
            strcpy(esp4[e4][1], Doents[i][1]);
            strcpy(esp4[e4][2], Doents[i][2]);
            e4++;

        }
        if (strcomp(Doents[i][2], "3") == 0) {
            strcpy(esp3[e3][0], Doents[i][0]);
            strcpy(esp3[e3][1], Doents[i][1]);
            strcpy(esp3[e3][2], Doents[i][2]);
            e3++;
        }
        if (strcomp(Doents[i][2], "2") == 0) {
            strcpy(esp2[e2][0], Doents[i][0]);
            strcpy(esp2[e2][1], Doents[i][1]);
            strcpy(esp2[e2][2], Doents[i][2]);
            e2++;
        }

        if (strcomp(Doents[i][2], "1") == 0) {
            strcpy(esp1[e1][0], Doents[i][0]);
            strcpy(esp1[e1][1], Doents[i][1]);
            strcpy(esp1[e1][2], Doents[i][2]);
            e1++;
        }
    }

    int count=0;
    for(int j=0; j < e4; j++) {
        strcpy(DoentsF[count][0], esp4[j][0]);
        strcpy(DoentsF[count][1], esp4[j][1]);
        strcpy(DoentsF[count][2], esp4[j][2]);
        count++;
    }
    for (int j = 0; j < e3; j++) {
        strcpy(DoentsF[count][0], esp3[j][0]);
        strcpy(DoentsF[count][1], esp3[j][1]);
        strcpy(DoentsF[count][2], esp3[j][2]);
        count++;
    }
    for (int j = 0; j < e2; j++) {
        strcpy(DoentsF[count][0], esp2[j][0]);
        strcpy(DoentsF[count][1], esp2[j][1]);
        strcpy(DoentsF[count][2], esp2[j][2]);
        count++;
    }
    for (int j = 0; j < e1; j++) {
        strcpy(DoentsF[count][0], esp1[j][0]);
        strcpy(DoentsF[count][1], esp1[j][1]);
        strcpy(DoentsF[count][2], esp1[j][2]);
        count++;
    }
}

//Função que servirá para "retirar" o Doente se ele for atendido
//Parâmetros:
//1. Array dos Doentes
//2. Posição do Doente no Array
void RetirarDoentes(char doents[][3][31], int index){

    strcpy(doents[index][0], "\0");
    strcpy(doents[index][1], "\0");
    strcpy(doents[index][2], "\0");

}

//Função para dar check na extenção do ficheiro introduzido
//Parâmetros:
//1. Nome do Ficheiro
//2. Extensão que se pretende verificar(neste caso TXT)
int checkTXT(char string[], char extencao[]){
    int j=0;
    int check=0;

    for(int i=(int)strlen(string)-4; i<(int)strlen(string); i++){
        if(string[i]==extencao[j]){
            check++;
        }
        j++;
    }

    if(check==(int)strlen(extencao)){
        return 0;
    }
    else{
        return 1;
    }
}

//Função para verificar se o conteúdo no ficheiro é válido
//Parâmetros:
//1. Número de entidades por linha(o ficheiro dos Médicos tem 2 e o ficheiro dos Doentes tem 3)
//2. Array dos Médicos ou Doentes(depende do ficheiro que se tiver a verificar)
//3. Número de Médicos ou Doentes(depende do ficheiro que se tiver a verificar)
int checkTEXTOeCONTEUDO(int atributos, char Array[][atributos][31], int nPessoas) {
    for(int i=0; i<nPessoas; i++){
        for(int j=0; j<atributos; j++) {
            if (j<2) {
                if (isString(Array[i][j]) == 1) {
                    return 1;
                }
            }
            else{
                if (isDigit(Array[i][j])==1 || (isDigit(Array[i][j])==0 && (atOi(Array[i][j])>4 || atOi(Array[i][j])<1)) || Array[i][j][0]==13){
                    return 1;
                }
            }
        }
    }
    return 0;
}

//Função para separar as entidades do Médicos ou dos Doentes(que estão separadas por #)
//Parâmetros:
//1. Linha que leva "split"
//2. Array para onde é guardadas as entidades
//3. Número de entidades que o array terá(os Médicos terão 2 e o Doentes terão 3)
void split(char *linha, char elemts[][31], int nEle){
    int j=0;

    for(int i=0; i<nEle;i++){
        char temp[31]="\0";
        int n=0;
        for(;j<(int)strlen(linha); j++){
            if(linha[j]=='#' ||  linha[j] =='\n'){
                j++;
                break;
            }

            temp[n]=linha[j];
            n++;
        }
        strcpy(elemts[i], temp);
    }
}

//Função para comparar strings(para evitar certos erros e warnings)
//Parâmetros:
//1. 1ª string a ser usada na comparação
//2. 2ª string a ser usada na comparação
int strcomp(char string1[31], const char string2[31]){
    int lim=1;

    for(int i=0; i<lim; i++){
        if(string1[i]==string2[i])    {
            lim++;
        }
    }
    if(lim==(int)strlen(string1)){
        return 0;
    }
    else{
        return 1;
    }
}

//Função para transformar uma string que é um número em inteiro(para evitar certos erros e warnings)
//Paramêtros:
//1. String para ser transformada
int atOi(const char numero[12]){
    int i=0;
    int count=-1;
    double res=0;


    while(numero[i]!='\0'){

        count++;
        i++;
    }
    for(int j=count; j>0; j--){
        res+=(numero[count-j]-48)*(pow(10, (j-1)));
    }


    return (int)res;
}

//Função para verificar se a string é número ou não(usada para evitar certos erros e warnings)
//Parâmetros:
//1. String a ser verificada
int isDigit(const char num[]){
    char numeros[]="0123456789";

    int i=0,  count=0;
    while(num[i]!='\0'){
        for(int j=0; j<(int)strlen(numeros); j++){
            if(num[i]==10 || num[i]==0){
                continue;
            }
            if(num[i]==numeros[j]){
                count++;
            }
        }
        i++;
    }

    if(count==i-1){
        return 0;
    }
    else{
        return 1;
    }
}

//Função para verificar se a string é mesmo um palavra ou não(usada para evitar certos erros e warnings)
//Parâmetros:
//1. String a ser verificada
int isString( char palavra[]){
    char letras[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i=0,  count=0, countEsp=0;
    for(i=0; i<31; i++){
        if(palavra[i]=='\0' || palavra[i]==13){
            break;
        }
        else{
            if(isSpace(palavra[i])==1){
                countEsp++;
                count++;
            }
            else {
                for (int j = 0; j < (int) strlen(letras); j++) {
                    if (toupper((int) palavra[i]) == letras[j]) {
                        count++;
                        break;
                    }
                }
            }
        }
    }
    if(count==i && countEsp!=count){
        return 0;
    }
    else{
        return 1;
    }
}

//Função para verificar se o caracter é "barra espaço" ou não(usada para evitar certos erros e warnings)
//Parâmetros:
//1. Caracter a ser verificado
int isSpace(char c){
    if(c==' '){
        return 1;
    }
    else{
        return 0;
    }
}

//Função para verificar se o caracter é um caracter de controlo ou não(usada para evitar certos erros e warnings)
//Parâmetros:
//1. Caracter a ser verificado
int isCntrl(char c){
    char str[9]="\a\b\f\n\r\t\v\0";
    for (int i=0; i < 8; i++) {
        if (c == str[i]) {
            return 1;
        }
    }

    return 0;
}

//Função ler o nome do ficheiro
//Parâmetros:
//1. Modo de entrada da escrita do nome do ficheiro
//2. Nome do ficheiro
//3. Tamanho máximo do nome do ficheiro
int get_one_line(FILE * fich, char *linha, int lim) {
    char c;
    int i=0;

    while (isSpace(c = (char)fgetc(fich)));
    if (c != EOF) {
        if (!isCntrl(c)) linha[i++] = c;
    } else
        return c;

    for (; i < lim - 1;) {
        c = (char)fgetc(fich);
        if (c == EOF)
            return c;
        if (c == '\n')
            break;
        if (!isCntrl(c)) linha[i++] = c;
    }
    linha[i] = 0;
    while ((c != EOF) && (c != '\n'))
        c = (char)fgetc(fich);
    return c;
}