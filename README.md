# ♟️ Jeu de Dames en C (SDL2)

Jeu de dames sur damier **10 × 10** écrit en langage C, avec un affichage graphique du plateau via **SDL2** et des coups saisis dans la console.

Projet réalisé en première année de cycle pré-ingénieur à **UniLaSalle Amiens** (module *Algorithme I1*, 2024).

---

## 🎯 Fonctionnalités

- Menu principal : afficher les règles, lancer une partie, quitter
- Nouvelle partie ou **reprise d'une partie sauvegardée**
- Plateau 10 × 10 affiché dans une fenêtre SDL2 (pions blancs et noirs, dames marquées d'une couronne rouge)
- Affichage du plateau en mode texte dans la console, avec le nombre de pions restants
- Liste des coups possibles calculée à chaque tour
- Prise obligatoire et prises multiples
- Promotion des pions en dames
- Sauvegarde automatique du plateau et du joueur courant après chaque tour

---

## 🚀 Compilation et lancement

### Prérequis

- Un compilateur C (GCC)
- La bibliothèque **SDL2** (développement)
  - Debian / Ubuntu : `sudo apt install libsdl2-dev`
  - macOS : `brew install sdl2`
  - Windows : MinGW + [SDL2](https://github.com/libsdl-org/SDL/releases) (le fichier `SDL2.dll` doit se trouver à côté de l'exécutable)

### Compilation

Avec le Makefile fourni :

```bash
make
```

Ou à la main sous Linux / macOS :

```bash
gcc -Wall -o dames main.c $(sdl2-config --cflags --libs) -lm
./dames
```

---

## 🕹️ Comment jouer

Le jeu se pilote **dans la console** ; la fenêtre SDL2 affiche le plateau.

| Valeur sur le plateau | Pièce |
|:---:|---|
| `1` / `o` | Pion blanc (joueur 1) |
| `2` / `x` | Pion noir (joueur 2) |
| `10` / `O` | Dame blanche |
| `20` / `X` | Dame noire |
| `0` | Case vide |

À chaque tour, saisis **quatre nombres** séparés par des espaces :

```
colonne_départ ligne_départ colonne_arrivée ligne_arrivée
```

Les coordonnées vont de **0 à 9**, comme sur les repères affichés autour du plateau dans la console.

Le programme affiche la liste des coups possibles ; si ton coup n'en fait pas partie, il est refusé.

### Règles en bref

- Les blancs commencent.
- Un pion avance en diagonale ; une dame se déplace sur plusieurs cases en diagonale.
- La prise est **obligatoire**, en avant comme en arrière, et peut s'enchaîner.
- Un pion qui atteint la dernière rangée devient dame.
- La partie est gagnée quand l'adversaire n'a plus de pièces.

Les règles complètes sont aussi disponibles depuis le menu du jeu.

---

## 💾 Sauvegarde

La partie en cours est enregistrée automatiquement dans deux fichiers texte créés à côté de l'exécutable :

- `fichier_plateau.txt` : l'état du plateau
- `fichier_joueur.txt` : le joueur dont c'est le tour

Le menu « jouer à la partie sauvegardée » recharge ces deux fichiers.

---

## 🗂️ Structure du code

| Fonction | Rôle |
|---|---|
| `remplissage()` | Place les pions en position de départ |
| `afficher_plateau()` / `afficher_plateau_char()` | Affichage console (valeurs / symboles) |
| `creation_plateau()` / `dessiner_pions()` | Dessin du damier et des pions avec SDL2 |
| `liste_coups_possibles_pions()` / `..._dames()` | Calcul des coups et des prises possibles |
| `deplacer_pion()` / `deplacer_dame()` | Déplacement et retrait des pièces prises |
| `jouer()` | Boucle de jeu principale |
| `score()` | Comptage des pièces restantes |
| `sauvegarder_*()` / `charger_*()` | Sauvegarde et chargement des fichiers texte |
| `menu_principal()` / `menu_sauvegarde()` | Menus |

---

## ⚠️ Limites connues

C'est un projet de première année, avec quelques imperfections :

- la saisie des coups se fait au clavier, la souris n'est pas utilisée ;
- la fenêtre SDL2 ne gère pas d'événements (elle ne se ferme pas à la main pendant une partie) ;
- l'affichage console contient des messages de débogage et la liste des coups possibles ;
- la fin de partie n'est détectée que lorsqu'un camp n'a plus de pièces (pas de détection de blocage ni des cas d'égalité) ;
- charger une partie sans fichier de sauvegarde existant n'est pas géré proprement ;
- une saisie non numérique peut bloquer la lecture (`scanf`).

---

## 🔭 Pistes d'amélioration

- Jeu à la souris directement dans la fenêtre SDL2
- Gestion complète des fins de partie (blocage, égalité)
- Découpage du code en plusieurs fichiers
- Mode solo contre une IA
