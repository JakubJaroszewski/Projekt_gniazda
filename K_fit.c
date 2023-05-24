//Paulina Rudzka i Jakub Jaroszewski
//sieci komputerowe 2023
//kod bazujący na programie wykonania: W.Bajdecki 


#include <sys/types.h> //typy danych
#include <sys/socket.h> //gniazda
#include <netinet/in.h> //adresy sieciowe
#include <arpa/inet.h> //adresy sieciowe
#include <netdb.h> //obsluga hostow
#include <stdio.h> //wejscie/wyjscie std
#include <stdlib.h> //ogólne 
#include <unistd.h> /* close */

int main (int argc, char *argv[]) { /* licznik argumentow, tablica argumentow */

//inicjalizuje stale
  const int SERVER_PORT=1500;
  int sd, rc;
  struct sockaddr_in localAddr, servAddr;
  struct hostent *h;
  
//zabezpieczenie przed złym wywołaniem
  if(argc < 2) {
    printf("usage: %s <server>\n",argv[0]);
    exit(1);
  }

//zmiana nazwy domenowej na adres IP maszyny
  h = gethostbyname(argv[1]); 
  if(h==NULL) {
    printf("%s: unknown host '%s'\n",argv[0],argv[1]);
    exit(1);
  }

  servAddr.sin_family = h->h_addrtype;
  memcpy((char *) &servAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
  servAddr.sin_port = htons(SERVER_PORT);

//tworzenie gniazda
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if(sd<0) {
    perror("cannot open socket ");
    exit(1);
  }

 //łączenie z serwerem
  rc = connect(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
  if(rc<0) {
    perror("cannot connect ");
    exit(1);
  }
 
 //powitanie nowego gracza
  printf("Gra zgadywanie słów. Wpisz literę, by sprawdzić czy jest w haśle.  (wpisz znak / aby wyjść, HELP by wywołać pomoc):\n");
  

  while (1){


//wczytanie znaku od klienta
	  fgets(bufor, 500, stdin);
	  
//koniec jeżeli /
	  if (bufor[0] == '/')
		break;

//wysłanie litery/hasła do serwera
	  rc = send(sd, bufor, strlen(bufor)+1, 0);
	  if(rc<0) {
	    perror("cannot send data ");
	    close(sd);
	    exit(1);
	  }
  
//odpowiedź serwera
	  rc = recv(sd, bufor, sizeof(bufor), 0); 
	  if(rc<0) {
	    perror("cannot receive data ");
	    close(sd);
	    exit(1);
	  }
	  printf("Odpowiedź serwera: %s\n", bufor);
	  memset(bufor, 0, sizeof(bufor)); // wyzerowanie bufora
  }

//zakończenie
  close(sd);
  return 0;
  
}


