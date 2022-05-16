# ObjectTracker
Il progetto nasce con l'idea di realizzare un'applicazione che sia in grado di riconoscere uno o più oggetti presenti all'interno di un frame video, in tempo reale. L'obiettivo è quello di avere una telecamera connessa sia al pc sia ad una componente hardware come Arduino. Una volta che il sistema riconosce uno o più oggetti deve essere in grado centrarli rispetto alla telecamera, simulando di fatto il comportamento di una telecamera PTZ tramite software.

In particolare, questo significa che l'applicazione dovrà stimare la posa dell'oggetto d'interesse nello spazio 3D e utilizzare queste informazioni per ruotare la scena attorno agli assi principali di una telecamera PTZ, ovvero quelli di pan e tilt.

Le esigenze del committente prevedono la possibilità di calibrare la telecamera o di caricare una calibrazione precedentemente salvata all'interno del dispositivo.

L'intero sistema è affiancato da feedback visivi differenti derivanti dalle periferiche di output montate su Arduino Uno.

Il progetto realizzato si compone di:
- una parte utile per la calibrazione della telecamera;
- una parte utilizzata per l'acquisizione video ed il riconoscimento di un oggetto;
- una parte che consenta di centrare l'oggetto rispetto all'asse ottico della telecamera;
- una parte utile per la comunicazione hw-sw.
