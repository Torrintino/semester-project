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

Wie oben bereits beschrieben, war für das Nachbilden von Lasertag eine drahtlose Übertragungsmethode notwendig, die möglichst zuverlässig ist. Im realen Lasertag wird dafür Infrarot verwendet und es werden LED's benutzt, um den Abschuss darzustellen. Auch in diesem Projekt fiel die Wahl auf Infrarot, es ist im Gegensatz zu Laser nicht schädlich für die Augen und es können über eine größere Distanz zielgerichtet gesendet werden. Es gibt kostengünstige Komponenten wie LED's und Sensoren, sowie Open Source Treiber für die Modellierung für Daten. Es reicht nicht aus, einfach nur zu erkennen, dass man getroffen wurde, weil es für die Spielregeln notwendig ist zu wissen, von welchem Spieler der Schuss kam. Diese Notwendigkeit ergibt sich daraus, das zwei Systeme die über das Netzwerk kommunizieren, dazu in der Lage sein müssen sich eindeutig zu identifizieren. Daher war es sehr nützlich, dass über Infrarot Daten versendet werden können, wie zum Beispiel bei Fernbedienungen. Die gegenseitige Identifizierung haben wir also so gelöst, dass das Spielgerät eines Spielers beim Abschuss eine ID versendet. Wenn der Sensor eines Spielers diese ID empfängt, weiß das Gerät von wem der Schuss kam und es weiß ebenso seine eigene ID. Daher hat es eine Information der Form: Spieler X hat Spieler Y getroffen. Diese Information wird an die Services Komponente weitergegeben, da hardwaretechnisch die Arbeit erledigt ist.


## Komponenten

Aus der Anforderungsanalyse ergeben sich drei abstrakte Komponenten:
 1. Die Spielgeräte und hardwarebasierte Infrastruktur
 2. Kommunikation der Spielgeräte und softwareseitige Infrastruktur
 3. Das Entwerfen eines Spielkonzepts und die Umsetzung in Software

Es handelt sich hierbei, um ein System mit mehreren Schichten, wobei jede Schicht Dienste für die darüberliegende Schicht anbietet. Man kann feststellen, dass die Trennung der Komponenten nicht sauber ist. Es ist zum Beispiel unklar, wo die hardwarebasierte Infrastruktur in die softwarebasierte übergeht, etwa bei Netzwerkprotokollen.

Bei der Arbeit am Semesterprojekt haben wir den Komponenten entsprechend die Teilnehmer in drei Gruppen eingeteilt, welche abgekürzt als "Hardware", "Services" und "Spielelogik" bezeichnet werden.

### Hardware

### Services

### Spielelogik

## Zusammenfassung

### Auswertung

### Ausblick
