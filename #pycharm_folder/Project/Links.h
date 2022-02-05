//
// Created by Ikiru on 5/13/2020.
//

#ifndef PROJETO_LINKS_H
#define PROJETO_LINKS_H

#endif //PROJETO_LINKS_H

int isDigit(const char num[]);

int checkTXT(char string[], char extencao[]);

void split( char *linha, char elemts[3][31], int nEle);

int atOi(const char numero[12]);

int checkTEXTOeCONTEUDO(int atributos, char [][atributos][31], int nMedicos);

void OrganizarConsultas(char medicos[][2][31], char doentes[][3][31], int linhaM, int linhaD);

void Imprimir();

int get_one_line(FILE * fich, char *linha, int lim);