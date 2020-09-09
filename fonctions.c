#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include "fonctions.h"
#define EMT_MAX 5000
#define LG_MAX 256



/* FONCTIONS POUR LA MACHINE A PILE */


// Fonction qui initialise une pile
Pile *Initialisation(void) {
        Pile *pile = malloc(sizeof(*pile));
        if (pile == NULL) {
                return NULL;
        }
        pile->premier = NULL;
        return pile;
}


// Fonction qui permet d'empiler un element sur une pile
void Empiler(Pile *pile, int i) {
        Cellule *nouveau = malloc(sizeof(*nouveau));
        if (pile == NULL || nouveau == NULL) {
                printf("Erreur.");
        }
        else {
                nouveau->entier = i;
                nouveau->suivant = pile->premier;
                pile->premier = nouveau;
        }
}


// Fonction qui permet de depiler une pile et qui retourne l'element depile
int Depiler(Pile *pile) {
        Cellule *elementAdepiler = pile->premier;
        int entierArecuperer = -1;

        if (pile == NULL) {
                return 0;
        }
        if (pile != NULL && pile->premier != NULL) {
                entierArecuperer = elementAdepiler->entier;
                pile->premier = elementAdepiler->suivant;
                free(elementAdepiler);
        }
        return entierArecuperer;
}


// Fonction qui vérifie si un élement est dans un tableau
int CaractereDansTableau(char tab[], char x) {
        int i = 0;
        while (tab[i] != '\0') {
                if (tab[i] == x) {
                        return 1;
                }
                i++;
        }
        return 0;
}


// Fonction qui trouve l'adresse d'une etiquette dans le fichier si l'etiquette est dans le fichier
int AdresseEtiquette(char etiquette[], char *nomfichier) {
        FILE* dictionnaire_etiquettes = fopen(nomfichier, "r");
        int adresse;
        char ligne_poubelle[LG_MAX];
        char etiquette_tmp[LG_MAX];

        while ((fscanf(dictionnaire_etiquettes, "%s", etiquette_tmp)) != EOF) {
                if (strcmp(etiquette, etiquette_tmp) == 0) {
                         fscanf(dictionnaire_etiquettes, "%d", &adresse);
                         fclose(dictionnaire_etiquettes);
                         return adresse;
                }
                fgets(ligne_poubelle, LG_MAX, dictionnaire_etiquettes);
        }
        fclose(dictionnaire_etiquettes);
        return -1;
}


// Fonction qui traduit de l'hexadecimal en decimal
int HexaToDecimal(char *str) {
        int valeur = 0;
        int index = 0;
        char c;
        while ((c = str[index++])) {
                if (isdigit(c)) {
                        valeur = 16 * valeur + (c -'0');
                }
                else {
                        valeur = 16 * valeur + (c -'a' + 10);
                }
        }
        return valeur;
}


// Fonction qui renvoie le nombre de chaines de caractères (correspondant aux instructions et donnees) dans un fichier
int NombreInstructionsDonnees(void) {
        int poubelle;
        int nb_instructions = 0;
        FILE* hexa = fopen("hexa.txt", "r");
        while (fscanf(hexa, "%x", &poubelle) != EOF) {
                nb_instructions ++;
        }
        fclose(hexa);
        return nb_instructions;
}


// Fonction qui renvoie le nombre d'occurrences d'un caractere dans une chaine de caracteres
int Occurences(char tab[], char x) {
        int i = 0;
        int occurrences = 0;
        while (tab[i] != '\0') {
                if (tab[i] == x) {
                        occurrences++;
                }
                i++;
        }
        return occurrences;
}


// Fonction qui affiche le numero de la ligne erronnee dans le fichier (pendant l'ecriture de intermediaire)
int ErreurCompteurPlusUn(int compteur) {
        printf("Erreur detectee a la ligne %d.\n", compteur + 1);
        remove("intermediaire.txt");
        remove("dictionnaire_etiquettes.txt");
        remove("hexa.txt");
        return -1;
}


// Fonction qui affiche le numero de la ligne erronee dans le fichier (pendant l'ecriture de hexa)
int ErreurCompteur(int compteur) {
        printf("Erreur detectee a la ligne %d.\n", compteur);
        remove("intermediaire.txt");
        remove("dictionnaire_etiquettes.txt");
        remove("hexa.txt");
        return -1;
}


// Fonction qui affiche le numero de la ligne erronee dans le fichier (cas ou il y a plusieurs etiquettes identiques)
int ErreurEtiquette(char* etiquette) {
        char etiquette_tmp[LG_MAX];
        int compteur = 0;
        int erreur;
        FILE* dictionnaire_etiquettes = fopen("dictionnaire_etiquettes.txt", "r");
        while (fscanf(dictionnaire_etiquettes, "%s", etiquette_tmp) != EOF) {
                if (strcmp(etiquette_tmp, etiquette) == 0) {
                        compteur++;
                }
                fscanf(dictionnaire_etiquettes, "%d", &erreur);
                if (compteur >= 2) {
                        printf("Il semblerait y avoir deux étiquettes identiques...\n");
                        printf("Erreur detectee a la ligne %d.\n", erreur + 1);
                        return 1;
                }
        }

        fclose(dictionnaire_etiquettes);
        return 0;
}


// Fonction qui renvoie le numero de la ligne erronee dans le fichier (cas ou la syntaxe est mauvaise)
// Detecte une erreur en comptant le nombre d'espaces entre les chaines de caracteres
int ErreurSyntaxe(char nomfichier[]) {

        FILE* fichier = fopen(nomfichier, "r");
        int ligne = 1;
        char premier_scan[LG_MAX];
        char deuxieme_scan[LG_MAX];
        char ipop[] = "ipop";
        char ipush[] = "ipush";
        char dup[] = "dup";
        char halt[] = "halt";

        while ((fscanf(fichier, "%s", premier_scan)) != EOF) {

                fgets(deuxieme_scan, LG_MAX, fichier);
                
                // Si la ligne recuperee contient normalement une etiquette
                if (CaractereDansTableau(premier_scan, ':') == 1) {
                
                        // Si la ligne recuperee contient normalement une etiquette et une instruction sans donnee
                        if (strstr(deuxieme_scan, ipop) != NULL ||
                            strstr(deuxieme_scan, ipush) != NULL ||
                            strstr(deuxieme_scan, dup) != NULL ||
                            strstr(deuxieme_scan, halt) != NULL)
                        {                                
                               if (Occurences(deuxieme_scan, ' ') > 1) {
                                       return ligne; 
                               }
                        }
                        // Si la ligne recuperee contient normalement une etiquette et une instruction suivie d'une donnee                   
                        else {
                                if (Occurences(deuxieme_scan, ' ') > 2) {
                                        return ligne;
                                }
                        }
                }                
                // Si la ligne recuperee ne contient normalement pas d'etiquette et une instruction sans donnee
                else if (strstr(premier_scan, ipop) != NULL ||
                         strstr(premier_scan, ipush) != NULL ||
                         strstr(premier_scan, dup) != NULL ||
                         strstr(premier_scan, halt)!= NULL)
                {                         
                         if (Occurences(deuxieme_scan, ' ') > 0) {
                                return ligne;
                        }                                                
                }
                // Si la ligne recuperee ne contient normalement pas d'etiquette et une instruction suivie d'une donnee
                else {
                        if (Occurences(deuxieme_scan, ' ') > 1) {
                                return ligne;
                        }
                }
                
                ligne++;
        }
        
        fclose(fichier);
        return 0;
}


// Fonction qui renvoie le numero de la ligne erronee dans le fichier (cas ou une instruction normalement suivie d'une donnee n'est pas suivi d'une donnee)
// Detecte une erreur si le code ascii du premier caractere apres l'instruction est superieur a 57 (code ascii de 9), ce n'est donc pas un chiffre
int ErreurDoubleInstruction(char nomfichier[]) {

        FILE* fichier = fopen(nomfichier, "r");
        int ligne = 1;
        char premier_scan[LG_MAX];
        char deuxieme_scan[LG_MAX];
        char troisieme_scan[LG_MAX];

        while ((fscanf(fichier, "%s", premier_scan)) != EOF) {
                
                // Cas ou une etiquette est scannee
                if (CaractereDansTableau(premier_scan, ':') == 1) {
                        fscanf(fichier,"%s", deuxieme_scan);
                        fgets(troisieme_scan, LG_MAX, fichier);
                        if (troisieme_scan[1] > 57 &&
                           (strcmp(deuxieme_scan, "pop") == 0 ||
                            strcmp(deuxieme_scan, "push") == 0 ||
                            strcmp(deuxieme_scan, "push#") == 0 ||
                            strcmp(deuxieme_scan, "op") == 0 ||
                            strcmp(deuxieme_scan, "rnd") == 0 ||
                            strcmp(deuxieme_scan, "write") == 0 ||
                            strcmp(deuxieme_scan, "read") == 0))
                        {
                            return ligne;
                        }
                }
                
                // Cas ou une instruction est scannee
                else{
                        fgets(deuxieme_scan, LG_MAX, fichier);
                        if (deuxieme_scan[1] > 57 &&
                           (strcmp(premier_scan, "pop") == 0 ||
                            strcmp(premier_scan, "push") == 0 ||
                            strcmp(premier_scan, "push#") == 0 ||
                            strcmp(premier_scan, "op") == 0 ||
                            strcmp(premier_scan, "rnd") == 0 ||
                            strcmp(premier_scan, "write") == 0 ||
                            strcmp(premier_scan, "read") == 0))
                        {
                            return ligne;
                        }
                }

                ligne++;
        }
        
        fclose(fichier);
        return 0;
}
