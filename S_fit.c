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

#define MAX_SLOWA 100
#define MAX_DLUGOSC_SLOWA 50
typedef enum { false, true } bool;
char slowa[MAX_SLOWA][MAX_DLUGOSC_SLOWA];

bool HELP(char arr[]);

bool fullcheck(char arr1[], char arr2[], int size);
const char* losuj_i_zwroc_slowo_z_pliku(const char* nazwa_pliku);
int* find_character(const char* word, char character, int* count);
bool check_letter(char* letter, char* word);
void insertionSort(char arr[], int n);
void print_char_array(char arr[], int size);
bool isCharInArray(char arr[], int size, char ch);
void removeDuplicates(char arr[], int size);
char* createStringWithUnderscores(int length);
void replace(char* str, const char* old, char new, const int* indices);
void replace_one(char* str, const char* old, char new, int index);
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
while(1){
  clientLen = sizeof(clientAddr);
  
  /* wait for client connection */
  client = accept(sd, (struct sockaddr *) &clientAddr, &clientLen);
  if(client<0) {
    perror("cannot accept ");
    exit(1);
  }

  printf("%s: received connection from %s\n",argv[0],inet_ntoa(clientAddr.sin_addr));




//   char ack[] = "murbatron";
//   char ack1[] = "murbatron"; // jezeli wpiszemy slowo zgadnda to nie mozemy zgadnac hasla

  const char* nazwa_pliku = "slowa.txt";
    const char* wylosowane_slowo = losuj_i_zwroc_slowo_z_pliku(nazwa_pliku);

        printf("Wylosowane slowo: %s\n", wylosowane_slowo);
        int dlugosc = strlen(wylosowane_slowo);
        printf("Dlugosc wylosowanego slowa: %d\n", dlugosc);



    char ack[dlugosc];
    strcpy(ack, wylosowane_slowo);
    char ack1[dlugosc];
    strcpy(ack1, wylosowane_slowo);

     printf("ack: %s\n", ack);
  int size2=sizeof(ack) / sizeof(ack[0]);
  removeDuplicates(ack,size2);

  int size1=sizeof(ack) / sizeof(ack[0]);
  char haslo[size1];
  char zgadniete[size1];
  //utworzenie stringa zawierającego same "_______"
  char* wysalanie = createStringWithUnderscores(size1);
   int k=0;
   int liczba_prob=15;
   int powtorzenia=0;
  while(1) {
    memset(buffer, 0x0, sizeof(buffer));
    rc = recv(client, buffer, sizeof(buffer), 0);
    if(rc<0) {
      perror("cannot receive data ");
      close(client);
      break;
    }

    if(rc==0) {
      printf("%s: connection closed\n",argv[0]);
      close(client);
      break;
    }

    printf("%s: received %u bytes: %s\n",argv[0],rc,buffer);

        if(fullcheck(buffer,ack,dlugosc)){
            char mess[100];  
            sprintf(mess, "zgadłes haslo! \nhasło to: %s", ack1);
            rc = send(client, mess, strlen(mess) + 1, 0);
            close(client);
      break;
        }
    if(rc>3){
        if(HELP(buffer)){
        rc = send(client, "gra wisielec", strlen("gra wisielec") + 1, 0);
        continue; 
        }
        else{
        if(!fullcheck(buffer,ack,dlugosc)){
        printf("XDXD");
        char messag[100];
      sprintf(messag, "Koniec gry, błędne hasło");
      rc = send(client, messag, strlen(messag) + 1, 0);
      liczba_prob==0;
      close(client);
      break;}
    }
    }


    char nieprawda[] = "litera się nie znajduje w haśle";
 for (int i = 0 ; i< size1;i++){
     haslo[i]=ack[i];
     printf("%c \n",haslo[i]);
   }

    
    if (check_letter(buffer,ack)){ 
    if (liczba_prob == 0){
      char message[37];
      sprintf(message, "Koniec gry, zbyt duża liczba prób");
      rc = send(client, message, strlen(message) + 1, 0);
      close(client);
      break;

    }
    //    printf("znak: %s\n", buffer);  
    if ( ! isCharInArray(zgadniete,size1,buffer[0])){
       zgadniete[k]=buffer[0];
       int numOccurrences = 0;
       int* occurrences= find_character(ack1,buffer[0],&numOccurrences); 
       if (numOccurrences == 1){
            replace_one(wysalanie,"_",buffer[0],occurrences[0]);
       }
       else{
            replace(wysalanie,"_",buffer[0],occurrences);
            powtorzenia++;
       }
       free(occurrences);
       k++;
    }
      memset(buffer, 0, sizeof(buffer)); 
      printf("k= %d  size=%d powtorzneia=%d",k,size1,powtorzenia);
      if (k==size1-powtorzenia){
      insertionSort(zgadniete,k);
      insertionSort(haslo,k);
      print_char_array(zgadniete,k);
      print_char_array(haslo,k);
      printf("%d", memcmp ( zgadniete, haslo,k ));
       if ( memcmp ( zgadniete, haslo,k ) == 0){
       char message[100];  
        sprintf(message, "zgadłes haslo! \nhasło to: %s", ack1);
        rc = send(client, message, strlen(message) + 1, 0);
         close(client);
      break;
       }
      }
      char message[100];
      sprintf(message,"litera się znajduje w haśle \nPozostała liczba prób: %d\nPozostale znaki: %s",liczba_prob,wysalanie);
      rc = send(client,message, strlen(message) + 1, 0);

    //  for(int i=0;i<k+1;i++){
    //    printf("znak: %c\n",zgadniete[i]);}

    
   
    }
    else{
      liczba_prob--;
      printf("znak: %s\n",buffer);
      rc = send(client, nieprawda, strlen(nieprawda) + 1, 0);
      
    }
    if(rc<0) {
      perror("cannot send acknowledgment ");
      close(client);
      break;
    }

    printf("%s: hasło: %s\n", argv[0], ack);
  }
}
close(sd);
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

void replace_one(char* str, const char* old, char new, int index) {
    size_t old_len = strlen(old);
    
    if (index >= 0 && index < strlen(str)) {
        if (memcmp(str + index, old, old_len) == 0) {
            str[index] = new;
        }
    }
}
void replace(char* str, const char* old, char new, const int* indices) {
   // print_int_array(indices,1);
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
        printf("%c ", arr[i]);
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
int* find_character(const char* word, char character, int* count) {
    int length = 0;
    int i;
    int* indices;
    
    // Zliczanie liczby wystąpień danego znaku w słowie
    for (i = 0; word[i] != '\0'; i++) {
        if (word[i] == character) {
            length++;
        }
    }
    
    // Tworzenie tablicy dla indeksów znalezionych znaków
    if (length == 1) {
        indices = (int*)malloc(sizeof(int));
        for (i = 0; word[i] != '\0'; i++) {
            if (word[i] == character) {
                indices[0] = i;
                break;
            }
        }
    } else {
        indices = (int*)malloc(length * sizeof(int));
        int index = 0;
        
        // Zapisywanie indeksów znalezionych znaków
        for (i = 0; word[i] != '\0'; i++) {
            if (word[i] == character) {
                indices[index++] = i;
            }
        }
    }
    
    *count = length; // Przekazanie liczby wystąpień przez wskaźnik
    
    return indices;
}


// int* find_character(const char* word, char character, int* count) {
//     int length = 0;
//     int i;
    
//     // Zliczanie liczby wystąpień danego znaku w słowie
//     for (i = 0; word[i] != '\0'; i++) {
//         // printf("znak %d: %c \n",i, word[i]);
//         // printf("litera %d: %c \n",i, character);
//         if (word[i] == character) {
//             length++;
//         }
//     }
//     // Tworzenie tablicy dla indeksów znalezionych znaków
//     int* indices = (int*)malloc(length* sizeof(int));
//     int index = 0;
    
//     // Zapisywanie indeksów znalezionych znaków
//     for (i = 0;  word[i] != '\0' ; i++) {
//         if (word[i] == character) {
//             indices[index++] = i;
            
//         }
//     }
//     *count = length; // Przekazanie liczby wystąpień przez wskaźnik
//     if (length == 1){
//         return indices[0];
//     }
//     else{
//         return indices;
//     }
// }
// int* findCharOccurrences(char character, const char* string, int* numOccurrences) {
    
//     int length = strlen(string);
//     int* occurrences = malloc(length * sizeof(int));
//     print_int_array(occurrences,2);
//     int index = 0;
//     printf("\n %c",string[0]);

//     for (int i = 0; i < length+1; i++) { /*
//     tutaj jest problem z ta dlugościa, nie rozkmniałem tego jakos długo dlaczego nie działa poprtu troche potestowałem i zoabczłem ze nie wypisuje odpowiedniej dluosci tablicy wiec stad to +1, ale cos jeszcze jest tutaj skurwione ewidentnie*/
//         if (string[i] == character) {
//             occurrences[index] = i;
//             index++;
//         }
//     }

//     if (index == 0) {
//         free(occurrences);
//         return NULL;
//     }
//     *numOccurrences = index;
    
//     return occurrences;
// }

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
const char* losuj_i_zwroc_slowo_z_pliku(const char* nazwa_pliku)
{
    // Inicjalizacja generatora liczb pseudolosowych
    srand(time(NULL));

    // Otwieranie pliku
    FILE* plik = fopen(nazwa_pliku, "r");
    if (plik == NULL)
    {
        printf("Nie udalo sie otworzyc pliku.\n");
        return NULL;
    }

    // Wczytywanie slow z pliku do tablicy
    int ilosc_slow = 0;

    char slowo[MAX_DLUGOSC_SLOWA];
    while (ilosc_slow < MAX_SLOWA && fscanf(plik, "%s", slowo) == 1)
    {
        strcpy(slowa[ilosc_slow], slowo);
        ilosc_slow++;
    }

    fclose(plik);

    if (ilosc_slow == 0)
    {
        printf("Brak slow w pliku.\n");
        return NULL;
    }

    // Wylosowanie indeksu slowa
    int indeks = rand() % ilosc_slow;

    // Zwracanie wylosowanego slowa
    return slowa[indeks];
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

bool fullcheck(char arr1[], char arr2[],int size) {
    

    for (int i = 0; i < size; i++) {
        if (arr1[i] != arr2[i]) {
            return false;
        }
    }

    return true;
}

bool HELP(char arr[]){
char wzor[]="HELP";
     for (int i = 0; i < 4; i++) {
        if (arr[i] != wzor[i]) {
            return false;
        }
    return true;
    }
}