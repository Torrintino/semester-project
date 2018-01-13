# Packages

Es werden zur Kompilierung die Packages 'libprotobuf-dev', 'protobuf-compiler' und 'liblirc-dev'
benötigt.
Zur Laufzeit braucht es die Packages 'libprotobuf10' und 'liblirc0'.

# Kompilierung und Installation

Das Ausführen des Befehls 'make' in diesem Verzeichnis erzeugt das Programm 'hardware-api', welches
die Middleware zwischen Hardware und Services darstellt. Dabei wird auch die Datei
'../driver/IrRecieveD.c' kompiliert. Es werden 'gcc' und 'g++' aufgerufen. Sollte eine
Cross-Kompilierung notwendig sein, so müssen die Variablen 'CXX' und 'CC' in der Makefile
entsprechend gesetzt werden.

Das resultierende Programm kann in jedem Verzeichnis ausgeführt werden.
Da die Kommunikation mit der Services-Komponente über Dateien im Verzeichnis '/var/run' läuft,
benötigt das Programm zur Ausführung Root-Rechte.

# Ausführung

Alle Ausgaben des Programms (Logs und Fehlermeldungen) erfolgen auf stderr.

Wird das Programm mit dem Argument 'simulate' gestartet, dann werden im Durchschnitt alle 7 Sekunden
zufällige IR-Daten (von 2 bis einschl. 4) an Services weitergegeben, als wenn sie von dem IR-Sensor
empfangen worden wären. Dies dient zum Testen des Zusammenspiels von Hardware und Services.

Anmerkung: Für den umgekehrten Fall (eine Simulation von Services, um die IR-Funktionalität zu
testen) befinden sich im Unterverzeichnis 'stub' Testprogramme.
