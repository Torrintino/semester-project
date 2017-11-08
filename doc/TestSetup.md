# Test Setup Dokumentation

Dieser Artikel beschreibt, wie wir jedem ein einheitliches Test Setup zur Verfügung stellen können.

## Anforderungen

### Grundlegendes

Damit die Services und Spiellogik produktiv arbeiten können, brauchen sie eine Umgebung auf der sie ihre Software testen können. Dies ist im Allgemeinem auf dem eigenen Rechner nicht möglich, da wir eine verteilte Anwendung entwickeln, die eine bestimmt Netzwerkarchitektur erwartet. Wenn sich das jemand zuhause selber einrichten möchte, bringt das nicht nur sein Heimnetzwerk durcheinander, sondern es ist auch fehleranfällig.

Ganz allgemein gesehen muss die Test Umgebung also folgende Bedingungen erfüllen:
 * die Anwendungen von Service und Spielelogik sind darauf ohne Anpassungen ausführbar
 * man kann Tests automatisiert ausführen
 * die Tests liefern Ergebnisse, die möglichst nah an der Realität liegen
 * das Verhalten der Testumgebung ist deterministisch
 * die Testumgebung ist isoliert von der Außenwelt

### Virtualisierung

Damit wir nicht mehrere Maschinen physikalisch zur Verfügung stellen müssen, wollen wir Virtualisierung einsetzen. Dann können auf einem einzigen Rechner ein Server und mehrere Clients laufen. Damit lässt sich auch die Isolierung trivial umsetzen.

Damit die Anwendung aller Gruppen wie erwartet funktionieren, müssen die virtuellen Maschinen auch die selben Schnittstellen zur Verfügung stellen, die in der Realität verwendet werden.

### Netzwerk

Einer der wichtigsten Punkte ist, dass sich die Maschinen im Netzwerk unter denselben Adressen und Hostnamen erreichen können, wie in der Realität. Da aber alles virtuell läuft, reden die Maschinen auch über virtuelle Netzwerkschnittstellen. Daher läuft auch keine Kommunikation über Infrarot oder WLAN ab.

Deshalb müssen Events (z.B. ein Spieler hat den anderen getroffen) simuliert werden. Das heißt, es gibt eine klar definierte Schnittstelle zwischen Hardware und Services, über die ein Treffer kommuniziert wird. Sollen nun automatische Tests ausgeführt werden, muss der Tester selbst die Treffer Events ausführen. In der Realität würde die Hardwaregruppe aus Infrarotsignalen Treffer auswerten und an die Schnittstelle senden.

### Abstraktion

Wir kommen damit zu einem sehr wichtigen Thema: die reale und die Testumgebung sind sehr unterschiedlich, wenn es um die Hardware und Betriebsystemkonfiguration geht. Deshalb muss es streng definierte Schnittstellen zur Hardwaregruppe geben. Die anderen Gruppen können keinerlei Änderungen an der Hardware vornehmen. Änderung an der Betriebssystemkonfiguration ist begrenzt möglich, weil alle Geräte Debian basiert sind. Daher sollten sie lediglich auf Paketinstallation beruhen, sonstige Änderungen sollten mit der Hardwaregruppe abgesprochen werden.

Wenn diese Schnittstellen nun vereinbart sind, macht es für Services und Spielelogik keinen Unterschied, ob die Applikationen auf der Testumgebung ausgeführt werden, oder nicht. Die darunter liegende Plattform ist prinzipiell die selbe. Der einzige Unterschied liegt in Verbindungsproblemen die in der Realität erwartet werden können. Aber auch diese kann man simulieren.
