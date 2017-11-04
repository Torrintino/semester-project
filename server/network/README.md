# Netzwerkkonfiguration

## Infrastruktur

Es ist noch nicht entschieden, wie unsere Netzwerk Infrastruktur konkret aussehen wird, derzeit haben wir folgendes in Planung:

Es gibt einen zentralen Server, der auch als Access Point fungiert. Auf ihm laufen verschiedene Netzwerkservices wie DHCP, DNS uns NTP. Dadurch wird die Netzwerkkonfiguration an einem zentralen Ort vorgenommen, was sie stabiler und weniger fehleranfällig macht.
Der Server kann mit mehreren Access Points verbunden werden, die sich über Roamind mit den Clients verbinden. Diese Server sind als Bridges konfiguriert und tuen nichts weiter, als Daten weiterzuleiten. Sie ermöglichen eine stabilere Verbindung auf größerer Fläche.

### Netzwerkadressen und DNS

Wir verwenden das 192.168.0.0/16 Netz für unsere Adressen. Jeder Server und jeder Client erhält eine eindeutige ID. Diese wird auch für seine jeweilige IP Adresse verwendet.

Die Aufteilung geschieht folgendermaßen:

 * 192.168.1.*
 * 192.168.1.1 - der zentrale Server
 * 192.168.1.2-249 - Clients
 * 192.168.1.250-254 - Testgeräte die als Clients fungieren

 * 192.168.2.* - zusätzliche Server
 * 192.168.2.2-249 - Server
 * 192.168.2.250-254 - Testgeräte die als Server fungieren

Die ID's für Clients und Server sind fest an ein Gerät gebunden. Die Adressen für Testgeräte dürfen beliebig vergeben werden, sie sind für private Geräte gedacht, die letztendlich nicht für das Spielen verwendet werden.
Es kann theoretisch mehrere Geräte für den zentralen Server geben, es darf davon aber immer nur einer davon innerhalb eines Netzwerks existieren.

## Der zentrale Server

Hier ist eine Liste an Features, die derzeit im Server implementiert sind:
 * hostapd - Daemon für Access Points
 * dnsmasq - DNS und DHCP (statisch)
 * systemd network (für die statische IP)

Wir haben uns noch nicht auf ein Betriebssystem geeinigt. Es muss Debian basierend sein, systemd unterstützen und eine grafische Oberfläche zur Verfügung stellen.

Für die statische Konfiguration von dnsmasq wird ein Python Skript verwendet. Es liest die CSV Dateien mit den ID's aus und generiert die Config Datei.