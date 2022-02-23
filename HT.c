#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int value;
    short balance;
    struct node *pLeft;
    struct node *pRight;
} NODE;

//TASAPAINOTETTU BINÄÄRIHAKUPUU ELI PUNAMUSTA PUU

int menu(void);
NODE* rightRotation(NODE *pRoot, int *etp);
NODE* leftRotation(NODE *pRoot, int *etp);
NODE* addNode(NODE *pRoot, int number, int *etp);
NODE* getKey(NODE *pRoot, int key);
void printTree(NODE *pNode);
void releaseMemory(NODE *pNode);



int main(void) {
  NODE *pRoot = NULL, *ptr;
  int choice, number, key;
  int etp = 0;
  //muutetaan myöhemmin tiedostoksi!!!!!
  int list[9] = {2, 4, 6, 8, 10, 12, 14, 30, 28};

  //Lisätään avaimet tiedostosta puuhun ja tulostetaan sen rakenne
  for (int i = 0; i < 9; i++){
    pRoot = addNode(pRoot, list[i], &etp);
  }
  printf("Binääriseen hakupuuhun on jo lisätty tiedostosta luetut avaimet ja sen rakenne on seuraavanlainen: \n");
  printTree(pRoot);
  printf("\n");

  while(1){
    printf("\n");
    choice = menu();
    if (choice == 0){
      break;
    }
    else if (choice == 1){
      printf("Anna lisättävä luku: ");
      scanf("%d", &number);
      getchar();
      pRoot = addNode(pRoot, number, &etp);
    }
    else if (choice == 2){
      printf("Anna haettava avain: ");
      scanf("%d", &key);
      getchar();
      ptr = getKey(pRoot, key);
      if (ptr != NULL) {
        printf("Etsimäsi solmu on: %d\n", ptr->value);
      }
      else {
        printf("Etsimäsi solmu ei ollut puussa.\n");
      }
    }
    else if (choice == 3){
      printTree(pRoot);
      printf("\n");
    }
    else {
      printf("Tunnistamaton valinta.\n");
    }
  }

  releaseMemory(pRoot);
  return 0;
}


int menu() {
    int choice;
    printf("Valitse toiminto:\n");
    printf("1) Lisää alkio\n");
    printf("2) Hae avain\n");
    printf("3) Tulosta hakupuu\n");
    printf("0) Lopeta\n");
    printf("Valintasi: ");
    scanf("%d", &choice);
    getchar();
    return choice;
}


NODE* reserveMemory(NODE *ptr) {
    if ((ptr = (NODE*)malloc(sizeof(NODE))) == NULL) {
        perror("Muistin varaus epäonnistui");
        exit(0);
    }
    return ptr;
}

//Lisätään yksittäisiä avaimia puuhun
NODE* addNode(NODE *pRoot, int number) {
    NODE *ptr, *pNew = NULL;
    pNew = reserveMemory(pNew);
    pNew->value = number;
    pNew->pLeft = NULL;
    pNew->pRight = NULL;
    if (pRoot == NULL) {
        pRoot = pNew;
    } else {
        ptr = pRoot;
        while (1) {
            if (number < ptr->value && ptr->pLeft != NULL) {
                ptr = ptr->pLeft;
            } else if (number >= ptr->value && ptr->pRight != NULL) {
                ptr = ptr->pRight;
            } else {
                break;
            }
        }
        if (number < ptr->value) {
            ptr->pLeft = pNew;
        } else {
            ptr->pRight = pNew;
        }
    }
    return pRoot;
}

//Haetaan yhtä avainta puusta
NODE* getKey(NODE *pRoot, int key) {
    NODE *ptr = pRoot;
    while (ptr != NULL) {
        if (ptr->value == key) {
            break;
        } else if (key < ptr->value) {
            ptr = ptr->pLeft;
        } else {
            ptr = ptr->pRight;
        }
    }
    return ptr;
}

//Tulostetaan puurakenne
void printTree(NODE *pNode) {
    if (pNode != NULL) {
        NODE *ptr = pNode;
        printTree(ptr->pLeft);
        printf("%d, ", ptr->value);
        printTree(ptr->pRight);
    }
    return;
}

void releaseMemory(NODE *pNode) {
    if (pNode != NULL) {
        releaseMemory(pNode->pLeft);
        releaseMemory(pNode->pRight);
        printf("poistetaan solmu: %d\n", pNode->value);
        free(pNode);
    }
    return;
}

//Viikon 7 työpajatehtävä
//mikä lyhenne on etp???
NODE* addNode(NODE *pRoot, int number, int *etp){
  if(!(pRoot)){
    *etp = 1;
    if((pRoot = (NODE*)malloc(sizeof(NODE))) == NULL){
      perror("malloc");
      exit(1);
    }
    pRoot->pLeft = pRoot->pRight = NULL;
    pRoot->balance = 0;
    pRoot->value = number;
  } 
  else if(number < pRoot->value){
    addNode(pRoot->pLeft, number, etp);
    if(*etp){
      switch(pRoot->balance){
        case -1:
          pRoot->balance = 0;
          *etp = 0;
          break;
        case 0:
          pRoot->balance = 1;
          break;
        case 1:
          pRoot = leftRotation(pRoot, etp);
      }
    }
  }
  else if(number > pRoot->value){
    addNode(pRoot->pRight, number, etp);
    if(*etp){
      switch(pRoot->balance){
        case 1:
          pRoot->balance = 0;
          *etp = 0;
          break;
        case 0:
          pRoot->balance = -1;
           break;
        case -1:
          pRoot = rightRotation(pRoot, etp);
      }
    }
  }
  else{
    *etp = 0;
    printf("Luku %d on jo puussa\n", number);
  }
  return pRoot;
}



NODE* rightRotation(NODE *pRoot, int *etp){
  NODE *child, *grandchild;

  child = pRoot->pRight;
  if(child->balance == -1) {
    //RR-kierto
    pRoot->pRight = child->pLeft;
    child->pLeft = pRoot;
    pRoot->balance = 0;
    pRoot = child;
  }
  else {
    //RL-kierto
    grandchild = child->pLeft;
    child->pLeft = grandchild->pRight;
    grandchild->pRight = child;
    pRoot->pRight = grandchild->pLeft;
    grandchild->pLeft = pRoot;
    switch(grandchild->balance){
      case 1:
        pRoot->balance = -1;
        child->balance = 0;
        break;
      case 0:
        pRoot->balance = child->balance = 0;
        break;
      case -1:
        pRoot->balance = 0;
        child->balance = 1;
    }
    pRoot = grandchild;
  }
  pRoot->balance = 0;
  *etp = 0;
  return pRoot;
}


NODE* leftRotation(NODE *pRoot, int *etp){
  NODE *child, *grandchild;

  child = pRoot->pLeft;
  if(child->balance == 1) {
    //LL-kierto
    pRoot->pLeft = child->pRight;
    child->pRight = pRoot;
    pRoot->balance = 0;
    pRoot = child;
  }
  else {
    //LR-kierto
    grandchild = child->pRight;
    child->pRight = grandchild->pLeft;
    grandchild->pLeft = child;
    pRoot->pLeft = grandchild->pRight;
    grandchild->pRight = pRoot;
    switch(grandchild->balance){
      case 1:
        pRoot->balance = -1;
        child->balance = 0;
        break;
      case 0:
        pRoot->balance = child->balance = 0;
        break;
      case -1:
        pRoot->balance = 0;
        child->balance = 1;
    }
    pRoot = grandchild;
  }
  pRoot->balance = 0;
  *etp = 0;
  return pRoot;
}


//Lisätään yksittäin avaimet 26, 24, 22, 20, 18, 16