# Installationsanleitung für Clients

Dieses Dokument beschreibt, wir ihr einen komplett neuen Raspberry Pi
installiert. Dabei handelt es sich um die Standardkonfiguration für unsere
Clients, wenn ihr also in diesem Dokument etwas findet, was nicht Teil des
Standards sein sollte, schreibt bitte einen entsprechenden Kommentar.

Vorraussetzung:
 * Besitz eines Raspberry Pi
 * Einen Linux Rechner (Mac/Windows wird nicht behandelt, bitte erweitert selber
   	       	        dieses Tutorial, wenn ihr den Use Case habt)
 * Einen funktionierenden SD card reader
 * Kenntnisse im Umgang mit der Kommandozeile

Anleitung:
 1. Downloaded [https://www.raspberrypi.org/downloads/raspbian/](Raspbian Lite)
 2. Verbindet einen SD card reader mit eurem PC, steckt die SD Karte ein
 3. Ihr braucht den device Pfad eures SD card readers. Ich empfehle die Verwendung von `lsblk` dafür. Bei mir heißt er zum Beispiel `/dev/mmcblk0`
 4. Führt `umount` für alle Partionen der SD card aus, die gemountet sind
 5. Geht in den Download Ordner und führt folgendes Kommando aus
 `dd if=<version>-raspbian-strech-lite.img of=<device Pfad> bs=1M`
 6. Führt `sync` aus
 7. Gegebenfalls noch mal `umount` ausführen und SD Karte herausnehmen
 8. SD Karte in den Raspberry Pi stecken und einschalten
 9. Benutzername: `pi`, Passwort: `raspberry` (Bitte erstmal nicht ändern)
 10. Führt `sudo apt update` und `sudo apt upgrade` aus (Bei mir gab dpkg einen
     Fehler zurück, sollte nicht kritisch sein, liegt womöglich an einem date
     bug)
 11. Führt `sudo raspi-config` aus und ändert die Lokalisierung auf deutsche
     Settings
 12. Ändert den hostnamen zu client-x, wobei x die Client ID ist. Wenn der RasPi
     noch keine Client ID hat, die bitte in Client ID Liste eintragen
 13. Unter Interfacing Options ssh aktivieren

Eventuell ist `apt --fix-broken install` notwendig

Als nächstes richten wir git ein:
 1. `sudo apt install git`
 2. `git clone https://gitlab.informatik.hu-berlin.de/coding-tag/hardware.git`

Jetzt kann man direkt auf dem Raspberry Pi arbeiten und auf alle Skripts
zugreifen.