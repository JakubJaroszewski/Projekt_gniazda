/* fpont 12/99 */
/* pont.net    */
/* tcpClient.c */
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
typedef enum { false, true } bool;
bool GAME(char arr[]);
bool EXIT(char arr[]);

int main (int argc, char *argv[]) { /* licznik argumentow, tablica argumentow */

  const int SERVER_PORT=1500;

  int sd, rc;
  struct sockaddr_in localAddr, servAddr;
  struct hostent *h;
  
  if(argc < 2) {
    printf("usage: %s <server>\n",argv[0]);
    exit(1);
  }

  h = gethostbyname(argv[1]); //zamiana nazwy domenowej na adres IP maszyny
  if(h==NULL) {
    printf("%s: unknown host '%s'\n",argv[0],argv[1]);
    exit(1);
  }

  servAddr.sin_family = h->h_addrtype;
  memcpy((char *) &servAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
  servAddr.sin_port = htons(SERVER_PORT);

  /* create socket */
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if(sd<0) {
    perror("cannot open socket ");
    exit(1);
  }

  /* connect to server */
  rc = connect(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
  if(rc<0) {
    perror("cannot connect ");
    exit(1);
  }

  char bufor[500];
  char bufor1[500];
  char zgadniete[500];
  int k =0;
  printf("Gra zgadywanie słów. Wpisz literę, by sprawdzić czy jest w haśle.  (wpisz znak / aby wyjść, HELP by wywołać pomoc):\n");
  
  while (1){
	  fgets(bufor, 500, stdin);
	  
	  if (bufor[0] == '/')
		break;
	  
      if(EXIT(bufor)){
            printf("KONIEC");
         close(sd);
        break;}


	  rc = send(sd, bufor, strlen(bufor)+1, 0);
	  
	  if(rc<0) {
	    perror("cannot send data ");
	    close(sd);
	    exit(1);
	  }

	  //printf("Wiadomość \"%s\" została wysłana do serwera.\n", bufor); //zostaje wyslana litera

    strcpy(bufor1, bufor); // do zmiennej bufor1 zapisujemy litere 
    //zapisac bufor do innej zmiennej
    /*OK*/
    //printf("znak: \"%s\" cos \n", bufor);
    //printf("znak: \"%s\" cos \n", bufor1);
	  memset(bufor, 0, sizeof(bufor));  // wyzerowanie bufora, aby nie wysyłać poprzedniej wiadomości
    /*OK*/
    //printf("znak: \"%s\" cos \n", bufor);
    //printf("znak: \"%s\" cos \n", bufor1);
	  
	  rc = recv(sd, bufor, sizeof(bufor), 0); //otrzymuemy nasze haslo do zgadniecia 
    /*OK*/
    //printf("znak: \"%s\" cos \n", bufor);
    //printf("znak: \"%s\" cos \n", bufor1);

  

	  if(rc<0) {
	    perror("cannot receive data ");
	    close(sd);
	    exit(1);
	  }

	  printf("Odpowiedź serwera: %s\n", bufor);
	  memset(bufor, 0, sizeof(bufor)); // wyzerowanie bufora
  }

  close(sd);
  return 0;
  
}


bool GAME(char arr[]){
char wzor[]="GAME";
     for (int i = 0; i < 4; i++) {
        if (arr[i] != wzor[i]) {
            return false;
        }
    return true;
    }
}

bool EXIT(char arr[]){
char wzor[]="EXIT";
     for (int i = 0; i < 4; i++) {
        if (arr[i] != wzor[i]) {
            return false;
        }
    return true;
    }
}