Dieser Stub dient zum Simulieren der Services API. Auf dem Clients läuft ein entsprechender Prozess
(gerne auch als Daemon), der die von der Hardware-API via Unix-Sockets gesendeten Daten empfängt und
an den Server weitergibt. Auf dem Server läuft ein Prozess, der auf der Konsole anzeigt, welche
Daten welcher Client empfangen hat.

Im Verzeichnis „server“ befindet sich der Code für den Server. Im Verzeichnis „client“ befindet sich
der Code für den Client.
