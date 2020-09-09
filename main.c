#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include "fonctions.h"
#define EMT_MAX 5000
#define LG_MAX 256



/* PROJET D'ARCHITECTURE DES ORDINATEURS : MACHINE A PILE */


int main(int argc, char *argv[]) {


/* PREMIERE PARTIE DU PROGRAMME : TRADUCTION EN HEXADECIMAL */

        printf("Bienvenue dans la machine a Pile !\n\n");
        
        
        // Cas ou aucun fichier n'est soumis
        if (argc != 2) {
                printf("N'oubliez pas de rentrer un fichier !\n");
                return -1;
        }
        

        FILE* fichier = fopen(argv[1], "r");
        
        
        // Cas ou le fichier soumis n'existe pas
        if (fichier == NULL) {
                printf("Votre fichier demeure introuvable...\n");
                return -1;
        }
        
        
        printf("...Traduction de votre fichier en hexadecimal...\n\n");
        
        
        // Verification de la syntaxe dans le fichier mis en argument
        int erreur_syntaxe = ErreurSyntaxe(argv[1]);
        
        if (erreur_syntaxe != 0) {
                printf("La syntaxe de votre fichier semble surprenante...\n");
                return ErreurCompteur(erreur_syntaxe);
        }
        
        
        // Verfification du fait qu'il n'y ait pas de chaine de caracteres apres une instruction normalement suivie d'une donnee dans le fichier mis en argument
        int erreur_double_instruction = ErreurDoubleInstruction(argv[1]);
        
        if (erreur_double_instruction != 0) {
                printf("Il semblerait y avoir une instruction suivie d'un argument non valide...\n");
                return ErreurCompteur(erreur_double_instruction);
        }
        
        
        
        /* SOUS-PARTIE 1 : ecriture de deux fichiers "intermediaire.txt" et "dictionnaire_etiquettes.txt" */
        
        FILE* dictionnaire_etiquettes = fopen("dictionnaire_etiquettes.txt", "w");
        FILE* intermediaire = fopen("intermediaire.txt", "w");
        char etiquette_ou_instruction[LG_MAX];
        char poubelle_instruction[LG_MAX];
        char poubelle_etiquette[LG_MAX];
        int poubelle_donnees;        
        int compteur = 0;

        // Lecture du fichier donne en argument
        while ((fscanf(fichier, "%s", etiquette_ou_instruction)) != EOF) {

                // Cas ou l'etiquette ou l'instruction fait plus de 256 caractres : elle n'existe donc pas
                if (strlen(etiquette_ou_instruction) > LG_MAX) {
                        printf("Une etiquette ou une instruction semble etre un peu longue...\n");
                        return ErreurCompteurPlusUn(compteur);
                }

                // Cas ou l'element scanne est une etiquette
                if (CaractereDansTableau(etiquette_ou_instruction,':') == 1) {
                
                        // Ecriture de l'etiquette dans "dictionnaire_etiquettes.txt" sans le caractère ':'
                        etiquette_ou_instruction[strlen(etiquette_ou_instruction)-1] = '\0';
                        fprintf(dictionnaire_etiquettes, "%s %d\n", etiquette_ou_instruction, compteur);
                        
                        // Ecriture de l'instruction dans "intermediaire.txt"
                        fscanf(fichier, "%s", poubelle_instruction);
                        
                        if(strlen(poubelle_instruction) > LG_MAX) {
                                printf("Une instruction, qui semble bien longue, n'est pas prise en charge par la machine...\n");
                                return ErreurCompteurPlusUn(compteur);
                        }
                        fprintf(intermediaire, "%s ", poubelle_instruction);
                        
                        // Ecriture de la donnee dans "intermediaire.txt" : cas ou c'est une etiquette
                        if (strcmp(poubelle_instruction, "call") == 0 ||
                            strcmp(poubelle_instruction, "jpz") == 0 ||
                            strcmp(poubelle_instruction, "jmp") == 0)
                        {
                                fscanf(fichier, "%s", poubelle_etiquette);
                                if (strlen(poubelle_etiquette) > LG_MAX) {
                                        return ErreurCompteurPlusUn(compteur);
                                }
                                fprintf(intermediaire, "%s\n", poubelle_etiquette);
                        }
                        // Ecriture de la donnee dans "intermediaire.txt" : cas ou c'est un nombre
                        else {
                                fscanf(fichier, "%d", &poubelle_donnees);
                                if ((poubelle_donnees < INT_MIN) || (poubelle_donnees > INT_MAX)) {
                                        return ErreurCompteurPlusUn(compteur);
                                }
                                fprintf(intermediaire, "%d\n", poubelle_donnees);
                        }
                }

                // Cas ou l'element scanne est une instruction
                else {
                        // Ecriture de l'instruction dans "intermediaire.txt"
                        fprintf(intermediaire, "%s ", etiquette_ou_instruction);
                        
                        // Ecriture de la donnee dans "intermediaire.txt" : cas ou c'est une etiquette
                        if (strcmp(etiquette_ou_instruction, "call") == 0 ||
                            strcmp(etiquette_ou_instruction, "jpz") == 0 ||
                            strcmp(etiquette_ou_instruction, "jmp") == 0)
                        {
                                fscanf(fichier, "%s", poubelle_etiquette);
                                if (strlen(poubelle_etiquette) > LG_MAX) {
                                        printf("Une etiquette semble etre un peu longue...\n");
                                        return ErreurCompteurPlusUn(compteur);
                                }
                                fprintf(intermediaire, "%s\n", poubelle_etiquette);
                        }
                        // Ecriture de la donnee dans "intermediaire.txt" : cas ou c'est un nombre
                        else {
                                fscanf(fichier, "%d", &poubelle_donnees);
                                if ((poubelle_donnees < INT_MIN) || (poubelle_donnees > INT_MAX)) {
                                        return ErreurCompteurPlusUn(compteur);
                                }
                                fprintf(intermediaire, "%d\n", poubelle_donnees);
                        }
                }
                
                compteur ++;
        }
        
        fclose(intermediaire);
        fclose(dictionnaire_etiquettes);


        // On vérifie s'il n'y a pas deux étiquettes identiques
        dictionnaire_etiquettes = fopen("dictionnaire_etiquettes.txt", "r");

        while (fscanf(dictionnaire_etiquettes, "%s", etiquette_ou_instruction) != EOF) {
                if (ErreurEtiquette(etiquette_ou_instruction) == 1) {
                        remove("intermediaire.txt");
                        remove("dictionnaire_etiquettes.txt");
                        remove("hexa.txt");
                        return -1;
                }
        }
        
        fclose(dictionnaire_etiquettes);

        
        
        /* SOUS-PARTIE 2 : ecriture en hexadecimal dans le fichier a generer "hexa.txt" */
        
        FILE* hexa = fopen("hexa.txt", "w");
        intermediaire = fopen("intermediaire.txt", "r");
        char instruction[LG_MAX];
        char etiquette[LG_MAX];
        int donnees;
        int adresse_etiquette;
        compteur = 0;

        while ((fscanf(intermediaire, "%s", instruction)) != EOF) {
                compteur ++;
                
                // Cas ou l'instruction est suivie d'une etiquette
                if (strcmp(instruction, "call") == 0 ||
                    strcmp(instruction, "jpz") == 0 ||
                    strcmp(instruction, "jmp") == 0)
                {
                        fscanf(intermediaire, "%s", etiquette);
                        adresse_etiquette = AdresseEtiquette(etiquette, "dictionnaire_etiquettes.txt");
                        
                        if (strcmp(instruction, "jmp") == 0) {
                                if (adresse_etiquette == -1) {
                                        printf("Malheureusement '%s' ne semble correspondre a aucune etiquette...\n", etiquette);
                                        return ErreurCompteur(compteur);
                                }
                                else {
                                        fprintf(hexa, "%02x %08x\n", 7, -(compteur - adresse_etiquette));
                                }
                        }
                        else if (strcmp(instruction,"jpz") == 0) {
                                if (adresse_etiquette == -1) {
                                        printf("Malheureusement '%s' ne semble correspondre a aucune etiquette...\n", etiquette);
                                        return ErreurCompteur(compteur);
                                }
                                else {
                                        fprintf(hexa, "%02x %08x\n", 8, -(compteur - adresse_etiquette));
                                }                                
                        }
                        else if (strcmp(instruction,"call") == 0) {
                                if (adresse_etiquette == -1) {
                                        printf("Malheureusement '%s' ne semble correspondre a aucune etiquette...\n", etiquette);
                                        return ErreurCompteur(compteur);
                                }
                                else {
                                        fprintf(hexa, "%02x %08x\n", 9, -(compteur - adresse_etiquette));
                                }
                        }
                }

                // Cas ou l'instruction est suivie d'une donnee
                else if (strcmp(instruction, "push") == 0 ||
                         strcmp(instruction, "ipush") == 0 ||
                         strcmp(instruction, "push#") == 0 ||
                         strcmp(instruction, "pop") == 0 ||
                         strcmp(instruction, "ipop") == 0 ||
                         strcmp(instruction, "dup") == 0 ||
                         strcmp(instruction, "op") == 0 ||
                         strcmp(instruction, "ret") == 0 ||
                         strcmp(instruction, "rnd") == 0 ||
                         strcmp(instruction, "write") == 0 ||
                         strcmp(instruction, "halt") == 0 ||
                         strcmp(instruction, "read") == 0)
                {
                        fscanf(intermediaire, "%d", &donnees);
                        
                        if (strcmp(instruction, "push") == 0) {
                                fprintf(hexa, "%02x %08x\n", 0, donnees);
                        }
                        else if (strcmp(instruction, "ipush") == 0) {
                                fprintf(hexa, "%02x %08x\n", 1, 0);
                        }
                        else if (strcmp(instruction, "push#") == 0) {
                                fprintf(hexa, "%02x %08x\n", 2, donnees);
                        }
                        else if (strcmp(instruction, "pop") == 0) {
                                fprintf(hexa, "%02x %08x\n", 3, donnees);
                        }
                        else if (strcmp(instruction, "ipop") == 0) {
                                fprintf(hexa, "%02x %08x\n", 4, 0);
                        }
                        else if (strcmp(instruction, "dup") == 0) {
                                fprintf(hexa, "%02x %08x\n", 5, 0);
                        }
                        else if (strcmp(instruction, "op") == 0) {
                                fprintf(hexa, "%02x %08x\n", 6, donnees);
                        }
                        else if (strcmp(instruction, "ret") == 0) {
                                fprintf(hexa, "%02x %08x\n", 10, 0);
                        }
                        else if (strcmp(instruction, "rnd") == 0) {
                                fprintf(hexa, "%02x %08x\n", 11, donnees);
                        }
                        else if (strcmp(instruction, "write") == 0) {
                                fprintf(hexa, "%02x %08x\n", 12, donnees);
                        }
                        else if (strcmp(instruction, "read") == 0) {
                                fprintf(hexa, "%02x %08x\n", 13, donnees);
                        }
                        else if (strcmp(instruction, "halt") == 0) {
                                fprintf(hexa, "%02x %08x\n", 99, 0);
                        }
                }

                else {
                        printf("Malheureusement l'instruction '%s' n'est pas prise en charge par la machine a Pile...\n", instruction);
                        return ErreurCompteur(compteur);
                }

        }
        
        fclose(hexa);
        fclose(intermediaire);
        remove("intermediaire.txt");
        remove("dictionnaire_etiquettes.txt");



/* DEUXIÈME PARTIE DU PROGRAMME : SIMULATION DE LA MACHILE A PILE */

        printf("Aucune coquille detectee. Bonne simulation !\n\n");

        // Creation de 2 tableaux : 1 pour les instructions et 1 pour les donnees
        int *tableau_instructions = NULL;
        int *tableau_donnees = NULL;
        int nb = NombreInstructionsDonnees();
        int indice = 0;
        int instruction2;
        int donnees2;
        char donnees_str[LG_MAX];

        hexa = fopen("hexa.txt", "r");
        tableau_instructions = malloc(sizeof(int) * nb);
        tableau_donnees = malloc(sizeof(int) * nb);

        while ((fscanf(hexa, "%x", &instruction2)) != EOF){
                fscanf(hexa, "%s", donnees_str);
                donnees2 = HexaToDecimal(donnees_str);
                tableau_instructions[indice] = instruction2;
                tableau_donnees[indice] = donnees2;
                indice++;
        }
        
        fclose(hexa);


        // Execution des instructions
        Pile *SP = Initialisation();
        int PC = 0;
        int EMT[EMT_MAX];
        int sommet;
        int random;
        int condition_arret = 0;

        while (PC != nb) {
                instruction2 = tableau_instructions[PC];
                donnees2 = tableau_donnees[PC];
                PC ++;

                switch (instruction2) {

                case 0x0 : // push
                        if (donnees2 > EMT_MAX) {
                                printf("Vous empietez sur un territoire interdit. Toi qui entres ici, prends garde. Seuls %d pas te sont permis.\n", EMT_MAX);
                                return ErreurCompteur(PC);
                        }
                        Empiler(SP, EMT[donnees2]);
                        break;

                case 0x1 : // ipush
                        sommet = Depiler(SP);
                        if (sommet > EMT_MAX) {
                                printf("Vous empietez sur un territoire interdit. Toi qui entres ici, prends garde. Seuls %d pas te sont permis.\n", EMT_MAX);
                                return ErreurCompteur(PC);
                        }
                        Empiler(SP, EMT[sommet]);
                        break;

                case 0x2 : // push#
                        Empiler(SP, donnees2);
                        break;

                case 0x3 : // pop
                        sommet = Depiler(SP);
                        if (donnees2 > EMT_MAX) {
                                printf("Vous empietez sur un territoire interdit. Toi qui entres ici, prends garde. Seuls %d pas te sont permis.\n", EMT_MAX);
                                return ErreurCompteur(PC);
                        }
                        EMT[donnees2] = sommet;
                        break;

                case 0x4 : // ipop
                        sommet = Depiler(SP);
                        if (sommet > EMT_MAX) {
                                printf("Vous empietez sur un territoire interdit. Toi qui entres ici, prends garde. Seuls %d pas te sont permis.\n", EMT_MAX);
                                return ErreurCompteur(PC);
                        }
                        EMT[sommet] = Depiler(SP);
                        break;

                case 0x5 : // dup
                        sommet = SP->premier->entier;
                        Empiler(SP, sommet);
                        break;

                case 0x6 : // op
                        switch (donnees2) {
                        
                        case 0 : // et logique bit a bit
                                sommet = Depiler(SP);
                                SP->premier->entier = SP->premier->entier & sommet;
                                break;
                                
                        case 1 : // ou logique bit a bit
                                sommet = Depiler(SP);
                                SP->premier->entier = SP->premier->entier | sommet;
                                break;
                                
                        case 2 : // ou-exclusif logique bit a bit
                                sommet = Depiler(SP);
                                SP->premier->entier = SP->premier->entier ^ sommet;
                                break;
                                
                        case 3 : // non logique bit a bit
                                SP->premier->entier = ~(SP->premier->entier);
                                break;
                                
                        case 4 : //inverse la valeur au sommet de la pile
                                SP->premier->entier = -(SP->premier->entier);
                                break;
                                
                        case 5 : // additionne les deux valeurs au sommet
                                sommet = Depiler(SP);
                                SP->premier->entier = SP->premier->entier + sommet;
                                break;
                                
                        case 6 : // soustraction
                                sommet = Depiler(SP);
                                SP->premier->entier = SP->premier->entier - sommet;
                                break;
                                
                        case 7 : // multiplication
                                sommet = Depiler(SP);
                                SP->premier->entier = SP->premier->entier * sommet;
                                break;
                                
                        case 8 : // division entiere
                                sommet = Depiler(SP);
                                SP->premier->entier = SP->premier->entier / sommet;
                                break;
                                
                        case 9 : // modulo
                                sommet = Depiler(SP);
                                SP->premier->entier = SP->premier->entier % sommet;
                                break;
                                
                        case 10 : // test d'egalite
                                sommet = Depiler(SP);
                                if (SP->premier->entier == sommet) {
                                        SP->premier->entier = 0;
                                }
                                else {
                                        SP->premier->entier = 1;
                                }
                                break;
                                
                        case 11 : // test d'inegalite
                                sommet = Depiler(SP);
                                if (SP->premier->entier != sommet) {
                                        SP->premier->entier = 0;
                                }
                                else{
                                        SP->premier->entier = 1;
                                }
                                break;
                                
                        case 12 : // test strictement superieur
                                sommet = Depiler(SP);
                                if (SP->premier->entier > sommet) {
                                        SP->premier->entier = 0;
                                }
                                else {
                                        SP->premier->entier = 1;
                                }
                                break;
                                
                        case 13 : // test superieur ou egal
                                sommet = Depiler(SP);
                                if (SP->premier->entier >= sommet) {
                                        SP->premier->entier = 0;
                                }
                                else{
                                        SP->premier->entier=1;
                                }
                                break;
                                
                         case 14 : // test strictement inferieur
                                sommet = Depiler(SP);
                                if (SP->premier->entier < sommet) {
                                        SP->premier->entier = 0;
                                }
                                else {
                                        SP->premier->entier = 1;
                                }
                                break;
                                
                         case 15 : // test inferieur ou egal
                                sommet = Depiler(SP);
                                if (SP->premier->entier <= sommet) {
                                        SP->premier->entier = 0;
                                }
                                else {
                                        SP->premier->entier = 1;
                                }
                                break;
                        } 
                        break;

                case 0x7 : // jmp
                        PC = PC + donnees2;
                        break;

                case 0x8 : // jpz
                        sommet = Depiler(SP);
                        if (sommet == 0) {
                                PC = PC + donnees2;
                        }
                        break;

                case 0x9 : // call
                        Empiler(SP, PC);
                        PC = PC + donnees2;
                        break;

                case 0xa : // ret
                        PC = Depiler(SP);
                        break;

                case 0xb : // rnd
                        random = rand()%(donnees2-1);
                        Empiler(SP, random);
                        break;

                case 0xc : // write
                        if (donnees2 > EMT_MAX) {
                                printf("Vous empietez sur un territoire interdit. Toi qui entres ici, prends garde. Seuls %d pas te sont permis.\n", EMT_MAX);
                                return ErreurCompteur(PC);
                        }
                        else {
                                printf("Valeur contenue a l'adresse %d : %d\n", donnees2, EMT[donnees2]);
                                }
                        break;

                case 0xd : // read
                        printf("Entrez une valeur : ");
                        scanf("%d", &EMT[donnees2]);                        
                        if (donnees2 > EMT_MAX) {
                                printf("Vous empietez sur un territoire interdit. Toi qui entres ici, prends garde. Seuls %d pas te sont permis.\n", EMT_MAX);
                                return ErreurCompteur(PC);
                        }
                        break;

                case 0x63 : // halt
                        printf("\nFin du programme. GAME OVER.\n");
                        condition_arret = 1;
                        break;
                }

                if (condition_arret == 1) {
                        printf("\nA bientot !\n");
                        return 1;
                }
        }
        printf("\nLe programme que vous avez soumis ne semble pas complet...\nA tres bientot avec une nouvelle version !\n");
        return 0;
}


/* FIN DU PROGRAMME SUR LA MACHINE A PILE */
