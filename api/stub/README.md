Diese Stubs dienen zum Simulieren der Services-API. Auf den Clients läuft ein entsprechender Prozess
(„client-stub“), der die von der Hardware-API via Unix-Sockets gesendeten Daten empfängt und an den
Server weitergibt. Auf dem Server läuft ein Prozess („server-stub“), der auf stdout anzeigt, welche
Daten welcher Client empfangen hat.

Im Verzeichnis „server“ befindet sich der Code für den Server. Im Verzeichnis „client“ befindet sich
der Code für den Client.

Außerdem befindet sich im Verzeichnis „led-trigger“ ein Programm, mit denen Nachrichten zum Triggern
von LED-Events an die Hardware-API gesendet werden können, die normalerweise von Services stammen.

Fehlermeldungen und Logs werden auf stderr geschrieben. Wenn Daten empfangen werden oder sich ein
Client verbindet, so wird dies auf stdout geschrieben.
