#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOM 50
#define MAX_CULTURE 30


typedef struct {
    char espece[MAX_NOM];
    int nombre;
} Animal;

typedef struct {
    float superficie;
    char localisation[MAX_NOM];
    char typeCulture[MAX_CULTURE];
} Terrain;


typedef struct Adherent {
    char nom[MAX_NOM];
    int age;
    Terrain terrain;
    Animal animal;
    struct Adherent *suivant;
} Adherent;

Adherent *ajouterAdherent(Adherent *tete) {
    Adherent *nouvelAdherent = (Adherent *)malloc(sizeof(Adherent));
    if (nouvelAdherent == NULL) {
        printf("Erreur d'allocation mémoire.\n");
        return tete;
    }

    printf("Nom : ");
    scanf(" %49[^\n]", nouvelAdherent->nom);

    printf("Age : ");
    scanf("%d", &nouvelAdherent->age);

    printf("Superficie du terrain (en hectares) : ");
    scanf("%f", &nouvelAdherent->terrain.superficie);

    printf("Localisation du terrain : ");
    scanf(" %49[^\n]", nouvelAdherent->terrain.localisation);

    printf("Type de culture : ");
    scanf(" %29[^\n]", nouvelAdherent->terrain.typeCulture);

    printf("Espèce animale : ");
    scanf(" %49[^\n]", nouvelAdherent->animal.espece);

    printf("Nombre d'animaux : ");
    scanf("%d", &nouvelAdherent->animal.nombre);

    nouvelAdherent->suivant = tete;
    printf("Adhérent ajouté avec succès !\n");

    return nouvelAdherent;
}

void afficherAdherents(Adherent *tete) {
    if (tete == NULL) {
        printf("Aucun adhérent à afficher.\n");
        return;
    }

    printf("\n+------------------+-----+------------+--------------+-----------------+----------------+------------------+\n");
    printf("| Nom              | Age | Superficie | Localisation | Type de Culture | Espèce Animale | Nombre d'Animaux |\n");
    printf("+------------------+-----+------------+--------------+-----------------+----------------+------------------+\n");

    Adherent *courant = tete;
    while (courant != NULL) {
        printf("| %-16s | %-3d | %-10.2f | %-12s | %-15s | %-15s | %-16d |\n",
               courant->nom, courant->age, courant->terrain.superficie,
               courant->terrain.localisation, courant->terrain.typeCulture,
               courant->animal.espece, courant->animal.nombre);
        courant = courant->suivant;
    }

    printf("+------------------+-----+------------+--------------+-----------------+----------------+------------------+\n");
}

void sauvegarderAdherentsDansFichier(Adherent *tete) {
    FILE *fichier = fopen("adherents.txt", "w");
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier pour la sauvegarde !\n");
        return;
    }

    Adherent *courant = tete;
    while (courant != NULL) {
        fprintf(fichier, "%s\t%d\t%.2f\t%s\t%s\t%s\t%d\n",
                courant->nom, courant->age,
                courant->terrain.superficie, courant->terrain.localisation,
                courant->terrain.typeCulture, courant->animal.espece,
                courant->animal.nombre);
        courant = courant->suivant;
    }

    fclose(fichier);
    printf("Données sauvegardées avec succès !\n");
}

Adherent *chargerAdherentsDepuisFichier() {
    FILE *fichier = fopen("adherents.txt", "r");
    if (fichier == NULL) {
        printf("Fichier introuvable, démarrage à vide.\n");
        return NULL;
    }

    Adherent *tete = NULL;
    Adherent *nouvelAdherent;

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        nouvelAdherent = (Adherent *)malloc(sizeof(Adherent));
        if (nouvelAdherent == NULL) {
            printf("Erreur d'allocation mémoire lors du chargement.\n");
            fclose(fichier);
            return tete;
        }

        if (sscanf(ligne, "%49[^\t]\t%d\t%f\t%49[^\t]\t%29[^\t]\t%49[^\t]\t%d",
                   nouvelAdherent->nom, &nouvelAdherent->age,
                   &nouvelAdherent->terrain.superficie,
                   nouvelAdherent->terrain.localisation,
                   nouvelAdherent->terrain.typeCulture,
                   nouvelAdherent->animal.espece,
                   &nouvelAdherent->animal.nombre) == 7) {
            nouvelAdherent->suivant = tete;
            tete = nouvelAdherent;
        } else {
            free(nouvelAdherent);
        }
    }

    fclose(fichier);
    printf("Données chargées avec succès.\n");
    return tete;
}

void libererListe(Adherent *tete) {
    Adherent *temp;
    while (tete != NULL) {
        temp = tete;
        tete = tete->suivant;
        free(temp);
    }
}

Adherent *supprimerAdherent(Adherent *tete) {
    if (tete == NULL) {
        printf("Aucun adhérent à supprimer.\n");
        return NULL;
    }

    char nomASupprimer[MAX_NOM];
    printf("Entrez le nom de l'adhérent à supprimer : ");
    scanf(" %49[^\n]", nomASupprimer);

    Adherent *courant = tete;
    Adherent *precedent = NULL;

    while (courant != NULL) {
        if (strcmp(courant->nom, nomASupprimer) == 0) {
            if (precedent == NULL) {
                tete = courant->suivant;
            } else {
                precedent->suivant = courant->suivant;
            }
            free(courant);
            printf("Adhérent supprimé avec succès.\n");
            return tete;
        }
        precedent = courant;
        courant = courant->suivant;
    }

    printf("Adhérent introuvable.\n");
    return tete;
}


void rechercherAdherent(Adherent *tete) {
    if (tete == NULL) {
        printf("Aucun adhérent à rechercher.\n");
        return;
    }

    char nomRecherche[MAX_NOM];
    printf("Entrez le nom de l'adhérent à rechercher : ");
    scanf(" %49[^\n]", nomRecherche);

    Adherent *courant = tete;
    while (courant != NULL) {
        if (strcmp(courant->nom, nomRecherche) == 0) {
            printf("\nAdhérent trouvé :\n");
            printf("Nom : %s\n", courant->nom);
            printf("Age : %d\n", courant->age);
            printf("Superficie du terrain : %.2f\n", courant->terrain.superficie);
            printf("Localisation : %s\n", courant->terrain.localisation);
            printf("Type de culture : %s\n", courant->terrain.typeCulture);
            printf("Espèce animale : %s\n", courant->animal.espece);
            printf("Nombre d'animaux : %d\n", courant->animal.nombre);
            return;
        }
        courant = courant->suivant;
    }

    printf("Adhérent introuvable.\n");
}


int main() {
    Adherent *tete = chargerAdherentsDepuisFichier();

    int choix;
    do {
        printf("\n=== Gestion de la coopérative agricole ===\n");
        printf("1. Ajouter un adhérent\n");
        printf("2. Afficher les adhérents\n");
        printf("3. Supprimer un adhérent\n");
        printf("4. Rechercher un adhérent\n");
        printf("5. Sauvegarder et quitter\n");
        printf("Choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                tete = ajouterAdherent(tete);
                break;
            case 2:
                afficherAdherents(tete);
                break;
            case 3:
                tete = supprimerAdherent(tete);
                break;
            case 4:
                rechercherAdherent(tete);
                break;
            case 5:
                sauvegarderAdherentsDansFichier(tete);
                printf("Données sauvegardées. Au revoir !\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 5);

    libererListe(tete);
    return 0;
}
