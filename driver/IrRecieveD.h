#include "IrRecieveD.h"
#include <lirc/lirc_client.h>
#include <fcntl.h>
#include <string.h>

//ACHTUNG: char* code ist nur ein Array pointer, den das Hauptprogramm aufsetzen sollte,
	   aus Gründen der Effizienz und meiner eigenen Unfähigkeit


//0,wenn erfolgreich, initialisiert den lircd-Socket
int initReciever();
	
//Liest den Input vom Socket aus, sollte im Hauptprogramm 
// in der Loop laufen (Verbrauch von Max 20 Ticks beim Empfangen von Daten)
int readInput(char *code);

//Leert falls notwendig(Bsp. nach Timeout wegen Treffer)
int flushReciever(char*code);
//Disconnect vom Socket, sobald nicht mehr nötig
int deinitReciever();
//Code senden, blockt, aber bei 16bit kein Problem
int sendCode(int sendPlayer);
