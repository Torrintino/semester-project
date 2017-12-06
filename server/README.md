# Server

## Installation

Die Server Installation wird durch Puppet automatisiert, allerdings müssen einige Schritte manuell durchgeführt werden.

 1. Raspbian Lite Image auf die SD Karte schreiben
 2. `sudo raspi-config` lässt sich leider schwerlich automatisieren. Man könnte die einzelnen Konfigurationskommandos einzeln ausführen, dass ist derzeit aber nicht den Aufwand wert. Die einzelnen Schritte:
   * Network: Hostname auf `server-1` setzen
   * Localisation: Alles auf deutsche Einstellungen
   * Interfacing: SSH Server aktivieren
   * reboot
 3. Puppet installieren
   * sudo apt install puppet
   * der Server übernimmt die Rolle des Agents, der Puppet Master läuft auf meinem Rechner und der Hostname wird mit `hostname` bezeichnet
   * Konfiguration testen `sudo puppet agent -t --server hostname`
   * reboot
   * in /etc/puppet.conf den Hostnamen vermerken