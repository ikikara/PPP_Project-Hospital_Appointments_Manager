//
// Created by Ikiru on 5/13/2020.
//

typedef struct SMedicos *p_medico;
struct SMedicos {
    char medico[31];
    char especialidade1[31];
    char MDoentes[10][31];
    int hora[2];
    int NDoentes;
    p_medico nextM;
}SMedicos;

typedef struct SDoentes *p_doente;
struct SDoentes {
    char medicoD[31];
    char doente[31];
    char especialidade2[31];
    int horaD[2];
    p_doente nextD;
    int grau;
}SDoentes;

