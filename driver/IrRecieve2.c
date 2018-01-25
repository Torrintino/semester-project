 #include "IrRecieveD.h"
#include <lirc/lirc_client.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

//0,wenn erfolgreich, initialisiert den lircd-Socket
int initReciever(){
	int socketId;
	if((socketId=lirc_init("codingTagR",1))==-1){
		printf("Fehler beim Connecten zu Lirc\n");
		return -1;
	}
	if(fcntl(socketId,F_SETFL,O_NONBLOCK)==-1){
	printf("Fehler beim Blockieren aufheben\n");
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
			return -1;
		}
		else{
			switch(code[24]){
				case'0':
					ret = 2;
					break;
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
					ret = 0;
					break;
				case'6':
					ret = 0;
					break;
				case'7':
					ret = 0;
					break;
				case'8':
					ret = 0;
					break;
				case'9':
					ret = 0;
					break;
				case'A':
					ret = 0;
					break;
				case'B':
					ret = 0;
					break;
				case'C':
					ret = 0;
					break;
				case'D':
					ret = 0;
					break;
				case'E':
					ret = 0;
					break;
				case'F':
					ret = 0;
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
int flushReciever(char*code){
	lirc_nextcode(&code);
	while(code !=NULL){
		lirc_nextcode(&code);
	}
return 0;
}

//Disconnect vom Socket, sobald nicht mehr nötig
int deinitReciever(){
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
		printf("Spieleranzahl überstiegen.");
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
	printf("Keine Verbindung zum Socket möglich");
	return -1;
	}
	printf("%u",fd);
	fflush(stdout);
	if(lirc_send_one(fd,remote,player )==-1){
		printf("Konnte keinen Code senden");
		return -1;
	}
	close(fd);
	return 0;
}
//Nicht blockierendes, permanentes senden.
int send_often(lirc_cmd_ctx ctx){
	int fd= lirc_get_local_socket(NULL,0);
	int r;
	r= lirc_command_init(&ctx,"%s %s %s\n","SEND_START","12","KEY_0");
	do{
	r=lirc_command_run(&ctx,fd);
	if(r!=0 && r!=EAGAIN){
		fprintf(stderr,"Error running command: %s\n", strerror(r));
		}
	}while(r==EAGAIN);
	close(fd);
}
//stoppen von Send_often();
int stop_send_often(lirc_cmd_ctx ctx){
	int fd= lirc_get_local_socket(NULL,0);
	int r;
	r= lirc_command_init(&ctx,"%s %s %s\n","SEND_STOP","12","KEY_0");
	do{
	r=lirc_command_run(&ctx,fd);
	if(r!=0 && r!=EAGAIN){
		fprintf(stderr,"Error running command: %s\n", strerror(r));
		}
	}while(r==EAGAIN);
	close(fd);
}
int main(){
	int k;
	int g;
	char *code;
	lirc_cmd_ctx ctx;
	code = (char*)malloc(sizeof(char));
	g=initReciever();
	printf("%u",g);
	fflush(stdout);
	send_often(ctx);
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
		stop_send_often(ctx);
	}
	deinitReciever();
	return 0;
}

