#include <stdio.h>
#include <string.h>
#include<unistd.h>
#define maxchar 1000


typedef struct {
    int pr;
    char descr[maxchar];
    int id;
} Tache;
Tache t;

typedef struct {
    char username[50];
    char password[50];
   
} User;

User user;
char nomtache[20];
char usernamec[20];

int chec = 0;
FILE *fp, *ff, *fu;

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
void filename() {
    sprintf(usernamec, "%s.txt", user.username);
}
int nombretache() {
    filename();
  
    fp = fopen(usernamec, "r");

    if (fp == NULL) {
        perror("Error");
        return -1;
    }

    int s = 0;

 
    while (fscanf(fp, "%d %d %[^\n]", &t.pr, &t.id, t.descr) == 3) {
        s++;
    }

    fclose(fp);
    return s;
}

void Loading() {
    int i;
    printf("Loading ");
    for (i = 0; i < 7; i++) {
        printf(".");
        fflush(stdout);  // Flush the output buffer to ensure immediate display
        usleep(300000);  // Sleep for 0.5 seconds (500,000 microseconds)
    }
    printf(" Done!\n");
}




void ajoutertache() {
    filename();
    fp = fopen(usernamec, "a");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    printf("Saisir la priorite de la tache (1.haut,2.moyen,3.bas): ");
    if (scanf("%d", &t.pr) != 1) {
        printf("Erreur lors de la saisie de la priorite.\n");
        fclose(fp);
        return;
    }
    fprintf(fp, "%d ", t.pr);

    printf("Entrer l'id (ne depasse pas 5 chiffres): ");
    if (scanf("%d", &t.id) != 1) {
        printf("Erreur lors de la saisie de l'id.\n");
        fclose(fp);
        return;
    }
    fprintf(fp, "%d ", t.id);

    clearBuffer();  // Clear the input buffer

    printf("Saisir la description: ");
    fgets(t.descr, maxchar, stdin);
    fprintf(fp, "%s", t.descr);


    fclose(fp);

    
}



void affichage() {
    int i = 0;
    char priorite[20];
    filename();
    fp = fopen(usernamec, "r");
    while (fscanf(fp, "%d %d %[^\n]", &t.pr, &t.id, t.descr) == 3) {
        switch (t.pr)
        {
        case 1:
            strcpy(priorite,"haute");
            break;
        case 2:
        strcpy(priorite,"moyenne");
            break;
            case 3:
            strcpy(priorite,"bas");
            break;

        default:
            break;
        }
                printf("----> la priorite:[%s] l id[%d] la description: %s\n", priorite, t.id, t.descr);
        i++;
    }

    fclose(fp);
}
void suppression() {
    filename(); // Assuming filename() is declared and implemented

    int tacheId = 0, s = 0;

    printf("Entrez l'indice de la tache a supprimer : ");
    scanf("%d", &tacheId);

    fp = fopen(usernamec, "r");
    ff = fopen("tachetemp.txt", "w");

    if (fp == NULL || ff == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    while (fscanf(fp, "%d %d %[^\n]", &t.pr, &t.id, t.descr) == 3) {
        if (tacheId == t.id) {
            s = 1;
            continue; 
        }
        fprintf(ff, "%d %d %s\n", t.pr, t.id, t.descr);
    }

    if (s != 1) {
        printf("Indice de tache invalide.\n");
    }

    fclose(fp);
    fclose(ff);


    remove(usernamec);
  
    rename("tachetemp.txt", usernamec);

    if (s == 1) {
        printf("Bien supprimee!\n");
    }
}

void modifier() {
    filename();
    int tacheId, s = 0;
    FILE *fp,*ft;
 

    printf("Entrer l'indice de tache a modifier:\n");
    scanf("%d", &tacheId);

    fp = fopen(usernamec, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }
     ft = fopen("temp.txt", "a");
    if (ft == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    while (fscanf(fp, "%d %d %[^\n]", &t.pr, &t.id, t.descr) == 3) {
        if (tacheId == t.id) {
            s = 1;
            break;
        }
    }

    if (s != 1) {
        printf("Indice de tache invalide.\n");
        fclose(fp);
        return;
    }

    fseek(fp, 0, SEEK_SET);

    while (fscanf(fp, "%d %d %[^\n]", &t.pr, &t.id, t.descr) == 3) {
        if (tacheId != t.id) {
            fprintf(ft, "%d %d %s\n", t.pr, t.id,t.descr);
        }
        else {
            printf("Entrer la nouvelle description:\n");
            getchar(); // Consume the newline character left in the input buffer
            fgets(t.descr, maxchar, stdin);
            fprintf(ft, "%d %d %s\n", t.pr,t.id,t.descr);
        }
    }

    fclose(fp);
    fclose(ft);
    remove(usernamec);
    rename("temp.txt", usernamec);

}

void triageparpriorite() {
    FILE *fp,*ft;
    filename();

    fp = fopen(usernamec, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    ft = fopen("temp.txt", "a");
    if (ft == NULL) {
        perror("Error opening temporary file");
        fclose(fp);
        return;
    }

    int j = 0, s = nombretache();
    printf("nombre de tache est: %d\n", s);

    while (j < s) {
        fscanf(fp, "%d %d %[^\n]", &t.id, &t.pr, t.descr);
        if (t.id == 1) {
            fprintf(ft, "%d %d %s\n", t.id, t.pr, t.descr);
        }
        j++;
    }

    rewind(fp);
    j = 0;
    while (j < s) {
        fscanf(fp, "%d %d %[^\n]", &t.id, &t.pr, t.descr);
        if (t.id == 2) {
            fprintf(ft, "%d %d %s\n", t.id, t.pr, t.descr);
        }
        j++;
    }

    rewind(fp);
    j = 0;
    while (j < s) {
        fscanf(fp, "%d %d %[^\n]", &t.id, &t.pr, t.descr);
        if (t.id == 3) {
            fprintf(ft, "%d %d %s\n", t.id, t.pr, t.descr);
        }
        j++;
    }

    fclose(fp);
    fclose(ft);

    // Replace the original file with the sorted temporary file
    remove(usernamec);
    rename("temp.txt", usernamec);
}

void login(char *username, char *password) {
    int rerun=0;
    fu = fopen("users.txt", "r");

    int isLoggedIn = 0;

    while (fscanf(fu, "%s %s",user.username, user.password) == 2) {
        if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0) {
            Loading();
            printf("Connexion réussie!\n");
            chec=1;
           strcpy(usernamec,user.username);
            isLoggedIn = 1;
            break;
        }
    }

    fclose(fu);

    if (!isLoggedIn) {
        printf("Nom d'utilisateur ou mot de passe incorrect.\n");
        chec=0;
        
    }
}

void signup() {
    int rerun=0;
    char tempuser[20];
    fu = fopen("users.txt", "a+");
    printf("Creation d'un nouveau compte\n");
    printf("Veuillez entrer un nom d'utilisateur: ");
     scanf("%s", tempuser);
    while((fscanf(fu,"%s %s",user.username,user.password))==2){
        if((strcmp(tempuser,user.username))==0){
            printf("username exists!!!\n");
            fclose(fu);
            rerun=1;
            break;
        }
   
    }
    if(rerun==0){
    strcpy(user.username,tempuser);
    fseek(fp,0,SEEK_END);
    
    printf("Veuillez entrer un mot de passe: ");
    scanf("%s", user.password);
    fprintf(fu, "%s %s\n",user.username, user.password);
    fclose(fu);
    filename();

    FILE *fs = fopen(usernamec, "a");
    
    fclose(fs);
    }
}
void catalogue() {
    printf("\n------------------------------------");
    printf("\n           TO-DO LIST               ");
    printf("\n------------------------------------");
    printf("\n1. Ajouter une tache ");
    printf("\n2. Afficher les tache");
    printf("\n3. Supprimer une tache ");
    printf("\n5. Modifier une tache ");
    printf("\n6. triage par priorite ");
    printf("\n4. Deconnecter");
    printf("\n0. quittter");
    printf("\n------------------------------------");
    printf("\nChoisissez une option (1-4): ");}
void menu() {
    

    int i,choice;
    do {
        catalogue();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
            Loading();
                ajoutertache();
                printf("Done!!!\n");
                break;
            case 2:
            Loading();
                affichage();
                printf("Done!!!\n");
                break;
            case 3:
            Loading();
                suppression();
                printf("Done!!!\n");
                break;
            case 5:
            Loading();
                modifier();
                printf("Done!!!\n");
                break;
            case 6:
            Loading();
            triageparpriorite();
            printf("Done!!!\n");
            break;
            case 0:
            exit(1);
            break;
            default:
                break;
        }
        
        
    } while (choice != 4);
    if(choice==4){
    printf("*************************\n");
    printf("*                       *\n");
    printf("*     Au Revoir!        *\n");
    printf("*                       *\n");
    printf("*                       *\n");
    printf("*************************\n");


    }
  
}

void mainn() {
    int choice;

    do {
        printf("1. S'inscrire\n");
        printf("2. Se connecter\n");
        printf("3. Quitter\n");
        printf("Veuillez entrer votre choix: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                signup();
                break;
            case 2: {

                char username[50], password[50];
                printf("Veuillez entrer votre nom d'utilisateur: ");
                scanf("%s", username);
                printf("Veuillez entrer votre mot de passe: ");
                scanf("%s", password);

                login(username, password);
                if(chec==1){
                    menu();
                }
                
                
                break;
            }
            case 3:
                printf("Au revoir!\n");
                
                break;
            default:
                printf("Choix non valide, veuillez réessayer.\n");
        }
        
    } while (choice != 3);
}

int main() {
    mainn();


    return 0;
}