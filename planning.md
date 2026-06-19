# Planning Praktijkexamen Microcontrollers

## Doel
- Voorbereiding
- Realisatie van de schakeling
- Programmeren
- Testen en documenteren

## planning

| Fase | Activiteit | Verwachte duur | Resultaat |
|---|---|---|---|
| 1 | Voorbereiding hardware/software | 1 dag | Componentlijst, libraries, methode |
| 2 | In elkaar zetten hardware | 2 uur | Werkende basisopstelling |
| 3 | Programmeren basisfuncties | 2 dagen | Menu + invoer + display |
| 4 | Programmeren spelmodi en highscores | 3 dagen | Volledige functionaliteit |
| 5 | Testen en verbeteren | 4 uur | Testverslag + fixes |
| 6 | Einddocumentatie | 1 uurtje | Complete examendocumentatie |

## Detailplanning

### Stap 1 - Voorbereiding
- Opdrachtanalyse en eisenlijst opstellen
- Hardwarekeuze onderbouwen
- Benodigde libraries controleren
- Ontwikkelmethode vastleggen

### Stap 2 - In elkaar zetten van de hardware
- OLED aansluiten en testen met voorbeeld uit library
- Knoppen aansluiten
- Buzzer aansluiten op gekozen pin
- Toevoer en GND controleren

### Stap 3 - Basissoftware ontwikkelen
- `setup()` en initialisatie schrijven
- Menu implementeren (highscore en play)
- Navigatie en selectie implementeren
- Geluidsfeedback toevoegen

### Stap 4 - Functionaliteiten afronden
- Normal mode implementeren
- Highscore-opslag met EEPROM implementeren
- Highscore reset

### Stap 5 - Testen, verbeteren en extras
- Functionele tests uitvoeren
- Bevindingen noteren
- Correcties doorvoeren
- Hertesten
- Countdown mode implementeren
- False start en timeout afhandeling toevoegen
- bevestiging toevoegen aan reset

### Stap 6 - Afronding en documentatie
- README bijwerken met onderbouwing per criterium
- Planning en testresultaten controleren
- Eindcontrole op leesbaarheid/volledigheid

## Debugging

| Fout | Impact | fix |
|---|---|---|
| Verkeerde bedrading | Hoog | Stap-voor-stap aansluiten en direct testen |
| Library werkt niet | Middel | Juiste versie installeren en voorbeeldsketch testen |
| Knop-bounce | Middel | Debounce via vertraging en release-check |
| EEPROM data fout | Laag | Lezen/schrijven controleren met meerdere runs |

## Afwijkingen
- Grote afwijking geweest in eerst bedachte planning (andere opdrachten die tussendoor kwamen)
- Hardware was optijd in elkaar, maar de software om het besturen was pas laat voltooid (vandaag, 16/06/2026) en volledig werkend

## Bedankjes
- Bedankt aan Tim voor het vinden voor een bug met false start en het aanraaden van een timeout na 10 seconden
- Joost voor het helpen op donderdag (want dan is Niels er nie)
- En de rest die heeft getest