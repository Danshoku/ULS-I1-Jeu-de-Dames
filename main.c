#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define LARGEUR_FENETRE 800
#define LONGUEUR_FENETRE 800
#define TAILLE_PLATEAU 10
#define TAILLE_CASE (LARGEUR_FENETRE / TAILLE_PLATEAU)

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void remplissage(int tab[10][10]){ //Remplissage du tableau de base (10x10)
    int i, j;
    int tab1[10] = {0, 2, 0, 2, 0, 0, 0, 1, 0, 1};
    int tab2[10] = {2, 0, 2, 0, 0, 0, 1, 0, 1, 0};
    for(i=1; i<=10; i++){
        if(i%2==0){
            for(j=0; j<10; j++){
                tab[i-1][j] = tab2[j];
            }
        }else{
            for(j=0; j<10; j++){
                tab[i-1][j] = tab1[j];
            }
        }
    }
}

void afficher_plateau(int tab[10][10]){
    int i, j;
    printf("    ");
    for(i=0; i<10; i++){
        printf("(%i)", i);
    }
    printf("\n");
    for(i=0; i<10; i++){
        printf("(%i)  ", i);
        for(j=0; j<10; j++){
            printf("%i  ", tab[j][i]);
        }
        printf("\n");
    }
}

void afficher_plateau_char(int tab[10][10]){
    int i, j;
    printf("    ");
    for(i=0; i<10; i++){
        printf("(%i)", i);
    }
    printf("\n");
    for(i=0; i<10; i++){
        printf("(%i)  ", i);
        for(j=0; j<10; j++){
            if(tab[j][i]==0){
                printf("%c  ", ' ');
            }else{
                if(tab[j][i]==1){
                    printf("%c  ", 'o');
                }
                if(tab[j][i]==2){
                    printf("%c  ", 'x');
                }
                if(tab[j][i]==10){
                    printf("%c  ", 'O');
                }
                if(tab[j][i]==20){
                    printf("%c  ", 'X');
                }
            }
        }
        printf("\n");
    }
}

void score(int tab[10][10], int pion_restant[2]){ //Calcul du score selon  les pions restant
    int i, j;
    pion_restant[0] = 0;
    pion_restant[1] = 0;
    for(i=0; i<10; i++){
        for(j=0; j<10; j++){
            if(tab[i][j]==1){
                pion_restant[0]+=1;
            }
            if(tab[i][j]==2){
                pion_restant[1]+=1;
            }
            if(tab[i][j]==10){
                pion_restant[0]+=1;
            }
            if(tab[i][j]==20){
                pion_restant[1]+=1;
            }
        }
    }
}

int alterne(int joueur){ //Changer de joueur
    if(joueur==1){
        return 2;
    }else{
        return 1;
    }
}

void deplacer_pion(int tab[10][10], int pion[2], int coup[2]){ //Déplacement d'un pion + prise eventuel
    int pion_c, pion_l, coup_c, coup_l;
    pion_c = pion[0], pion_l = pion[1], coup_c = coup[0], coup_l = coup[1];
    int var_temp;
    if(pion_c==coup_c-1 || pion_c==coup_c+1){
        if(pion_l==coup_l-1 || pion_l==coup_l+1){
            var_temp = tab[pion_c][pion_l];
            tab[pion_c][pion_l] = 0;
            tab[coup_c][coup_l] = var_temp;
        }
    }else{
        if(pion_c==coup_c-2 && pion_l==coup_l-2){
            tab[coup_c-1][coup_l-1] = 0;
        }
        if(pion_c==coup_c+2 && pion_l==coup_l+2){
            tab[coup_c+1][coup_l+1] = 0;
        }
        if(pion_c==coup_c-2 && pion_l==coup_l+2){
            tab[coup_c-1][coup_l+1] = 0;
        }
        if(pion_c==coup_c+2 && pion_l==coup_l-2){
            tab[coup_c+1][coup_l-1] = 0;
        }
        var_temp = tab[pion_c][pion_l];
        tab[pion_c][pion_l] = 0;
        tab[coup_c][coup_l] = var_temp;
    }
}

void deplacer_dame(int tab[10][10], int pion[2], int coup[2]){ //Déplacement d'une dame + prise eventuel
    int nb, co, li;
    nb = 0;
    co = coup[0] - pion[0];
    li = coup[1] - pion[1];
    int tmp = tab[pion[0]][pion[1]];
    if(co>0 && li>0){
        while(co!=0&&li!=0){
            co=co-1;
            li=li-1;
            tab[co+pion[0]][li+pion[1]] = 0;
        }
    }
    if(co<0 && li<0){
        while(co!=0&&li!=0){
            co=co+1;
            li=li+1;
            tab[co+pion[0]][li+pion[1]] = 0;
        }
    }
    if(co>0 && li<0){
        while(co!=0&&li!=0){
            co=co-1;
            li=li+1;
            tab[co+pion[0]][li+pion[1]] = 0;
        }
    }
    if(co<0 && li>0){
        while(co!=0&&li!=0){
            co=co+1;
            li=li-1;
            tab[co+pion[0]][li+pion[1]] = 0;
        }
    }
    tab[pion[0]][pion[1]] = 0;
    tab[coup[0]][coup[1]] = tmp;

}

void affiche_regles(int t[10][10]){ //Affichage des règles du jeu
    printf("Voici les regles du jeu de dames.\n\n");
    printf("Materiel :\n\n");
    printf("Le jeu de dames se joue sur un damier carre divise en 100 cases egales,alternativement claires et foncees\n");
    printf("Le jeu se joue sur les cases foncees du damier, il y a donc 50 cases actives.\n");
    printf("Chaque joueur possede 20 pions, l'un joue les pions noirs et l'autre joue les pions blancs.\n");
    printf("Avant de debuter une partie, les 20 pions noirs et les 20 pions blancs sont disposés sur les 4 premieres rangees de chaque joueur.\n");
    printf("Ci-dessous le tableau de jeu :\n\n");
    afficher_plateau_char(t);
    printf("\n\nDeplacement :\n\n");
    printf("Il existe deux types de pieces : les pions et les dames.\n");
    printf("Le premier coup est toujours joue par les blancs. Puis les adversaires jouent chacun un coup tour a tour.\n");
    printf("Un pion se deplace obligatoirement vers l’avant, en diagonale, sur une case libre de la rangee suivante.\n");
    printf("Lorsqu'il atteint la derniere rangee, le pion devient dame.\n");
    printf("La dame peut se deplacer en avant et en arriere, sur le nombre de cases libres successives de sa diagonale.\n ");
    printf("\n\nPrise :\n\n");
    printf("La prise des pieces adverses est obligatoire et s'effectue en avant et en arriere.\n");
    printf("C'est a dire que lorsqu'un pion se trouve en presence, diagonalement, d'une piece adverse derriere laquelle se trouve une case libre, il doit obligatoirement sauter par-dessus cette piece et occuper la case libre.\n");
    printf("La piece adverse est alors enlevee du damier.\n");
    printf("C'est la meme chose pour la dame sauf qu'elle peut se mettre sur n'importe quelle case libre derriere le pion qu'elle a pris.\n");
    printf("Apres la prise d'un pion, si le pion se retrouve encore en diagonale avec un pion adverse, alors il peut aussi prendre ce pion et ainsi de suite tant que c'est possible, c'est la meme chose pour la prise de pions avec la dame.\n");
    printf("\n\nFin de partie :\n");
    printf("Les cas de victoire :\n\n");
    printf("-Lorsqu'un des 2 joueurs ne peut pas se deplacer alors que c'est a son tour de jouer.\n");
    printf("-Lorsqu'un des 2 joueurs n'a plus de pieces sur le plateau.\n\n");
    printf("Les cas de l'egalite :\n\n");
    printf("S'il n'y a plus que trois dames, deux dames et un pion, ou une dame et deux pions contre une dame, la fin de partie sera consideree comme egale lorsque les deux joueurs auront encore joue chacun 16 coups au maximum.");
}

void sauvegarder_plateau(int tab[10][10], const char* fichier_plateau){ //Sauvegarde du plateau dans un fichier texte
    FILE* fichier = fopen(fichier_plateau, "w"); //Ouverture du fichier texte en mode écriture
    if(fichier == NULL){
        return;
    }
    for(int i = 0; i < 10; i++){ //Ecriture du tableau dans le fichier texte
        for(int j = 0; j < 10; j++){
            fprintf(fichier, "%d ", tab[i][j]);
        }
        fprintf(fichier, "\n");
    }
    fclose(fichier); //Fermeture du fichier texte
}

void charger_plateau(int tab[10][10], const char* fichier_plateau){ //Chargement du plateau avec le fichier texte
    FILE* fichier = fopen(fichier_plateau, "r"); //Ouverture du fichier texte en mode lecture
    if(fichier == NULL){
        return;
    }
    for(int i = 0; i < 10; i++){ //Lecture du plateau dans le fichier texte
        for(int j = 0; j < 10; j++){
            fscanf(fichier, "%d", &tab[i][j]);
        }
    }
    fclose(fichier); //Fermeture du fichier texte
}

void sauvegarder_joueur(const char* fichier_joueur, int joueur){ //Sauvegarde du joueur dans un fichier texte
    FILE* fichier = fopen(fichier_joueur, "w"); //Ouverture du fichier texte en mode écriture
    if(fichier == NULL){
        return;
    }
    fprintf(fichier, "%d", joueur); //Ecriture du joueur dans le fichier texte
    fclose(fichier); //Fermeture du fichier texte
}

int charger_joueur(int joueur, const char* fichier_joueur){ //Chargement du joueur dans un fichier texte
    FILE* fichier = fopen(fichier_joueur, "r"); //Ouverture du fichier texte en mode lecture
    if(fichier == NULL){
        return 0;
    }
    fscanf(fichier, "%d", &joueur);
    fclose(fichier); //Fermeture du fichier texte + retour du numero du joueur
    return joueur;
}

void dessiner_pions(SDL_Renderer* renderer, int plateau[10][10]){ //Dessin des differents pions
    for(int i = 0; i < TAILLE_PLATEAU; i++){
        for(int j = 0; j < TAILLE_PLATEAU; j++){
            if(plateau[j][i] == 1){ //Création de la couleur du pion avec le cercle (Blanc)
                int radius = TAILLE_CASE / 2 - 5; //Creation du rayon du cercle selon la taille de la case
                int x = j * TAILLE_CASE + TAILLE_CASE / 2; //Creation des cordonnées du centre de la case
                int y = i * TAILLE_CASE + TAILLE_CASE / 2;
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //Creation de la couleur blanc
                for(int dx = -radius; dx <= radius; ++dx){ //Dessin du cercle avec la bonne couleur selon les coordonnées
                    for(int dy = -radius; dy <= radius; ++dy){
                        if(dx*dx + dy*dy <= radius*radius){
                            SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                        }
                    }
                }
            }
            if(plateau[j][i] == 2){ //Création de la couleur du pion avec le cercle (Noir)
                int radius = TAILLE_CASE / 2 - 5; //Creation du rayon du cercle selon la taille de la case
                int x = j * TAILLE_CASE + TAILLE_CASE / 2; //Creation des cordonnées du centre de la case
                int y = i * TAILLE_CASE + TAILLE_CASE / 2;
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Creation de la couleur blanc
                for(int dx = -radius; dx <= radius; ++dx){ //Dessin du cercle avec la bonne couleur selon les coordonnées
                    for(int dy = -radius; dy <= radius; ++dy){
                        if(dx*dx + dy*dy <= radius*radius){
                            SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                        }
                    }
                }
            }
            if(plateau[j][i] == 10){ //Création de la couleur du pion avec le cercle (Blanc) + Ajout d'une couronne pour les dames (Rouge)
                int radius = TAILLE_CASE / 2 - 5; //Creation du rayon du cercle selon la taille de la case
                int x = j * TAILLE_CASE + TAILLE_CASE / 2; //Creation des cordonnées du centre de la case
                int y = i * TAILLE_CASE + TAILLE_CASE / 2;
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //Creation de la couleur blanc
                for(int dx = -radius; dx <= radius; ++dx){ //Dessin du cercle avec la bonne couleur selon les coordonnées
                    for(int dy = -radius; dy <= radius; ++dy){
                        if(dx*dx + dy*dy <= radius*radius){
                            SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                        }
                    }
                }
                radius = TAILLE_CASE / 10; //Creation du rayon de la couronne selon la taille de la case
                SDL_SetRenderDrawColor(renderer, 171, 20, 20, 255); //Creation de la couleur de la couronne (Rouge)
                for(int dx = -radius; dx <= radius; ++dx){ //Dessin de la couronne avec la bonne couleur selon les coordonnées
                    for(int dy = -radius; dy <= radius; ++dy){
                        if(dx*dx + dy*dy <= radius*radius){
                            SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                        }
                    }
                }
            }
            if (plateau[j][i] == 20){ //Création de la couleur du pion avec le cercle (Noir) + Ajout d'une couronne pour les dames (Rouge)
                int radius = TAILLE_CASE / 2 - 5; //Creation du rayon du cercle selon la taille de la case
                int x = j * TAILLE_CASE + TAILLE_CASE / 2; //Creation des cordonnées du centre de la case
                int y = i * TAILLE_CASE + TAILLE_CASE / 2;
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Creation de la couleur blanc
                for(int dx = -radius; dx <= radius; ++dx){ //Dessin du cercle avec la bonne couleur selon les coordonnées
                    for(int dy = -radius; dy <= radius; ++dy){
                        if(dx*dx + dy*dy <= radius*radius){
                            SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                        }
                    }
                }
                radius = TAILLE_CASE / 10; //Creation du rayon de la couronne selon la taille de la case
                SDL_SetRenderDrawColor(renderer, 171, 20, 20, 255); //Creation de la couleur de la couronne (Rouge)
                for(int dx = -radius; dx <= radius; ++dx){ //Dessin de la couronne avec la bonne couleur selon les coordonnées
                    for(int dy = -radius; dy <= radius; ++dy){
                        if(dx*dx + dy*dy <= radius*radius){
                            SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                        }
                    }
                }
            }
        }
    }
}

void creation_plateau(int plateau[10][10]){ //Dessin du plateau + appelle du dessin des pions
    SDL_SetRenderDrawColor(renderer, 220, 165, 100, 255); //Deffinition de la couleur foncée des cases
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 229, 204, 255); //Deffinition de la couleur claire des cases
    int x, y;
    for(int i = 0; i < TAILLE_PLATEAU; i++){ //Boucle pour les couleur du plateau, et les coordonnées selon la taille du plateau
        for(int j = 0; j < TAILLE_PLATEAU; j++){
            if((i + j) % 2 == 0){ //Pour les cases paire = couleur claire + Pour les cases impaire = couleur foncé
                x = j * TAILLE_CASE; //Creation des cordonnées de la case
                y = i * TAILLE_CASE;
                SDL_Rect rect = {x, y, TAILLE_CASE, TAILLE_CASE}; //Dessin des cases du plateau
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    dessiner_pions(renderer, plateau); //Appel de la fonction qui dessine les pions
    SDL_RenderPresent(renderer);
}

void distribution_dame(int plateau[10][10]){ //Remplacement d'un pion par une dame s'il attent le camp adverse
    int i;
    for(i=0; i<10; i++){ //Si un pion blanc (1) attend la derniere ligne adverse il se transforme (10 = dame blanc)
        if (plateau[i][0] == 1){
            plateau[i][0] = 10;
        }
    }
    for(i=0; i<10; i++){ //Si un pion blanc (2) attend la derniere ligne adverse il se transforme (20 = dame noir)
        if (plateau[i][9] == 2){
            plateau[i][9] = 20;
        }
    }
}

// Vérifie si les coordonnées (x, y) sont valides sur le plateau
bool coordonnees_valides(int x, int y) {
    return x >= 0 && x < 10 && y >= 0 && y < 10;
}

// Vérifie si une case est occupée par un pion du joueur donné
bool case_occupee_par_joueur(int plateau[10][10], int joueur, int x, int y) {
    return coordonnees_valides(x, y) && (plateau[x][y] == joueur || plateau[x][y] == joueur * 10);
}

// Vérifie si une case est occupée par un pion adverse
bool case_occupee_par_adversaire(int plateau[10][10], int joueur, int x, int y) {
    return coordonnees_valides(x, y) && (plateau[x][y] == alterne(joueur) || plateau[x][y] == alterne(joueur) * 10);
}

// Vérifie si une case est vide
bool case_vide(int plateau[10][10], int x, int y) {
    return coordonnees_valides(x, y) && plateau[x][y] == 0;
}
void liste_coups_possibles_pions_double(int plateau[10][10], int joueur, int tab_coup_possible_double[1000][4], int k[1], int pion[2], int valide[1], int boucle[1]){
    int var_a = 1, var_b = 2;
    if(case_occupee_par_adversaire(plateau, joueur, pion[0] - var_a, pion[1] - var_a) && case_vide(plateau, pion[0] - var_b, pion[1] - var_b) && coordonnees_valides(pion[0] - var_b, pion[1] - var_b)){ //Verif si une prise en devant à gauche est possible
        printf("Il y a une nouvelle prise est possible : (%d, %d) -> (%d, %d)\n", pion[0], pion[1], pion[0] - var_b, pion[1] - var_b);
        tab_coup_possible_double[k[0]][0] = pion[0];
        tab_coup_possible_double[k[0]][1] = pion[1];
        tab_coup_possible_double[k[0]][2] = pion[0] - var_b;
        tab_coup_possible_double[k[0]][3] = pion[1] - var_b;
        k[0] = k[0] + 1;
        valide[0] = 1;
    }
    else{
        boucle[0] = boucle[0] + 50;
    }
    if(case_occupee_par_adversaire(plateau, joueur, pion[0] - var_a, pion[1] + var_a) && case_vide(plateau, pion[0] - var_b, pion[1] + var_b) && coordonnees_valides(pion[0] - var_b, pion[1] + var_b)){ //Verif si une prise en devant à gauche est possible
        printf("Il y a une nouvelle prise est possible : (%d, %d) -> (%d, %d)\n", pion[0], pion[1], pion[0] - var_b, pion[1] + var_b);
        tab_coup_possible_double[k[0]][0] = pion[0];
        tab_coup_possible_double[k[0]][1] = pion[1];
        tab_coup_possible_double[k[0]][2] = pion[0] - var_b;
        tab_coup_possible_double[k[0]][3] = pion[1] + var_b;
        k[0] = k[0] + 1;
        valide[0] = 1;
    }
    else{
        boucle[0] = boucle[0] + 50;
    }
    if(case_occupee_par_adversaire(plateau, joueur, pion[0] + var_a, pion[1] - var_a) && case_vide(plateau, pion[0] + var_b, pion[1] - var_b) && coordonnees_valides(pion[0] + var_b, pion[1] - var_b)){ //Verif si une prise en devant à gauche est possible
        printf("Il y a une nouvelle prise est possible : (%d, %d) -> (%d, %d)\n", pion[0], pion[1], pion[0] + var_b, pion[1] - var_b);
        tab_coup_possible_double[k[0]][0] = pion[0];
        tab_coup_possible_double[k[0]][1] = pion[1];
        tab_coup_possible_double[k[0]][2] = pion[0] + var_b;
        tab_coup_possible_double[k[0]][3] = pion[1] - var_b;
        k[0] = k[0] + 1;
        valide[0] = 1;
    }
    else{
        boucle[0] = boucle[0] + 50;
    }
    if(case_occupee_par_adversaire(plateau, joueur, pion[0] + var_a, pion[1] + var_a) && case_vide(plateau, pion[0] + var_b, pion[1] + var_b) && coordonnees_valides(pion[0] + var_b, pion[1] + var_b)){ //Verif si une prise en devant à gauche est possible
        printf("Il y a une nouvelle prise est possible : (%d, %d) -> (%d, %d)\n", pion[0], pion[1], pion[0] + var_b, pion[1] + var_b);
        tab_coup_possible_double[k[0]][0] = pion[0];
        tab_coup_possible_double[k[0]][1] = pion[1];
        tab_coup_possible_double[k[0]][2] = pion[0] + var_b;
        tab_coup_possible_double[k[0]][3] = pion[1] + var_b;
        k[0] = k[0] + 1;
        valide[0] = 1;
    }
    else{
        boucle[0] = boucle[0] + 50;
    }
}

void liste_coups_possibles_pions_simple(int plateau[10][10], int joueur, int tab_coup_possible[1000][4], int k[1], int i[1], int j[1], int prise[1]){
    int var_a = 1, var_b = 2;
    if(case_occupee_par_adversaire(plateau, joueur, i[0] - var_a, j[0] - var_a) && case_vide(plateau, i[0] - var_b, j[0] - var_b) && coordonnees_valides(i[0] - var_b, j[0] - var_b)){ //Verif si une prise en devant à gauche est possible
        printf("(%d, %d) -> (%d, %d) prise = 1\n", i[0], j[0], i[0] - var_b, j[0] - var_b);
        tab_coup_possible[k[0]][0] = i[0];
        tab_coup_possible[k[0]][1] = j[0];
        tab_coup_possible[k[0]][2] = i[0] - var_b;
        tab_coup_possible[k[0]][3] = j[0] - var_b;
        k[0] = k[0] + 1;
        prise[0] = prise[0] + 1;
    }
    if(case_occupee_par_adversaire(plateau, joueur, i[0] - var_a, j[0] + var_a) && case_vide(plateau, i[0] - var_b, j[0] + var_b) && coordonnees_valides(i[0] - var_b, j[0] + var_b)){ //Verif si une prise en arriere à gauche est possible
        printf("(%d, %d) -> (%d, %d) prise = 1\n", i[0], j[0], i[0] - var_b, j[0] + var_b);
        tab_coup_possible[k[0]][0] = i[0];
        tab_coup_possible[k[0]][1] = j[0];
        tab_coup_possible[k[0]][2] = i[0] - var_b;
        tab_coup_possible[k[0]][3] = j[0] + var_b;
        k[0] = k[0] + 1;
        prise[0] = prise[0] + 1;
    }
    if(case_occupee_par_adversaire(plateau, joueur, i[0] + var_a, j[0] - var_a) && case_vide(plateau, i[0] + var_b, j[0] - var_b) && coordonnees_valides(i[0] + var_b, j[0] - var_b)){ //Verif si une prise en devant à droite est possible
        printf("(%d, %d) -> (%d, %d) prise = 1\n", i[0], j[0], i[0] + var_b, j[0] - var_b);
        tab_coup_possible[k[0]][0] = i[0];
        tab_coup_possible[k[0]][1] = j[0];
        tab_coup_possible[k[0]][2] = i[0] + var_b;
        tab_coup_possible[k[0]][3] = j[0] - var_b;
        k[0] = k[0] + 1;
        prise[0] = prise[0] + 1;
    }
    if(case_occupee_par_adversaire(plateau, joueur, i[0] + var_a, j[0] + var_a) && case_vide(plateau, i[0] + var_b, j[0] + var_b) && coordonnees_valides(i[0] + var_b, j[0] + var_b)){ //Verif si une prise en arriere à droite est possible
        printf("(%d, %d) -> (%d, %d) prise = 1\n", i[0], j[0], i[0] + var_b, j[0] + var_b);
        tab_coup_possible[k[0]][0] = i[0];
        tab_coup_possible[k[0]][1] = j[0];
        tab_coup_possible[k[0]][2] = i[0] + var_b;
        tab_coup_possible[k[0]][3] = j[0] + var_b;
        k[0] = k[0] + 1;
        prise[0] = prise[0] + 1;
    }
}

void liste_coups_possibles_pions(int plateau[10][10], int joueur, int tab_coup_possible[1000][4], int k[1], int prise[1]){
    if(joueur == 1){
        k[0] = 0;
        printf("Liste des pions possibles pour le joueur 1 :\n");
        int i[1];
        for(i[0] = 0; i[0] < TAILLE_PLATEAU; i[0]++){
            int j[1];
            for(j[0] = 0; j[0] < TAILLE_PLATEAU; j[0]++){
                if(plateau[i[0]][j[0]] == joueur){ //Si la pion est blanc
                    //Vérifier les déplacements diagonaux
                    if(case_vide(plateau, i[0] - 1, j[0] - 1) && coordonnees_valides(i[0] - 1, j[0] - 1)){ //Verif si un deplacement en devant à gauche est possible
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 1, j[0] - 1);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 1;
                        tab_coup_possible[k[0]][3] = j[0] - 1;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] + 1, j[0] - 1) && coordonnees_valides(i[0] + 1, j[0] - 1)){ //Verif si un deplacement en devant à droite est possible
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 1, j[0] - 1);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 1;
                        tab_coup_possible[k[0]][3] = j[0] - 1;
                        k[0] = k[0] + 1;
                    }
                    //Vérifier les prises simple et multiples
                    liste_coups_possibles_pions_simple(plateau, joueur, tab_coup_possible, k, i, j, prise);
                }
            }
        }
    }
    if(joueur == 2){
        int k[1];
        k[0] = 0;
        printf("Liste des pions possibles pour le joueur 2 :\n");
        int i[1];
        for(i[0] = 0; i[0] < TAILLE_PLATEAU; i[0]++){
            int j[1];
            for(j[0] = 0; j[0] < TAILLE_PLATEAU; j[0]++){
                if(plateau[i[0]][j[0]] == joueur){ //Si la pion est noir
                    //Vérifier les déplacements diagonaux
                    if(case_vide(plateau, i[0] + 1, j[0] + 1) && coordonnees_valides(i[0] + 1, j[0] + 1)){ //Verif si un deplacement en devant à gauche est possible
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 1, j[0] + 1);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 1;
                        tab_coup_possible[k[0]][3] = j[0] + 1;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] - 1, j[0] + 1) && coordonnees_valides(i[0] - 1, j[0] + 1)){ //Verif si un deplacement en devant à droite est possible
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 1, j[0] + 1);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 1;
                        tab_coup_possible[k[0]][3] = j[0] + 1;
                        k[0] = k[0] + 1;
                    }
                    //Vérifier les prises simple et multiples
                    liste_coups_possibles_pions_simple(plateau, joueur, tab_coup_possible, k, i, j, prise);
                }
            }
        }
    }
}

void liste_coups_possibles_dames(int plateau[10][10], int joueur, int tab_coup_possible[1000][4], int k[1]){
    if(joueur == 1 || 2){
        printf("Liste des dames possibles pour le joueur :\n");
        int i[1];
        for(i[0] = 0; i[0] < TAILLE_PLATEAU; i[0]++){
            int j[1];
            for(j[0] = 0; j[0] < TAILLE_PLATEAU; j[0]++){
                if(plateau[i[0]][j[0]] == joueur * 10){ //Si la pièce est une dame
                    //Verif si un deplacement en avant à gauche est possible
                    if(case_vide(plateau, i[0] - 9, j[0] - 9) && coordonnees_valides(i[0] - 9, j[0] - 9) && case_vide(plateau, i[0] - 8, j[0] - 8) && coordonnees_valides(i[0] - 8, j[0] - 8) && case_vide(plateau, i[0] - 7, j[0] - 7) && coordonnees_valides(i[0] - 7, j[0] - 7) && case_vide(plateau, i[0] - 6, j[0] - 6) && coordonnees_valides(i[0] - 6, j[0] - 6) && case_vide(plateau, i[0] - 5, j[0] - 5) && coordonnees_valides(i[0] - 5, j[0] - 5) && case_vide(plateau, i[0] - 4, j[0] - 4) && coordonnees_valides(i[0] - 4, j[0] - 4) && case_vide(plateau, i[0] - 3, j[0] - 3) && coordonnees_valides(i[0] - 3, j[0] - 3) && case_vide(plateau, i[0] - 2, j[0] - 2) && coordonnees_valides(i[0] - 2, j[0] - 2) && case_vide(plateau, i[0] - 1, j[0] - 1) && coordonnees_valides(i[0] - 1, j[0] - 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 9, j[0] - 9);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 9;
                        tab_coup_possible[k[0]][3] = j[0] - 9;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] - 8, j[0] - 8) && coordonnees_valides(i[0] - 8, j[0] - 8) && case_vide(plateau, i[0] - 7, j[0] - 7) && coordonnees_valides(i[0] - 7, j[0] - 7) && case_vide(plateau, i[0] - 6, j[0] - 6) && coordonnees_valides(i[0] - 6, j[0] - 6) && case_vide(plateau, i[0] - 5, j[0] - 5) && coordonnees_valides(i[0] - 5, j[0] - 5) && case_vide(plateau, i[0] - 4, j[0] - 4) && coordonnees_valides(i[0] - 4, j[0] - 4) && case_vide(plateau, i[0] - 3, j[0] - 3) && coordonnees_valides(i[0] - 3, j[0] - 3) && case_vide(plateau, i[0] - 2, j[0] - 2) && coordonnees_valides(i[0] - 2, j[0] - 2) && case_vide(plateau, i[0] - 1, j[0] - 1) && coordonnees_valides(i[0] - 1, j[0] - 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 8, j[0] - 8);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 8;
                        tab_coup_possible[k[0]][3] = j[0] - 8;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] - 7, j[0] - 7) && coordonnees_valides(i[0] - 7, j[0] - 7) && case_vide(plateau, i[0] - 6, j[0] - 6) && coordonnees_valides(i[0] - 6, j[0] - 6) && case_vide(plateau, i[0] - 5, j[0] - 5) && coordonnees_valides(i[0] - 5, j[0] - 5) && case_vide(plateau, i[0] - 4, j[0] - 4) && coordonnees_valides(i[0] - 4, j[0] - 4) && case_vide(plateau, i[0] - 3, j[0] - 3) && coordonnees_valides(i[0] - 3, j[0] - 3) && case_vide(plateau, i[0] - 2, j[0] - 2) && coordonnees_valides(i[0] - 2, j[0] - 2) && case_vide(plateau, i[0] - 1, j[0] - 1) && coordonnees_valides(i[0] - 1, j[0] - 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 7, j[0] - 7);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 7;
                        tab_coup_possible[k[0]][3] = j[0] - 7;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] - 6, j[0] - 6) && coordonnees_valides(i[0] - 6, j[0] - 6) && case_vide(plateau, i[0] - 5, j[0] - 5) && coordonnees_valides(i[0] - 5, j[0] - 5) && case_vide(plateau, i[0] - 4, j[0] - 4) && coordonnees_valides(i[0] - 4, j[0] - 4) && case_vide(plateau, i[0] - 3, j[0] - 3) && coordonnees_valides(i[0] - 3, j[0] - 3) && case_vide(plateau, i[0] - 2, j[0] - 2) && coordonnees_valides(i[0] - 2, j[0] - 2) && case_vide(plateau, i[0] - 1, j[0] - 1) && coordonnees_valides(i[0] - 1, j[0] - 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 6, j[0] - 6);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 6;
                        tab_coup_possible[k[0]][3] = j[0] - 6;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] - 5, j[0] - 5) && coordonnees_valides(i[0] - 5, j[0] - 5) && case_vide(plateau, i[0] - 4, j[0] - 4) && coordonnees_valides(i[0] - 4, j[0] - 4) && case_vide(plateau, i[0] - 3, j[0] - 3) && coordonnees_valides(i[0] - 3, j[0] - 3) && case_vide(plateau, i[0] - 2, j[0] - 2) && coordonnees_valides(i[0] - 2, j[0] - 2) && case_vide(plateau, i[0] - 1, j[0] - 1) && coordonnees_valides(i[0] - 1, j[0] - 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 5, j[0] - 5);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 5;
                        tab_coup_possible[k[0]][3] = j[0] - 5;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] - 4, j[0] - 4) && coordonnees_valides(i[0] - 4, j[0] - 4) && case_vide(plateau, i[0] - 3, j[0] - 3) && coordonnees_valides(i[0] - 3, j[0] - 3) && case_vide(plateau, i[0] - 2, j[0] - 2) && coordonnees_valides(i[0] - 2, j[0] - 2) && case_vide(plateau, i[0] - 1, j[0] - 1) && coordonnees_valides(i[0] - 1, j[0] - 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 4, j[0] - 4);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 4;
                        tab_coup_possible[k[0]][3] = j[0] - 4;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] - 3, j[0] - 3) && coordonnees_valides(i[0] - 3, j[0] - 3) && case_vide(plateau, i[0] - 2, j[0] - 2) && coordonnees_valides(i[0] - 2, j[0] - 2) && case_vide(plateau, i[0] - 1, j[0] - 1) && coordonnees_valides(i[0] - 1, j[0] - 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 3, j[0] - 3);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 3;
                        tab_coup_possible[k[0]][3] = j[0] - 3;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] - 2, j[0] - 2) && coordonnees_valides(i[0] - 2, j[0] - 2) && case_vide(plateau, i[0] - 1, j[0] - 1) && coordonnees_valides(i[0] - 1, j[0] - 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 2, j[0] - 2);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 2;
                        tab_coup_possible[k[0]][3] = j[0] - 2;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] - 1, j[0] - 1) && coordonnees_valides(i[0] - 1, j[0] - 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 1, j[0] - 1);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 1;
                        tab_coup_possible[k[0]][3] = j[0] - 1;
                        k[0] = k[0] + 1;
                    }
                    //Verif si un deplacement en avant à droite est possible
                    if(case_vide(plateau, i[0] + 9, j[0] - 9) && coordonnees_valides(i[0] + 9, j[0] - 9) && case_vide(plateau, i[0] + 8, j[0] - 8) && coordonnees_valides(i[0] + 8, j[0] - 8) && case_vide(plateau, i[0] + 7, j[0] - 7) && coordonnees_valides(i[0] + 7, j[0] - 7) && case_vide(plateau, i[0] + 6, j[0] - 6) && coordonnees_valides(i[0] + 6, j[0] - 6) && case_vide(plateau, i[0] + 5, j[0] - 5) && coordonnees_valides(i[0] + 5, j[0] - 5) && case_vide(plateau, i[0] + 4, j[0] - 4) && coordonnees_valides(i[0] + 4, j[0] - 4) && case_vide(plateau, i[0] + 3, j[0] - 3) && coordonnees_valides(i[0] + 3, j[0] - 3) && case_vide(plateau, i[0] + 2, j[0] - 2) && coordonnees_valides(i[0] + 2, j[0] - 2) && case_vide(plateau, i[0] + 1, j[0] - 1) && coordonnees_valides(i[0] + 1, j[0] - 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 9, j[0] - 9);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 9;
                        tab_coup_possible[k[0]][3] = j[0] - 9;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] + 8, j[0] - 8) && coordonnees_valides(i[0] + 8, j[0] - 8) && case_vide(plateau, i[0] + 7, j[0] - 7) && coordonnees_valides(i[0] + 7, j[0] - 7) && case_vide(plateau, i[0] + 6, j[0] - 6) && coordonnees_valides(i[0] + 6, j[0] - 6) && case_vide(plateau, i[0] + 5, j[0] - 5) && coordonnees_valides(i[0] + 5, j[0] - 5) && case_vide(plateau, i[0] + 4, j[0] - 4) && coordonnees_valides(i[0] + 4, j[0] - 4) && case_vide(plateau, i[0] + 3, j[0] - 3) && coordonnees_valides(i[0] + 3, j[0] - 3) && case_vide(plateau, i[0] + 2, j[0] - 2) && coordonnees_valides(i[0] + 2, j[0] - 2) && case_vide(plateau, i[0] + 1, j[0] - 1) && coordonnees_valides(i[0] + 1, j[0] - 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 8, j[0] - 8);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 8;
                        tab_coup_possible[k[0]][3] = j[0] - 8;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] + 7, j[0] - 7) && coordonnees_valides(i[0] + 7, j[0] - 7) && case_vide(plateau, i[0] + 6, j[0] - 6) && coordonnees_valides(i[0] + 6, j[0] - 6) && case_vide(plateau, i[0] + 5, j[0] - 5) && coordonnees_valides(i[0] + 5, j[0] - 5) && case_vide(plateau, i[0] + 4, j[0] - 4) && coordonnees_valides(i[0] + 4, j[0] - 4) && case_vide(plateau, i[0] + 3, j[0] - 3) && coordonnees_valides(i[0] + 3, j[0] - 3) && case_vide(plateau, i[0] + 2, j[0] - 2) && coordonnees_valides(i[0] + 2, j[0] - 2) && case_vide(plateau, i[0] + 1, j[0] - 1) && coordonnees_valides(i[0] + 1, j[0] - 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 7, j[0] - 7);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 7;
                        tab_coup_possible[k[0]][3] = j[0] - 7;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] + 6, j[0] - 6) && coordonnees_valides(i[0] + 6, j[0] - 6) && case_vide(plateau, i[0] + 5, j[0] - 5) && coordonnees_valides(i[0] + 5, j[0] - 5) && case_vide(plateau, i[0] + 4, j[0] - 4) && coordonnees_valides(i[0] + 4, j[0] - 4) && case_vide(plateau, i[0] + 3, j[0] - 3) && coordonnees_valides(i[0] + 3, j[0] - 3) && case_vide(plateau, i[0] + 2, j[0] - 2) && coordonnees_valides(i[0] + 2, j[0] - 2) && case_vide(plateau, i[0] + 1, j[0] - 1) && coordonnees_valides(i[0] + 1, j[0] - 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 6, j[0] - 6);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 6;
                        tab_coup_possible[k[0]][3] = j[0] - 6;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] + 5, j[0] - 5) && coordonnees_valides(i[0] + 5, j[0] - 5) && case_vide(plateau, i[0] + 4, j[0] - 4) && coordonnees_valides(i[0] + 4, j[0] - 4) && case_vide(plateau, i[0] + 3, j[0] - 3) && coordonnees_valides(i[0] + 3, j[0] - 3) && case_vide(plateau, i[0] + 2, j[0] - 2) && coordonnees_valides(i[0] + 2, j[0] - 2) && case_vide(plateau, i[0] + 1, j[0] - 1) && coordonnees_valides(i[0] + 1, j[0] - 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 5, j[0] - 5);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 5;
                        tab_coup_possible[k[0]][3] = j[0] - 5;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] + 4, j[0] - 4) && coordonnees_valides(i[0] + 4, j[0] - 4) && case_vide(plateau, i[0] + 3, j[0] - 3) && coordonnees_valides(i[0] + 3, j[0] - 3) && case_vide(plateau, i[0] + 2, j[0] - 2) && coordonnees_valides(i[0] + 2, j[0] - 2) && case_vide(plateau, i[0] + 1, j[0] - 1) && coordonnees_valides(i[0] + 1, j[0] - 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 4, j[0] - 4);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 4;
                        tab_coup_possible[k[0]][3] = j[0] - 4;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] + 3, j[0] - 3) && coordonnees_valides(i[0] + 3, j[0] - 3) && case_vide(plateau, i[0] + 2, j[0] - 2) && coordonnees_valides(i[0] + 2, j[0] - 2) && case_vide(plateau, i[0] + 1, j[0] - 1) && coordonnees_valides(i[0] + 1, j[0] - 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 3, j[0] - 3);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 3;
                        tab_coup_possible[k[0]][3] = j[0] - 3;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] + 2, j[0] - 2) && coordonnees_valides(i[0] + 2, j[0] - 2) && case_vide(plateau, i[0] + 1, j[0] - 1) && coordonnees_valides(i[0] + 1, j[0] - 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 2, j[0] - 2);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 2;
                        tab_coup_possible[k[0]][3] = j[0] - 2;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] + 1, j[0] - 1) && coordonnees_valides(i[0] + 1, j[0] - 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 1, j[0] - 1);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 1;
                        tab_coup_possible[k[0]][3] = j[0] - 1;
                        k[0] = k[0] + 1;
                    }
                    //Verif si un deplacement en arriere à gauche est possible
                    if(case_vide(plateau, i[0] - 9, j[0] + 9) && coordonnees_valides(i[0] - 9, j[0] + 9) && case_vide(plateau, i[0] - 8, j[0] + 8) && coordonnees_valides(i[0] - 8, j[0] + 8) && case_vide(plateau, i[0] - 7, j[0] + 7) && coordonnees_valides(i[0] - 7, j[0] + 7) && case_vide(plateau, i[0] - 6, j[0] + 6) && coordonnees_valides(i[0] - 6, j[0] + 6) && case_vide(plateau, i[0] - 5, j[0] + 5) && coordonnees_valides(i[0] - 5, j[0] + 5) && case_vide(plateau, i[0] - 4, j[0] + 4) && coordonnees_valides(i[0] - 4, j[0] + 4) && case_vide(plateau, i[0] - 3, j[0] + 3) && coordonnees_valides(i[0] - 3, j[0] + 3) && case_vide(plateau, i[0] - 2, j[0] + 2) && coordonnees_valides(i[0] - 2, j[0] + 2) && case_vide(plateau, i[0] - 1, j[0] + 1) && coordonnees_valides(i[0] - 1, j[0] + 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 9, j[0] + 9);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 9;
                        tab_coup_possible[k[0]][3] = j[0] + 9;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] - 8, j[0] + 8) && coordonnees_valides(i[0] - 8, j[0] + 8) && case_vide(plateau, i[0] - 7, j[0] + 7) && coordonnees_valides(i[0] - 7, j[0] + 7) && case_vide(plateau, i[0] - 6, j[0] + 6) && coordonnees_valides(i[0] - 6, j[0] + 6) && case_vide(plateau, i[0] - 5, j[0] + 5) && coordonnees_valides(i[0] - 5, j[0] + 5) && case_vide(plateau, i[0] - 4, j[0] + 4) && coordonnees_valides(i[0] - 4, j[0] + 4) && case_vide(plateau, i[0] - 3, j[0] + 3) && coordonnees_valides(i[0] - 3, j[0] + 3) && case_vide(plateau, i[0] - 2, j[0] + 2) && coordonnees_valides(i[0] - 2, j[0] + 2) && case_vide(plateau, i[0] - 1, j[0] + 1) && coordonnees_valides(i[0] - 1, j[0] + 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 8, j[0] + 8);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 8;
                        tab_coup_possible[k[0]][3] = j[0] + 8;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] - 7, j[0] + 7) && coordonnees_valides(i[0] - 7, j[0] + 7) && case_vide(plateau, i[0] - 6, j[0] + 6) && coordonnees_valides(i[0] - 6, j[0] + 6) && case_vide(plateau, i[0] - 5, j[0] + 5) && coordonnees_valides(i[0] - 5, j[0] + 5) && case_vide(plateau, i[0] - 4, j[0] + 4) && coordonnees_valides(i[0] - 4, j[0] + 4) && case_vide(plateau, i[0] - 3, j[0] + 3) && coordonnees_valides(i[0] - 3, j[0] + 3) && case_vide(plateau, i[0] - 2, j[0] + 2) && coordonnees_valides(i[0] - 2, j[0] + 2) && case_vide(plateau, i[0] - 1, j[0] + 1) && coordonnees_valides(i[0] - 1, j[0] + 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 7, j[0] + 7);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 7;
                        tab_coup_possible[k[0]][3] = j[0] + 7;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] - 6, j[0] + 6) && coordonnees_valides(i[0] - 6, j[0] + 6) && case_vide(plateau, i[0] - 5, j[0] + 5) && coordonnees_valides(i[0] - 5, j[0] + 5) && case_vide(plateau, i[0] - 4, j[0] + 4) && coordonnees_valides(i[0] - 4, j[0] + 4) && case_vide(plateau, i[0] - 3, j[0] + 3) && coordonnees_valides(i[0] - 3, j[0] + 3) && case_vide(plateau, i[0] - 2, j[0] + 2) && coordonnees_valides(i[0] - 2, j[0] + 2) && case_vide(plateau, i[0] - 1, j[0] + 1) && coordonnees_valides(i[0] - 1, j[0] + 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 6, j[0] + 6);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 6;
                        tab_coup_possible[k[0]][3] = j[0] + 6;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] - 5, j[0] + 5) && coordonnees_valides(i[0] - 5, j[0] + 5) && case_vide(plateau, i[0] - 4, j[0] + 4) && coordonnees_valides(i[0] - 4, j[0] + 4) && case_vide(plateau, i[0] - 3, j[0] + 3) && coordonnees_valides(i[0] - 3, j[0] + 3) && case_vide(plateau, i[0] - 2, j[0] + 2) && coordonnees_valides(i[0] - 2, j[0] + 2) && case_vide(plateau, i[0] - 1, j[0] + 1) && coordonnees_valides(i[0] - 1, j[0] + 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 5, j[0] + 5);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 5;
                        tab_coup_possible[k[0]][3] = j[0] + 5;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] - 4, j[0] + 4) && coordonnees_valides(i[0] - 4, j[0] + 4) && case_vide(plateau, i[0] - 3, j[0] + 3) && coordonnees_valides(i[0] - 3, j[0] + 3) && case_vide(plateau, i[0] - 2, j[0] + 2) && coordonnees_valides(i[0] - 2, j[0] + 2) && case_vide(plateau, i[0] - 1, j[0] + 1) && coordonnees_valides(i[0] - 1, j[0] + 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 4, j[0] + 4);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 4;
                        tab_coup_possible[k[0]][3] = j[0] + 4;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] - 3, j[0] + 3) && coordonnees_valides(i[0] - 3, j[0] + 3) && case_vide(plateau, i[0] - 2, j[0] + 2) && coordonnees_valides(i[0] - 2, j[0] + 2) && case_vide(plateau, i[0] - 1, j[0] + 1) && coordonnees_valides(i[0] - 1, j[0] + 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 3, j[0] + 3);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 3;
                        tab_coup_possible[k[0]][3] = j[0] + 3;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] - 2, j[0] + 2) && coordonnees_valides(i[0] - 2, j[0] + 2) && case_vide(plateau, i[0] - 1, j[0] + 1) && coordonnees_valides(i[0] - 1, j[0] + 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 2, j[0] + 2);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 2;
                        tab_coup_possible[k[0]][3] = j[0] + 2;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] - 1, j[0] + 1) && coordonnees_valides(i[0] - 1, j[0] + 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] - 1, j[0] + 1);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] - 1;
                        tab_coup_possible[k[0]][3] = j[0] + 1;
                        k[0] = k[0] + 1;
                    }
                    //Verif si un deplacement en arriere à droite est possible
                    if(case_vide(plateau, i[0] + 9, j[0] + 9) && coordonnees_valides(i[0] + 9, j[0] + 9) && case_vide(plateau, i[0] + 8, j[0] + 8) && coordonnees_valides(i[0] + 8, j[0] + 8) && case_vide(plateau, i[0] + 7, j[0] + 7) && coordonnees_valides(i[0] + 7, j[0] + 7) && case_vide(plateau, i[0] + 6, j[0] + 6) && coordonnees_valides(i[0] + 6, j[0] + 6) && case_vide(plateau, i[0] + 5, j[0] + 5) && coordonnees_valides(i[0] + 5, j[0] + 5) && case_vide(plateau, i[0] + 4, j[0] + 4) && coordonnees_valides(i[0] + 4, j[0] + 4) && case_vide(plateau, i[0] + 3, j[0] + 3) && coordonnees_valides(i[0] + 3, j[0] + 3) && case_vide(plateau, i[0] + 2, j[0] + 2) && coordonnees_valides(i[0] + 2, j[0] + 2) && case_vide(plateau, i[0] + 1, j[0] + 1) && coordonnees_valides(i[0] + 1, j[0] + 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 9, j[0] + 9);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 9;
                        tab_coup_possible[k[0]][3] = j[0] + 9;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] + 8, j[0] + 8) && coordonnees_valides(i[0] + 8, j[0] + 8) && case_vide(plateau, i[0] + 7, j[0] + 7) && coordonnees_valides(i[0] + 7, j[0] + 7) && case_vide(plateau, i[0] + 6, j[0] + 6) && coordonnees_valides(i[0] + 6, j[0] + 6) && case_vide(plateau, i[0] + 5, j[0] + 5) && coordonnees_valides(i[0] + 5, j[0] + 5) && case_vide(plateau, i[0] + 4, j[0] + 4) && coordonnees_valides(i[0] + 4, j[0] + 4) && case_vide(plateau, i[0] + 3, j[0] + 3) && coordonnees_valides(i[0] + 3, j[0] + 3) && case_vide(plateau, i[0] + 2, j[0] + 2) && coordonnees_valides(i[0] + 2, j[0] + 2) && case_vide(plateau, i[0] + 1, j[0] + 1) && coordonnees_valides(i[0] + 1, j[0] + 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 8, j[0] + 8);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 8;
                        tab_coup_possible[k[0]][3] = j[0] + 8;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] + 7, j[0] + 7) && coordonnees_valides(i[0] + 7, j[0] + 7) && case_vide(plateau, i[0] + 6, j[0] + 6) && coordonnees_valides(i[0] + 6, j[0] + 6) && case_vide(plateau, i[0] + 5, j[0] + 5) && coordonnees_valides(i[0] + 5, j[0] + 5) && case_vide(plateau, i[0] + 4, j[0] + 4) && coordonnees_valides(i[0] + 4, j[0] + 4) && case_vide(plateau, i[0] + 3, j[0] + 3) && coordonnees_valides(i[0] + 3, j[0] + 3) && case_vide(plateau, i[0] + 2, j[0] + 2) && coordonnees_valides(i[0] + 2, j[0] + 2) && case_vide(plateau, i[0] + 1, j[0] + 1) && coordonnees_valides(i[0] + 1, j[0] + 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 7, j[0] + 7);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 7;
                        tab_coup_possible[k[0]][3] = j[0] + 7;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] + 6, j[0] + 6) && coordonnees_valides(i[0] + 6, j[0] + 6) && case_vide(plateau, i[0] + 5, j[0] + 5) && coordonnees_valides(i[0] + 5, j[0] + 5) && case_vide(plateau, i[0] + 4, j[0] + 4) && coordonnees_valides(i[0] + 4, j[0] + 4) && case_vide(plateau, i[0] + 3, j[0] + 3) && coordonnees_valides(i[0] + 3, j[0] + 3) && case_vide(plateau, i[0] + 2, j[0] + 2) && coordonnees_valides(i[0] + 2, j[0] + 2) && case_vide(plateau, i[0] + 1, j[0] + 1) && coordonnees_valides(i[0] + 1, j[0] + 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 6, j[0] + 6);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 6;
                        tab_coup_possible[k[0]][3] = j[0] + 6;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] + 5, j[0] + 5) && coordonnees_valides(i[0] + 5, j[0] + 5) && case_vide(plateau, i[0] + 4, j[0] + 4) && coordonnees_valides(i[0] + 4, j[0] + 4) && case_vide(plateau, i[0] + 3, j[0] + 3) && coordonnees_valides(i[0] + 3, j[0] + 3) && case_vide(plateau, i[0] + 2, j[0] + 2) && coordonnees_valides(i[0] + 2, j[0] + 2) && case_vide(plateau, i[0] + 1, j[0] + 1) && coordonnees_valides(i[0] + 1, j[0] + 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 5, j[0] + 5);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 5;
                        tab_coup_possible[k[0]][3] = j[0] + 5;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] + 4, j[0] + 4) && coordonnees_valides(i[0] + 4, j[0] + 4) && case_vide(plateau, i[0] + 3, j[0] + 3) && coordonnees_valides(i[0] + 3, j[0] + 3) && case_vide(plateau, i[0] + 2, j[0] + 2) && coordonnees_valides(i[0] + 2, j[0] + 2) && case_vide(plateau, i[0] + 1, j[0] + 1) && coordonnees_valides(i[0] + 1, j[0] + 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 4, j[0] + 4);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 4;
                        tab_coup_possible[k[0]][3] = j[0] + 4;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] + 3, j[0] + 3) && coordonnees_valides(i[0] + 3, j[0] + 3) && case_vide(plateau, i[0] + 2, j[0] + 2) && coordonnees_valides(i[0] + 2, j[0] + 2) && case_vide(plateau, i[0] + 1, j[0] + 1) && coordonnees_valides(i[0] + 1, j[0] + 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 3, j[0] + 3);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 3;
                        tab_coup_possible[k[0]][3] = j[0] + 3;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] + 2, j[0] + 2) && coordonnees_valides(i[0] + 2, j[0] + 2) && case_vide(plateau, i[0] + 1, j[0] + 1) && coordonnees_valides(i[0] + 1, j[0] + 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 2, j[0] + 2);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 2;
                        tab_coup_possible[k[0]][3] = j[0] + 2;
                        k[0] = k[0] + 1;
                    }
                    if(case_vide(plateau, i[0] + 1, j[0] + 1) && coordonnees_valides(i[0] + 1, j[0] + 1)){
                        printf("(%d, %d) -> (%d, %d)\n", i[0], j[0], i[0] + 1, j[0] + 1);
                        tab_coup_possible[k[0]][0] = i[0];
                        tab_coup_possible[k[0]][1] = j[0];
                        tab_coup_possible[k[0]][2] = i[0] + 1;
                        tab_coup_possible[k[0]][3] = j[0] + 1;
                        k[0] = k[0] + 1;
                    }
                    //Vérifier les prises simple et multiples

                }   
            }
        }
    }
}

void replacement_valeurDansTableau(int tableau[1000][4], int taille){ //Mettre le tableau avec des valeur impossible
    for(int i = 0; i < taille; i++){
        tableau[i][0] = -1;
        tableau[i][1] = -1;
        tableau[i][2] = -1;
        tableau[i][3] = -1;
    }
}

int valeurDansTableau(int tableau[1000][4], int taille, int pion[2], int coup[2]){ //Fonction pour vérifier si une valeur est présente dans un tableau
    for(int i = 0; i < taille; i++){
        if(tableau[i][0] == pion[0] && tableau[i][1] == pion[1] && tableau[i][2] == coup[0] && tableau[i][3] == coup[1]){
            replacement_valeurDansTableau(tableau, taille);
            return 1; //La valeur est trouvé dans le tableau
        }
    }
    return 0; //La valeur n'est pas trouvé dans le tableau
}

void jouer(int joueur, int plateau[10][10]){
    int pion_restant[2]; //Initialisation du score
    int pion[2]; //Coordonnées du pion sélectionné
    int coup[2]; //Coordonnées du coup souhaité
    int tab_coup_possible[1000][4]; //Tableau des different coup possible de jouer
    int tab_coup_possible_double[1000][4]; //Tableau des different coup possible de jouer
    int coup_valide; //Variable pour la boucle d'obligation de joueur
    int k[1];
    int valide[1];
    int boucle[1];
    int prise[1];
    prise[0] = 0;
    creation_plateau(plateau);

    while(1){
        coup_valide = 0;
        afficher_plateau(plateau);
        afficher_plateau_char(plateau); //Afficher le plateau + le score et commencer à jouer
        score(plateau, pion_restant);
        printf("Pions blancs restants: %d\n", pion_restant[0]);
        printf("Pions noirs restants: %d\n", pion_restant[1]);

        liste_coups_possibles_pions(plateau, joueur, tab_coup_possible, k, prise);
        liste_coups_possibles_dames(plateau, joueur, tab_coup_possible, k);

        coup_valide = 0;
        while(coup_valide != 1){
            // Demander à l'utilisateur de saisir les coordonnées de départ et d'arrivée
            printf("Joueur %d, veuillez sélectionner un pion (colonne ligne) et un coup d'arrivé: ", joueur); //Sélectionner un pion à déplacer
            scanf("%d %d %d %d", &pion[0], &pion[1], &coup[0], &coup[1]);

            // Vérifier si les coordonnées de départ et d'arrivée correspondent à l'un des coups possibles
            if(valeurDansTableau(tab_coup_possible, 1000, pion, coup) == 1) {
                coup_valide = 1;
                //Déplacer la dame sur le plateau
                deplacer_pion(plateau, pion, coup);
            }
            else{
                printf("Coup invalide. Veuillez sélectionner un coup valide.\n");
            }
        }

        if(prise[0] != 0){
            boucle[0] = 0;
            while(boucle[0] != 200){
                valide[0] = 0;
                k[0] = 0;
                liste_coups_possibles_pions_double(plateau, joueur, tab_coup_possible, k, coup, valide, boucle);
                if(valide[0] != 0){
                    coup_valide = 0;
                    while(coup_valide != 1){
                        // Demander à l'utilisateur de saisir les coordonnées de départ et d'arrivée
                        printf("Joueur %d, veuillez sélectionner un pion (colonne ligne): ", joueur); //Sélectionner un pion à déplacer
                        scanf("%d %d %d %d", &pion[0], &pion[1], &coup[0], &coup[1]);

                        // Vérifier si les coordonnées de départ et d'arrivée correspondent à l'un des coups possibles
                        if(valeurDansTableau(tab_coup_possible, 1000, pion, coup) == 1) {
                            coup_valide = 1;
                            //Déplacer la dame sur le plateau
                            deplacer_pion(plateau, pion, coup);
                        }
                        else{
                            printf("Coup invalide. Veuillez sélectionner un coup valide.\n");
                        }
                    }
                }
                valide[0] = 0;
                k[0] = 0;
                boucle[0] = 0;
                liste_coups_possibles_pions_double(plateau, joueur, tab_coup_possible, k, coup, valide, boucle);
                printf("%d\n", valide[0]);
                printf("%d\n", boucle[0]);
            }
        }

        joueur = alterne(joueur);
        distribution_dame(plateau);
        creation_plateau(plateau);
        sauvegarder_plateau(plateau, "fichier_plateau.txt");
        sauvegarder_joueur("fichier_joueur.txt", joueur);
        if (pion_restant[0] == 0 || pion_restant[1] == 0) {
            printf("La partie est terminée. Le joueur %d a gagné !\n", (pion_restant[0] == 0) ? 2 : 1);
            remplissage(plateau);
            sauvegarder_plateau(plateau, "fichier_plateau.txt");
            break;
        }
    }
}

void menu_sauvegarde(int plateau[10][10]){ //Menu de selection de jeu de dames (Partie/Sauvegarde/Quitter)
    int rep, joueur;
    do{
        printf("-Tapez 1 pour commencer une nouvelle partie\n");
        printf("-Tapez 2 pour jouer à la partie sauvegardé\n");
        printf("-Tapez 3 pour quitter le menu\n");
        scanf("%i",&rep);
        printf("\n");
    }while(rep<1||rep>3);
    switch(rep){
        case 1 :
            joueur = 1;
            remplissage(plateau);
            jouer(joueur, plateau);
            break;
        case 2 :
            joueur = charger_joueur(joueur, "fichier_joueur.txt");
            charger_plateau(plateau, "fichier_plateau.txt");
            jouer(joueur, plateau);
            break;
        case 3 :
            break;
    printf("\n");
    }
}

void menu_principal(int plateau[10][10]){ //Menu de selection de jeu de dames (Regles/Partie/Quitter)
    int rep, fin = 0;
    do{
    do{
        printf("Bienvenue sur le menu du Jeu de Dames, veuillez choisir l'action que vous souhaitez réaliser :\n\n");
        printf("-Tapez 1 pour afficher les regles du jeu\n");
        printf("-Tapez 2 pour commencer une partie\n");
        printf("-Tapez 3 pour quitter le programme\n");
        scanf("%i",&rep);
        printf("\n");
    }while(rep<1||rep>3);
    switch(rep){
        case 1 :
            remplissage(plateau);
            affiche_regles(plateau);
            break;
        case 2 :
            menu_sauvegarde(plateau);
            break;
        case 3 :
            printf("Vous avez quitte le programme.");
            fin = 1;
    printf("\n");
    }
    }while(fin != 1);
}

int main(int argc, char* args[]){
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Jeu de dames", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LARGEUR_FENETRE, LONGUEUR_FENETRE, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int plateau[10][10]; //O --> vide, 1 --> blanc, 2 --> noir, 1O --> dame blanc, 2O --> dame noir
    menu_principal(plateau);

    SDL_Delay(2000);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}