#include "IrRecieveD.h"
#include <lirc/lirc_client.h>
#include <fcntl.h>
#include <string.h>


//0,wenn erfolgreich, initialisiert den lircd-Socket
int initReceiver(){
	int socketId;
	if((socketId=lirc_init("codingTagR",1))==-1){
		fprintf(stderr,"Fehler beim Connecten zu Lirc\n");
		return -1;
	}
	if(fcntl(socketId,F_SETFL,O_NONBLOCK)==-1){
	fprintf(stderr,"Fehler beim Blockieren aufheben\n");
	return -1;
	}
	return socketId;
}
//Liest den Input vom Socket aus
int readInput(char *code){
	int ret;
	ret = -1;

	if(lirc_nextcode(&code)==0){
		if(code==NULL){
			return 0;
		}
		else{
			switch(code[24]){
				case'1':
					ret = 1;
					break;
				case'2':
					ret = 2;
					break;
				case'3':
					ret = 3;
					break;
				case'4':
					ret = 4;
					break;
				case'5':
					ret = 5;
					break;
				case'6':
					ret = 6;
					break;
				case'7':
					ret = 7;
					break;
				case'8':
					ret = 8;
					break;
				case'9':
					ret = 9;
					break;
				case'A':
					ret = 10;
					break;
				case'B':
					ret = 11;
					break;
				case'C':
					ret = 12;
					break;
				case'D':
					ret = 13;
					break;
				case'E':
					ret = 14;
					break;
				case'F':
					ret = 15;
					break;
				default:
					break;
			}
		}
		free(code);
	}
	return ret;
}
//Leert falls notwendig(Bsp. nach Timeout wegen Treffer)
int flushReceiver(char*code){
	lirc_nextcode(&code);
	while(code !=NULL){
		lirc_nextcode(&code);
	}
return 0;
}

//Disconnect vom Socket, sobald nicht mehr nötig
int deinitReceiver(){
	lirc_deinit();
}
//Code senden, blockt, aber bei 16bit kein Problem
int sendCode(int sendPlayer){

	int fd;
	char player[5];
	char remote[2];
	strcpy(remote,"12");
	strcpy(player,"KEY_");
	if(sendPlayer>15){
		fprintf(stderr,"Spieleranzahl überstiegen.");
		return -1;
	}
	switch(sendPlayer){
		case 1: strcat(player,"1");
			break;
		case 2: strcat(player,"2");
			break;
		case 3: strcat(player,"3");
			break;
		case 4: strcat(player,"4");
			break;
		case 5: strcat(player,"5");
			break;
		case 6: strcat(player,"6");
			break;
		case 7: strcat(player,"7");
			break;
		case 8: strcat(player,"8");
			break;
		case 9: strcat(player,"9");
			break;
		case 10: strcat(player,"A");
			break;
		case 11: strcat(player,"B");
			break;
		case 12: strcat(player,"C");
			break;
		case 13: strcat(player,"D");
			break;
		case 14: strcat(player,"E");
			break;
		case 15: strcat(player,"F");
			break;
	}
	fd= lirc_get_local_socket(NULL,0);
	if(fd<1){
    	fprintf(stderr,"Keine Verbindung zum Socket möglich");
    	return -1;
	}
	if(lirc_send_one(fd,remote,player )==-1){
		fprintf(stderr,"Konnte keinen Code senden");
		return -1;
	}
	return 0;
}
/*Testecke
int main(){
	int k;
	char *code;
	code = (char*)malloc(sizeof(char));
	initReciever();
	sendCode(1);
	while(1==1){
		for(int i=0;i<10000;i++){
			for(int j=0;j<10000;j++){
			k=i+j;
			}
		}
		flushReciever(code);
		for(int i=0;i<10000;i++){
			for(int j=0;j<10000;j++){
			k=i+j;
			}
		}
		k=readInput(code);
		printf("%u",k);
		fflush(stdout);
	}
	deinitReciever();
	return 0;
}
*/