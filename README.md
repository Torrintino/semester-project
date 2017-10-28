# Willkommen bei der Hardwaregruppe

Hier ein Vorschlag für die Verzeichnistruktur:
 * /doc enthält allgemeine Dokumentation
 * /client enthält Konfigurationsdateien, Scripts, etc. für den Client Raspberry Pi
 * /server enthält das selbe für den Server
 * /treffer enthält Code, um Informationen über Treffer durch Socket
   	    Programmierung zu übertragen
 * /driver enthält etwaige Treiber

Einige Worte zur Netzwerk Programmierung:
Alle beteiligten Clients und Server sind in einem privaten Netzwerk.
Die Clients erhalten die Adressen 192.168.1.X, wobei X eine Client ID zwischen 2 und 254 ist.
Die Server erhalten die Adressen 192.168.2.X, wobei X eine Server ID zwischen 2 und 254 ist.

Um Informationen über Treffer auszutauschen existiert bereits ein Client/Server
Modell, welches in Python geschrieben wurde. Damit es richtig funktioniert
müssen Hostnamen und IP-Adressen richtig gesetzt sein. Es wird vermutlich
mit der Zeit durch eine effizientere C-Implementierung ersetzt werden.