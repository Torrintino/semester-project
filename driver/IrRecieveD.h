#ifndef CODINGTAG_HARDWARE_DRIVER_IRRECIEVED_H_HEADER_GUARD_
#define CODINGTAG_HARDWARE_DRIVER_IRRECIEVED_H_HEADER_GUARD_ 1

#include <lirc/lirc_client.h>
#include <fcntl.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif


//ACHTUNG: char* code ist nur ein Array pointer, den das Hauptprogramm aufsetzen sollte,
//	   aus Gründen der Effizienz und meiner eigenen Unfähigkeit


//initialisiert den lircd-Socket
// Returns:
//   SocketID (Success)
//   -1 (Failure)
int initReceiver();
	
//Liest den Input vom Socket aus, sollte im Hauptprogramm 
// in der Loop laufen (Verbrauch von Max 20 Ticks beim Empfangen von Daten)
//bei keinem Input, -1 bei Fehler, sonst Spielernummer
int readInput(char *code);

//Leert falls notwendig(Bsp. nach Timeout wegen Treffer)
int flushReceiver(char*code);
//Disconnect vom Socket, sobald nicht mehr nötig
int deinitReceiver();
//Code senden, blockt, aber bei 16bit kein Problem
int sendCode(int sendPlayer);
//Button init
int setupButtons(int Buttonnummer);
//Ist Button gedrückt? Ja 1 sonst 0;
int isPressed(int Buttonnummer);

#ifdef __cplusplus
}
#endif

#endif // CODINGTAG_HARDWARE_DRIVER_IRRECIEVED_H_HEADER_GUARD_
