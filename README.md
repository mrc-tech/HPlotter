# HPlotter
Harry Plotter


## ToDo

- [x] Versione nell'"About" (https://semver.org)
- [ ] mettere le opzioni
	- [x] margini
	- [x] spessore linee
	- [ ] tema scuro (per ora solo placeholder)
	- [ ] moltiplicatore testo
	- [ ] griglia (principale e secondaria)
	- [ ] legenda
	- [ ] lineplot o scatter
- [ ] mi piacerebbe poter compilare anche una versione per linux. Anche se lo vedo difficile, dato che questo codice è specializzato per windows.
- [ ] vede automaticamente se la prima riga sono delle lettere, in caso le considera come label dei dati.
- [ ] le righe che iniziano con "#" sono dei commenti e non vengono considerate
- [ ] file troppo grandi potrebbero rallentare notevolmente il programma: fare che prende solamente alcuni dati dai file, ovvero solamente quelli necessari a disegnare un grafico
- [ ] mettere opzioni tipo File->Apri, o anche che salva in altri formati oltre il DXF, come png, pdf, svg ....
- [x] l'icona piccola (nel titolo della finestra) non è corretta. Forse devo modificare il file .ico (ancora non mi convince quella di sotto)
- [ ] drag and drop dei file sulla schermata del programma per aprire il file
- [x] dare la possibilità di cambiare lo spessore e il colore delle linee con una specie di Dialog con le opzioni
- [ ] sistemare meglio i numeri che risultano un po troppo grossi e ingombranti (i numeri dei limiti)
- [x] Mouse che passandoci sopra ti dice il valore facendo lo "snap" alla curva. Meglio come matplotlib (coordinate nella barra di controllo inferiore)