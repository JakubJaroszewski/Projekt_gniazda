/* fpont 12/99 */
/* pont.net    */
/* tcpServer.c */
/* korekta W.Bajdecki 2009, 2019, 2023 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* close */
#include <string.h> /* memset() */

#include <time.h>



typedef enum { false, true } bool;

bool check_letter(char* letter, char* word);
void insertionSort(char arr[], int n);
void print_char_array(char arr[], int size);
bool isCharInArray(char arr[], int size, char ch);
void removeDuplicates(char arr[], int size);
char* createStringWithUnderscores(int length);
void replace(char* str, const char* old, char new, const int* indices);
int* findCharOccurrences(char character, const char* string, int* numOccurrences) ;
void print_int_array(int arr[], int size) ;
int main (int argc, char *argv[]) { /* licznik argumentow, tablica argumentow */

  const int PORT=1500;

  int sd, client, rc, on=1;
  char buffer[1024];
  struct sockaddr_in servAddr;
  struct sockaddr_in clientAddr;
  socklen_t clientLen;


  /* create socket */
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if(sd<0) {
    perror("cannot open socket ");
    exit(1);
  }

  /* set socket option */
  rc = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
  if (rc<0) {
    perror("cannot set socket option ");
    exit(1);
  }

  /* bind server port */
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(PORT);
  rc = bind(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
  if(rc<0) {
    perror("cannot bind port ");
    exit(1);
  }

  /* specify queue */
  rc = listen(sd,5);
  if(rc<0) {
    perror("cannot listen ");
    exit(1);
  }

  printf("%s: listening for incoming connections on port %d\n",argv[0],PORT);

  clientLen = sizeof(clientAddr);
  
  /* wait for client connection */
  client = accept(sd, (struct sockaddr *) &clientAddr, &clientLen);
  if(client<0) {
    perror("cannot accept ");
    exit(1);
  }

  printf("%s: received connection from %s\n",argv[0],inet_ntoa(clientAddr.sin_addr));




 char words[][10] = {
        "paula",
        "domek",
        "bajda",
        "myszka",
        "kuba"
    };

    int i;
    int length = sizeof(words) / sizeof(words[0]);

    for (i = 0; i < length; i++) {
        printf("%s\n", words[i]);
    }

    srand(time(NULL));
    int indeks = rand() % length;

    int dlugosc = strlen(words[indeks]);

    char ack[dlugosc];
    char ack1[dlugosc];

    strcpy(ack, words[indeks]);
    printf("slowo: %s\n", ack);
    strcpy(ack1, words[indeks]);
    printf("slowo1: %c\n", ack1[0]);



  int size2=sizeof(ack) / sizeof(ack[0]);
  printf("slowosize1: %d\n", size2);
  removeDuplicates(ack,size2);

  int size1=sizeof(ack) / sizeof(ack[0]);
  char haslo[size1];
  char zgadniete[size1];
  //utworzenie stringa zawierającego same "_______"
  char* wysalanie = createStringWithUnderscores(size1);
   int k=0;
   int liczba_prob=15;
  while(1) {
    memset(buffer, 0x0, sizeof(buffer));
    rc = recv(client, buffer, sizeof(buffer), 0);
    if(rc<0) {
      perror("cannot receive data ");
      close(sd);
      exit(1);
    }

    if(rc==0) {
      printf("%s: connection closed\n",argv[0]);
      close(sd);
      exit(1);
    }

    printf("%s: received %u bytes: %s\n",argv[0],rc,buffer);

    char nieprawda[] = "litera się nie znajduje w haśle";
for (int i = 0 ; i< size1;i++){
    haslo[i]=ack[i];
    printf("%c \n",haslo[i]);
  }

    
    if (check_letter(buffer,ack)){ 
    liczba_prob--;
    if (liczba_prob == 0){
      char message[37];
      sprintf(message, "Koniec gry, zbyt duża liczba prób");
      rc = send(client, message, strlen(message) + 1, 0);
      close(sd);
      exit(1);

    }
       printf("znak: %s\n", buffer);  
    if ( ! isCharInArray(zgadniete,size1,buffer[0])){
      zgadniete[k] = buffer[0];
      int numOccurrences = 0;
       int* occurrences= findCharOccurrences(buffer[0], ack1,&numOccurrences); /* dla slowa zgadnia wszystko działa jak powinno, dla słowa kupsko juz nie [mozliwe ze chodzi o to ze k jest na 0 i 4 miejscu, dokladniej ze litera która występuje dwa razy znajduje sie na 0 miejscu i cos sie w funkcji pierdoli] 
       UPDATE
       raczej na pewno chodzi o to ze w slowie kupsko k jest na 0 i 4 miejscu, testowałem to dla paru innych słów gdzie lietra która występuje 2 razy nie znajduje sie na 0 miejscu i wsystko działa, możesz sprobować to naprawić, ja juz cos tam grzebałem przy niej bo ewidentnnie cos jest nie tak, komenatrze nizej*/
      replace(wysalanie,"_",buffer[0],occurrences);
      free(occurrences);
      k++;
    }
      memset(buffer, 0, sizeof(buffer)); 
      if (k==size1-2){
      insertionSort(zgadniete,size1);
      insertionSort(haslo,size1);
       if ( memcmp ( zgadniete, haslo,size1 ) == 0){
       char message[100];  
        sprintf(message, "zgadłes haslo! \nhasło to: %s", ack1);
        rc = send(client, message, strlen(message) + 1, 0);
       }
      }
      char message[100];
      sprintf(message,"litera się znajduje w haśle \nPozostała liczba prób: %d\nPozostale znaki: %s",liczba_prob,wysalanie);
      rc = send(client,message, strlen(message) + 1, 0);

     for(int i=0;i<k+1;i++){
       printf("znak: %c\n",zgadniete[i]);}

    
   
    }
    else{
      liczba_prob--;
      printf("znak: %s\n",buffer);
      rc = send(client, nieprawda, strlen(nieprawda) + 1, 0);
      
    }
    if(rc<0) {
      perror("cannot send acknowledgment ");
      close(sd);
      exit(1);
    }

    printf("%s: hasło: %s\n", argv[0], ack);
  }

  return 0;
}

void print_int_array(int arr[], int size) {
    printf("Tablica intów:");
    for (int i = 0; i < size; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");
}
bool check_letter(char* letter, char* word) {
    int letter_len = strlen(letter);
    for (int i = 0; word[i] != '\0'; i++) {
        for (int j = 0; j < letter_len; j++) {
            if (word[i] == letter[j]) {
                return true;
            }
        }
    }
    return false;
}

void replace(char* str, const char* old, char new, const int* indices) {
    size_t old_len = strlen(old);
    int size = sizeof(indices) / sizeof(indices[0]);
    for (int i = 0; i < size; i++) {
        int index = indices[i];

        if (index >= 0 && index < strlen(str)) {
            if (memcmp(str + index, old, old_len) == 0) {
                str[index] = new;
            }
        }
    }
}
void print_char_array(char arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%s ", arr[i]);
    }
    printf("\n");
}
void insertionSort(char arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}
int* findCharOccurrences(char character, const char* string, int* numOccurrences) {
    int length = strlen(string);
    int* occurrences = malloc(length * sizeof(int));
    int index = 0;

    for (int i = 0; i < length+1; i++) { /*
    tutaj jest problem z ta dlugościa, nie rozkmniałem tego jakos długo dlaczego nie działa poprtu troche potestowałem i zoabczłem ze nie wypisuje odpowiedniej dluosci tablicy wiec stad to +1, ale cos jeszcze jest tutaj skurwione ewidentnie*/
        if (string[i] == character) {
            occurrences[index] = i;
            index++;
        }
    }

    if (index == 0) {
        free(occurrences);
        return NULL;
    }
    *numOccurrences = index;
    return occurrences;
}

void removeDuplicates(char arr[], int size) {
    int index = 0;  // Indeks dla unikalnych znaków

    for (int i = 0; i < size; i++) {
        bool isDuplicate = false;

        // Sprawdź, czy znak jest duplikatem
        for (int j = 0; j < index; j++) {
            if (arr[i] == arr[j]) {
                isDuplicate = true;
                break;
            }
        }

        // Jeśli znak nie jest duplikatem, dodaj go do unikalnych znaków
        if (!isDuplicate) {
            arr[index] = arr[i];
            index++;
        }
    }

    // Wypełnij pozostałe miejsca w tablicy znakiem null terminatora
    for (int i = index; i < size; i++) {
        arr[i] = '\0';
    }
}
char* createStringWithUnderscores(int length) {
    char* str = malloc((length + 1) * sizeof(char));  // +1 dla znaku null na końcu
    
    if (str == NULL) {
        printf("Błąd alokacji pamięci.\n");
        exit(1);
    }
    
    for (int i = 0; i < length; i++) {
        str[i] = '_';
    }
    
    str[length] = '\0';  // Dodanie znaku null na końcu
    
    return str;
}
bool isCharInArray(char arr[], int size, char ch) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == ch) {
            return true;  // Znak jest już w tablicy
        }
    }
    return false;  // Znak nie jest w tablicy
}
// bool compare ( char tab[] , char tab1[]  ){
//  int size = sizeof(tab) / (sizeof(tab[0]));
//  int size1 = sizeof(tab1) / (sizeof(tab1[0]));
//  if (size1 != size){
//    return false;
//  }
//  insertionSort(tab,size);
//  insertionSort(tab1,size);
 
 
//  for (int i = 0 ; i< size;i++){
//     if( tab[i]==tab1[i]){
//       continue;
//     }
//     else{
//       return false;
//     }
//  }
//  return true;
  
// }