//Paulina Rudzka i Jakub Jaroszewski
//SK 2023
//Część programu związana a gniazdami ze źródła: program W. Bajdeckiego



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
#include <ctype.h>
#include <string.h>

#define MAX_SLOWA 100
#define MAX_DLUGOSC_SLOWA 50

typedef enum { false, true } bool;
char slowa[MAX_SLOWA][MAX_DLUGOSC_SLOWA];

//deklaracje używanych funkcji
bool HELP(char arr[]); 
bool fullcheck(char arr1[], char arr2[], int size);
const char* choose_word(const char* nazwa_pliku);
int* find_character(const char* word, char character, int* count);
bool check_letter(char* letter, char* word);
bool isCharInArray(char arr[], int size, char ch);
void removeDuplicates(char arr[], int size);
char* createStringWithUnderscores(int length);
void replace(char* str, const char* old, char new, const int* indices);
void replace_one(char* str, const char* old, char new, int index);
int* findCharOccurrences(char character, const char* string, int* numOccurrences) ;


int main (int argc, char* argv[]) { // licznik argumentow, tablica argumentow 

  const int PORT=1500;

  int sd, client, rc, on=1;
  char buffer[1024];
  struct sockaddr_in servAddr;
  struct sockaddr_in clientAddr;
  socklen_t clientLen;


//utworzenie gniazda
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if(sd<0) {
    perror("cannot open socket ");
    exit(1);
  }

//wielokrotne używanie adresu
  rc = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
  if (rc<0) {
    perror("cannot set socket option ");
    exit(1);
  }

//przypisanie adresu i portu do gniazda, bind
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(PORT);
  rc = bind(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
  if(rc<0) {
    perror("cannot bind port ");
    exit(1);
  }

//program obsługuje max 1 połączenie
  rc = listen(sd,1);
  if(rc<0) {
    perror("cannot listen ");
    exit(1);
  }

//nasłuchiwanie
  printf("%s: nasłuchiwanie na porcie %d\n",argv[0],PORT);

while(1){


  clientLen = sizeof(clientAddr);
  
//połączenie z klientem
  client = accept(sd, (struct sockaddr *) &clientAddr, &clientLen);
  if(client<0) {
    perror("cannot accept ");
    exit(1);
  }
  printf("%s: nawiązane połączenie z: %s\n",argv[0],inet_ntoa(clientAddr.sin_addr));


//losowanie słowa z bazy
    const char* nazwa_pliku = "slowa.txt";
    const char* wylosowane_slowo = choose_word(nazwa_pliku);

    printf("Wylosowane slowo: %s\n", wylosowane_slowo);
    int dlugosc = strlen(wylosowane_slowo);
    printf("Dlugosc wylosowanego slowa: %d\n", dlugosc);


//tworzenie tablicy z hasłem
    char ack[dlugosc];
    strcpy(ack, wylosowane_slowo);
    char ack1[dlugosc];
    strcpy(ack1, wylosowane_slowo);

//usuwanie powtarzających się liter z hasła
    int size2=sizeof(ack) / sizeof(ack[0]);
    removeDuplicates(ack,size2);

  int size1=sizeof(ack) / sizeof(ack[0]);

//tablica zgadniętych liter
  char zgadniete[size1];

//utworzenie stringa zawierającego same "_____"
  char* wysalanie = createStringWithUnderscores(size1);

//k - liczba zgadniętych liter
   int k=0;
   int liczba_prob=15;
   int powtorzenia=0;

  while(1) {

//otrzymanie litery/hasła od klienta
    memset(buffer, 0x0, sizeof(buffer));
    rc = recv(client, buffer, sizeof(buffer), 0);
    if(rc<0) {
      perror("cannot receive data ");
      close(client);
      break;
    }

    if(rc==0) {
      continue;
    }

//zabezpieczenie: uppercase->lowercase, hasła w bazie są w małych
if(HELP(buffer)==0){ //HELP wielkimi wywołuje pomoc, HELP nie chcemy pomniejszać
     for (int i = 0 ; i< size1;i++){
     buffer[i]=tolower(buffer[i]);
   }
  }


//jeżeli klient wpisze kilka liter

//sprawdza poprawność hasła
    if(fullcheck(buffer,ack1,dlugosc)){
        char mess[100];  
        sprintf(mess, "zgadłes haslo! \nhasło to: %s\n", ack1);
        rc = send(client, mess, strlen(mess) + 1, 0);
        close(client);
        break;
     
    }
    if(rc>3){
//Wywołuje pomoc
        if(HELP(buffer)){
        rc = send(client, "gra zgadywanie słów. użytkownik ma 15 prób na zgadnięcie pasujących liter; istnieje możliwość zgadnięcia całego hasła - jeżeli będzie błędne, gra się kończy", strlen("gra zgadywanie słów. użytkownik ma 15 prób na zgadnięcie pasujących liter; istnieje możliwość zgadnięcia całego hasła - jeżeli będzie błędne, gra się kończy") + 1, 0);
        continue; 
        }
//Jeżeli hasło błędne - koniec gry
        else{
            if(!fullcheck(buffer,ack,dlugosc)){
                char messag[100];
                sprintf(messag, "Koniec gry, błędne hasło");
                rc = send(client, messag, strlen(messag) + 1, 0);
                liczba_prob=0;
                close(client);
                break;}
    }
    }

if (liczba_prob == 0){ //koniec prób, koniec gry
      char message[37];
      sprintf(message, "Koniec gry, zbyt duża liczba prób");
      rc = send(client, message, strlen(message) + 1, 0);
      close(client);
      break;

    }

//sprawdzanie, czy litera jest w haśle
    if (check_letter(buffer,ack)){ 
    

//sprawdzenie, czy litera jest już w bazie zgadniętych liter
    if ( ! isCharInArray(zgadniete,size1,buffer[0])){
       zgadniete[k]=buffer[0];
       int numOccurrences = 0;
//sprawdzanie, czy występują powtórzenia liter
       int* occurrences= find_character(ack1,buffer[0],&numOccurrences); 
       if (numOccurrences == 1){ //brak powtórzzeń
            replace_one(wysalanie,"_",buffer[0],occurrences[0]);
       }
       else{ //powtórzenia
            replace(wysalanie,"_",buffer[0],occurrences);
            powtorzenia++;
       }
       free(occurrences);
       k++;
    }
//
      memset(buffer, 0, sizeof(buffer)); 

//jeżeli zgadnięto pełne hasło - pojedynczymi literami
      if (k==size1-powtorzenia){
        char message[100];  
        sprintf(message, "zgadłes haslo! \nhasło to: %s\n", ack1);
        rc = send(client, message, strlen(message) + 1, 0);
        close(client);
      break;
      } 
//jeżeli zgadnięto jedną literę
      char message[100];
      sprintf(message,"litera się znajduje w haśle \nPozostała liczba prób: %d\nPozostale znaki: %s\nHasło ma %d liter \n",liczba_prob,wysalanie,size1);
      rc = send(client,message, strlen(message) + 1, 0);
    }
//jeżeli litera jest błędna
    else{
      liczba_prob--;
      char messageb[100];
      sprintf(messageb,"litera nie znajduje się w haśle \nPozostała liczba prób: %d\nPozostale znaki: %s\nHasło ma %d liter \n",liczba_prob,wysalanie,size1);
      rc = send(client,messageb, strlen(messageb) + 1, 0);
      
    }
    if(rc<0) {
      perror("cannot send acknowledgment ");
      close(client);
      break;
    }
  }
}
//po wygranej/przegranej
  close(sd);
  return 0;
}

//sprawdza, czy litera jest w słowie
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

//zastępuje jeden znak w haśle (wykorzystywane w __)
void replace_one(char* str, const char* old, char new, int index) {
    size_t old_len = strlen(old);
    
    if (index >= 0 && index < strlen(str)) {
        if (memcmp(str + index, old, old_len) == 0) {
            str[index] = new;
        }
    }
}
//jak wyżej, tylko kilka
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

//znajduje na jakim miejscu w stringu jest znak
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
    

        indices = (int*)malloc(length * sizeof(int));
        int index = 0;
        
        // Zapisywanie indeksów znalezionych znaków
        for (i = 0; word[i] != '\0'; i++) {
            if (word[i] == character) {
                indices[index++] = i;
            }
        }
    
    
    *count = length; // Przekazanie liczby wystąpień przez wskaźnik
    
    return indices;
}

//usuwa powtózenia z liter
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
}

//do interfejsu, tworzy ciąg _
char* createStringWithUnderscores(int length) {
    char* str = malloc((length + 1) * sizeof(char));  // +1 dla znaku null na końcu
    
    if (str == NULL) {
        printf("Błąd alokacji pamięci.\n");
        exit(1);
    }
    
    for (int i = 0; i < length; i++) {
        str[i] = '_';
    }    
    return str;
}

//sprawdza, czy znak jest w tablicy
bool isCharInArray(char arr[], int size, char ch) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == ch) {
            return true;  // Znak jest już w tablicy
        }
    }
    return false;  // Znak nie jest w tablicy
}

//losuje słowa z bazhy
const char* choose_word(const char* nazwa_pliku)
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

//sprawdzanie hasła - w wypadku wpisania całego
bool fullcheck(char arr1[], char arr2[],int size) {
    

    for (int i = 0; i < size; i++) {
        if (arr1[i] != arr2[i]) {
            return false;
        }
    }

    return true;
}

//HELP
bool HELP(char arr[]){
char wzor[]="HELP";
     for (int i = 0; i < 4; i++) {
        if (arr[i] != wzor[i]) {
            return false;
        }
    return true;
    }
}