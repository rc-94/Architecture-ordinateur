#ifndef FONCTIONS_H
#define FONCTIONS_H



/* HEARDER POUR LA MACHINE A PILE */


// Structure Cellule et Pile
typedef struct cellule {
        int entier;
        struct cellule *suivant;
} Cellule;


typedef struct {
        Cellule *premier;
} Pile;


// Fonction qui initialise une pile
Pile *Initialisation(void);


// Fonction qui permet d'empiler un element sur une pile
void Empiler(Pile *pile, int i);


// Fonction qui permet de depiler une pile et qui retourne l'element depile
int Depiler(Pile *pile);


// Fonction qui vérifie si un élement est dans un tableau
int CaractereDansTableau(char tab[], char x);


// Fonction qui trouve l'adresse d'une etiquette dans le fichier si l'etiquette est dans le fichier
int AdresseEtiquette(char etiquette[], char *nomfichier);


// Fonction qui traduit de l'hexadecimal en decimal
int HexaToDecimal(char *str);


// Fonction qui renvoie le nombre de chaines de caractères (correspondant aux instructions et donnees) dans un fichier
int NombreInstructionsDonnees(void);


// Fonction qui renvoie le nombre d'occurrences d'un caractere dans une chaine de caracteres
int Occurences(char tab[], char x);


// Fonction qui affiche le numero de la ligne erronnee dans le fichier (pendant l'ecriture de intermediaire)
int ErreurCompteurPlusUn(int compteur);


// Fonction qui affiche le numero de la ligne erronee dans le fichier (pendant l'ecriture de hexa)
int ErreurCompteur(int compteur);


// Fonction qui affiche le numero de la ligne erronee dans le fichier (cas ou il y a plusieurs etiquettes identiques)
int ErreurEtiquette(char* etiquette);


// Fonction qui renvoie le numero de la ligne erronee dans le fichier (cas ou la syntaxe est mauvaise)
// Detecte une erreur en comptant le nombre d'espaces entre les chaines de caracteres
int ErreurSyntaxe(char nomfichier[]);


// Fonction qui renvoie le numero de la ligne erronee dans le fichier (cas ou une instruction normalement suivie d'une donnee n'est pas suivi d'une donnee)
// Detecte une erreur si le code ascii du premier caractere apres l'instruction est superieur a 57 (code ascii de 9), ce n'est donc pas un chiffre
int ErreurDoubleInstruction(char nomfichier[]);



#endif
