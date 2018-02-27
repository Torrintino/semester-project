Dieses Dokument ist ein Entwurf für den Abschlussbericht aus Sicht von
Lennart Weiß von der Hardware Gruppe.

# Semesterprojekt Mobile Systeme

## Inhaltsverzeichnis

 1. Einleitung
 2. Anforderungsanalyse
 3. Systemarchitektur
 4. Komponenten
     1. Hardware
     2. Services
     3. Spielelogik
 5. Zusammenfassung
     1. Auswertung
     2. Ausblick

## Einleitung

Das Semesterprojekt "Mobile Systeme" wurde vom Lehrstuhl der Technischen
Informatik an der Humboldt Universität zu Berlin organisiert und umfasste
12 studentische Teilnehmer. Das Projekt begann im Oktober 2017 und die
Endpräsentation fand am 16.02.2018 statt.

Das Ziel war es das populäre Spiel Lasertag mit modifizierten Regeln
nachzubilden. Die besondere Anforderung war dabei, dass die Spielart der
Funktionsweise eines Peer-to-Peer Netzwerkprotokolls entsprechen sollte.
Der Entwurf und die Funktionsweise des technischen Systems, das dies
bewerkstelligt, wurde komplett den Teilnehmern überlassen.

## Anforderungsanalyse

Bereits Forderung, dass das Spiel Lasertag nachgebildet werden soll, hatte
weitreichende Konsequenzen. Wie im original muss jeder Spieler ein Gerät
bekommen, mit dem er andere Spiel abschießen kann und entsprechend muss
ein Abschuss detektiert werden. Die Spieler sollen dabei möglichst mobil
und frei beweglich sein. Daraus resultiert, dass irgendeine Form von
Sender/Empfänger Kombination verwendet werden muss.

Implizit war auch der Spielspaß eine Anforderung. Dies ist weniger eine technische Anforderung, es verlangt eher, dass die technische Funktionsweise des Systems nicht nur demonstriert werden kann, sondern auch ausgereift und
zuverlässig ist. Das Spiel sollte so aufbereitet sein, dass jemand mit möglichst wenig Vorkenntnissen ein intuitives Verständnis entwickeln kann. Da das Spiel als "Lasertag Nerd Style" bezeichnet wird, können von der primären Zielgruppe zumindest grundlegende Vorkenntnisse zu Netzwerkpotokollen erwartet werden. Dennoch muss das P2P-Protokoll der Wahl in die Form eines spielgerechten Regelwerks transformiert werden.

Es wurden keine genaueren Anforderungen an die Spielgeräte gestellt, daher
kann sich das Team eine beliebige Architektur aussuchen, die das Spiel
unterstützt. Um den Spielspaß zu gewärleisten, muss jedoch die Übertragung
von Treffern zuverlässig sein und die Geräte sollten möglichst handlich und leicht zu bedienen sein. Die Spieler brauchen insbesondere eine Möglichkeit Feedback darüber zu erlangen, ob sie einen Spieler getroffen haben und wie das Spiel derzeit verläuft. Auch gilt, dass eine möglichst bequeme Lösung für den Spieler den Spaß unterstützt.

Aus den Anforderungen für Bequemlichkeit ergibt sich, dass drahtlose Kommunikationsmethoden verwendendet werden sollten und die Spielgeräte über Akkumulatoren betrieben werden sollten. Die Sensoren sollten entweder am Spielgerät befestigt sein, oder es sollte möglichst einfach am Körper anbringbar und robust sein.

Da drahtlose Kommunikation verwendet wird und das Spiel möglichst reibungslos ablaufen soll, muss insbesondere bedacht werden, dass es zu Störungen kommen könnte und somit Informationen verloren gehen. Es ist also notwendig, dass die Geräte nicht von einem unzuverlässigen Übertragungskanal ausgehen und mit daraus resultierenden Problemen umgehen können. Es ist außerdem zu beachten, dass es zu Schwierigkeiten mit konkurrierenden Prozessen kommen kann, weil z.B. Signal X schneller als Signal Y empfangen wurde, obwohl Signal Y früher abgeschickt wurde.


## Systemarchitektur

In diesem Abschnitt wird das geplante System beschrieben mit dem die Gruppe die Anforderungen erfüllen möchte.

### Die Spielgeräte

Wie oben bereits beschrieben, war für das Nachbilden von Lasertag eine drahtlose Übertragungsmethode notwendig, die möglichst zuverlässig ist. Im realen Lasertag wird dafür Infrarot verwendet und es werden LED's benutzt, um den Abschuss darzustellen. Auch in diesem Projekt fiel die Wahl auf Infrarot, es ist im Gegensatz zu Laser nicht schädlich für die Augen und es können über eine größere Distanz zielgerichtet gesendet werden. Es gibt kostengünstige Komponenten wie LED's und Sensoren, sowie Open Source Treiber für die Modellierung für Daten. Es reicht nicht aus, einfach nur zu erkennen, dass man getroffen wurde, weil es für die Spielregeln notwendig ist zu wissen, von welchem Spieler der Schuss kam. Diese Notwendigkeit ergibt sich daraus, das zwei Systeme die über das Netzwerk kommunizieren, dazu in der Lage sein müssen sich eindeutig zu identifizieren. Daher war es sehr nützlich, dass über Infrarot Daten versendet werden können, wie zum Beispiel bei Fernbedienungen.

Die gegenseitige Identifizierung wurde so gelöst, dass das Spielgerät eines Spielers beim Abschuss eine ID versendet. Wenn der Sensor eines Spielers diese ID empfängt, weiß das Gerät von wem der Schuss kam und es weiß ebenso seine eigene ID. Daher hat es eine Information der Form: Spieler X hat Spieler Y getroffen.

Als Gerät fiel die Wahl auf den Raspberry Pi Model Zero W. Die Entscheidung wurde getroffen, weil ein leichter, stromsparender Computer benötigt wurde, der freiprogrammierbar ist und mit Infrarotkomponenten erweitert werden kann. Für den kabellosen Betrieb werden Powerbanks verwendet, um die Geräte mit Strom zu versorgen.

### Netzwerkarchitektur

Informationen über Abschüsse müssen eine entsprechende Auswirkung auf den Spielstand haben. Eine Option dafür wäre die selbe Applikation auf jedem Gerät zu installieren und ein P2P Netzwerk zu errichten, in dem sich die Geräte konstant über den gegenwärtigen Spielstand austauschen. Daraus resultiert jedoch ein sehr kompliziertes Protokoll zur Synchronisierung, insbesondere weil ein Gerät mitten im Spiel die Verbindung verlieren könnte. Stattdessen wurde eine simplere Client-Server Architektur gewählt, in der Clients lediglich Informationen sammeln und diese an den Server weiterreichen. Der Server wertet sie aus und gibt Informationen über den Spielstand an die Clients zurück. Durch diese Architektur werden viele potenzielle Logikfehler, die sich durch konkurrente Nachrichten ergeben, eliminiert.

Die Clients und der Server verwenden zur Kommunikation WLAN. Dies ist die naheliegendste Lösung, die Technik ist zuverlässig und Verbindungsprobleme werden größtenteils durch den TCP/IP Stack des Betriebssystems gelöst. Um ein weitaufspannendes Feld zu errichten wäre es notwendig mehrere Access Points (AP) zu errichten, die sich mit Roaming austauschen. Es wurde allerdings entschieden, dass dies den Rahmen des Projekts sprengen würde und es wurde der Einfachheit halber entschieden, den Server auch gleichzeitig als AP einzusetzen, womit theoretisch ein Spielfeld unterstützt wird, dass mehrere Räume umspannt.

Für den Server wurde ebenfalls ein Raspberry Pi (Model B+) verwendet, ein Desktop PC wäre jedoch genauso geeignet gewesen. Der Raspi wurde verwendet, weil es für die Entwicklung praktisch war einen Minicomputer zu haben, der leicht zu transportieren ist und als WLAN AP verwendet werden kann.

### Das Spiel

Zuvor wurde die grundlegende Infrastruktur beschrieben, mit der Informationen über Abschüsse ausgetauscht werden können. Das allein reicht für das Spiel noch nicht aus, weil die Spieler eine Möglichkeit brauchen eine Übersicht über den Spielstand zu behaupten. Dafür gibt es zwei Ansätze die verfolgt wurde: Eine Anzeige auf jedem Gerät über den Status des Spielers und eine Anzeige für den Server über den aktuellen Spielstand.

Die Anzeige für die Spielgeräte wurde mit LEDs gelöst, die mit verschiedenen Farben anzeigen, ob ein anderer Spiel getroffen wurde, ob man unverwandbar ist, etc. Dies ist eine billige Möglichkeit, die durch die GPIO Anschlüsse der Raspberry Pis unterstützt wird.

Die Anzeige des Servers wurde mit einer Website umgesetzt die tabellarische das Scoreboard anzeigt. Die Webseite soll auf einem oder mehreren Bildschirmen dargestellt werden, die auf das gesammte Spielfeld verteilt werden.

In diesen Entscheidung spiegelt sich wieder, dass Spieler regelmäßig erfahren wollen, ob sie noch im Spiel sind oder ob sie einen anderen Spieler getroffen haben. Daher muss es eine Möglichkeit geben die entsprechende Anzeige sehr schnell und einfach zu überprüfen. Der aktuelle Spielstand muss seltener angesehen werden, daher ist es in Ordnung, wenn der Spieler einige Sekunden braucht, um auf die Website zu schauen.

Letztendlich muss auch auf Fairness geachtet werden, daher ist es wichtig dass die Fähigkeit andere Spieler abzuschießen limitiert ist. Dies muss auf mehrere Weisen geschehen:

 1. Die Streuung von Abschüssen muss eingeschränkt sein. Dies wurde gelöst, indem Strohhalme über die LEDs gestülpt wurden, die das Aussenden des Signals in eine bestimmte Richtung lenken.
 2. Ein Spieler darf nicht konstant schießen, es muss also eine Art Abklingzeit geben. Dafür wurde an den Geräten ein Button angebracht, sodass das Gerät nur schießt, wenn der Button gedrückt wurde und nicht mehr schießen kann, bis er wieder losgelassen wurde.
 3. Damit ein Spieler nicht mehrmals hintereinander getroffen werden kann, gibt es bei vielen Spielmodi eine kurze Unverwundbarkeitszeit, nachdem ein Spieler getroffen wurde. In dieser Zeit hat der getroffene die Möglichkeit sich so zu positionieren, dass er nicht erneut getroffen wird.

### Die Spielmodi

Es wurde entschieden, dass die Spielelogik möglichst vom Backend getrennt sein soll. Deswegen wurde sie in Form von LUA Skripts gekapselt und wird vom Server eingebunden. Die Schnittstelle zwischen diesen beiden Komponenten ist ein Informationsaustausch. Der Server gibt Spielparameter und Abschussinformationen an LUA weiter und die Spielelogik gibt die resultieren Auswirkung auf den Spielstand zurück, damit dieser auf der Website und den Geräten angezeigt werden kann.

Als zu spielendes Netzwerkprotokoll haben wir eine vereinfachte Version von Bittorrent gewählt. Diese Wahl fiel unter anderem, da dieses Original auch in der Realität spielerische Elemente bietet. So gibt es im Netzwerk etwa Teilnehmer, die möglichst viel Datenvolumen von anderen beziehen wollen, ohne selber eine entsprechende Upload-Bandbreite anzubieten. Dieses Element der Fairness sollte eine zentrale Komponente im Spiel sein.

Tatsächlich wurden mehrere Spielmodi implementiert. Dies geschah aus verschiedenen Gründen:
 1. Es zwang uns eine modulare Architektur zu errichten, die völlig unabhängig vom gewählten Spielmodus funktioniert. Dadurch habe wir eine garantierte Flexibiltät Änderungen an Spielmodi vornehmen zu können.
 2. Die Spielmodi haben eine unterschiedliche Reichhaltigkeit an Funktionen. Dies ist nützlich für das Debugging, weil somit gezielt Features getestet und gewisse Rahmenbedingungen je nach Spielmodus vernachlässigt werden können.

## Komponenten

Aus der Anforderungsanalyse ergeben sich drei abstrakte Komponenten:
 1. Die Spielgeräte und hardwarebasierte Infrastruktur
 2. Kommunikation der Spielgeräte und softwareseitige Infrastruktur
 3. Das Entwerfen eines Spielkonzepts und die Umsetzung in Software

Es handelt sich hierbei, um ein System mit mehreren Schichten, wobei jede Schicht Dienste für die darüberliegende Schicht anbietet. Man kann feststellen, dass die Trennung der Komponenten nicht sauber ist. Es ist zum Beispiel unklar, wo die hardwarebasierte Infrastruktur in die softwarebasierte übergeht, etwa bei Netzwerkprotokollen.

Bei der Arbeit am Semesterprojekt haben wir den Komponenten entsprechend die Teilnehmer in drei Gruppen eingeteilt, welche abgekürzt als "Hardware", "Services" und "Spielelogik" bezeichnet werden. In den folgenden Abschnitten werden die jeweiligen Gruppenmitglieder aufgezählt und die Arbeitsergebnisse derselben erläutert. 

### Hardware

### Services

### Spielelogik

## Zusammenfassung

### Auswertung

### Ausblick
