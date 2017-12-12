# Packages

Es werden zur Kompilierung die Packages 'libprotobuf-dev' und 'liblirc-dev' benötigt.
Zur Laufzeit braucht es die Packages 'libprotobuf10' und 'liblirc0'.

# Kompilierung und Installation

Das Ausführen des Befehls 'make' in diesem Verzeichnis erzeugt das Programm 'hardware-api', welche
die Middelware zwischen Hardware und Services darstellt. Dabei wird auch die Datei
'../driver/IrRecieveD.c' kompiliert. Es werden dabei 'gcc' und 'g++' aufgerufen. Sollte eine
Cross-Kompilierung notwendig sein, so müssen die Variablen 'CXX' und 'CC' in der Makefile
entsprechend gesetzt werden.

Das resultierende Programm kann in jedem Verzeichnis ausgeführt werden.
Da die Kommunikation mit der Services-Komponente über Dateien im Verzeichnis '/var/run' läuft,
benötigt das Programm zur Ausführung Root-Rechte.
