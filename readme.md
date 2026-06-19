# Praktijkexamen Microcontrollers - Reactietijd Tester

Persoonsinformatie:
- Kandidaat: Mikey
- Studentnummer: 0271757
- Klas: 25SDBA
- Datum: 19/06/2026


## Projectbeschrijving
Voor dit project heb ik een reactietijd-spel gebouwd op een microcontroller met een OLED-scherm, twee knoppen en een buzzer.
Je navigeert met 1 knop, selecteert met 1 knop en je kunt 2 modes spelen:
- Normal mode (willekeurige wachttijd)
- Countdown mode (3-2-1 aftellen)

Het systeem meet de reactietijd, slaat highscores op in EEPROM en laat alles zien op het schermpje.

### Hardware
Hardware die ik heb gebruikt:
- Arduino-uno
- OLED-display 128x64 (SH1106, I2C)
- 2 drukknoppen
- 1 buzzer
- Breadboard en jump wires
- USB-kabel voor voeding/programmeren

### Software
Programmeertaal:
- C++ (ino)

Benodigde libraries/packages:
- `Adafruit GFX Library`
- `Adafruit SH110X`

### Documentatie
Documentatie die ik heb gebruikt:
- Datasheet/voorbeeldgebruik SH1106 OLED (I2C adres en initialisatie)
- Arduino referentie voor `tone()`, `digitalRead()`, `millis()`, `INPUT_PULLUP`
- EEPROM gebruik (`EEPROM.get`, `EEPROM.put`) voor highscores
- Aansluitschema van knoppen met pull-up logica

### Passende ontwikkelmethode
Gekozen methode:
- Ik heb gewerkt in kleine stappen:
  1. Hardware aansluiten
  2. Basistest van display en knoppen
  3. Menu en schermstates bouwen
  4. Spelmodi programmeren (eerst normaal en later ook countdown als extraatje toegevoegd)
  5. Highscore-opslag toevoegen
  6. Alles testen en finetunen (met dus nog de extra countdown mode)

Motivatie:
- Dit werkte fijn, omdat ik na elke stap direct op de hardware kon checken of het goed ging en alle functies werken.

### planning
De planning staat in [planning.md](planning.md)


### Ontwikkelomgeving
Ontwikkelomgeving:
- Arduino IDE geinstalleerd en ingesteld
- Benodigde libraries geinstalleerd
- Werkende USB-verbinding met de microcontroller
- Upload getest

### Afwijkingen planning
Afwijkingenlog
- Planning is een beetje uit de hand gelopen en heb sommige dingen last moment moeten doen vanwege dat ik ook nog andere dingen moest afronden en andere dingen had om aan te werken


### Onderdelen gebruikt
De gekozen materialen passen goed bij de opdracht:
- OLED voor beeld
- Knoppen voor invoer
- Buzzer voor audio
- EEPROM voor opslag van highscores

### Aansluitschema:
- `selectPin = 2` -> knop naar GND (met `INPUT_PULLUP`)
- `navPin = 3` -> knop naar GND (met `INPUT_PULLUP`)
- `buzzerPin = 8` -> buzzer
- OLED I2C:
  - 1 -> 3.3v
  - 2 -> GND
  - 3 -> A5
  - 4 -> A4

Resultaat:
- Schakeling is volledig opgebouwd en werkt.

### Manier van assembleren
- Eerst voeding en GND gecontroleerd
- Daarna de display aangesloten
- Daarna de knoppen en buzzer aangesloten
- Na elke stap direct getest

### Middelen om te programmeren
- Programmeren via Arduino IDE en USB-interface
- Upload direct naar microcontroller

### Functionaliteit
Functionaliteiten die ik heb gemaakt:
- Hoofdmenu en submenus
- Twee spelmodus (Normal en Countdown)
- Detectie van false start
- Timeout "too slow" na 10s
- Meting van reactietijd
- Top 3 highscores per modus (gemarkeert als N en C)
- Highscore reset met bevestiging
- Geluidsfeedback bij acties/resultaten

### Opbouw van code
Ik heb de code logisch opgesplitst in functies:
- Setup/loop structuur
- Menu handling (`draw...`, `handle...`)
- Speellogica (`playNormalMode`, `playCountdownMode`)
- Opslaglogica (`load...`, `save...`, `insertHighscore...`)

### Vorm van code
- logische namen van variabelen en functies
- Leesbare structuur met duidelijke functies per taak
- Hergebruik van hulpfuncties (`beep`, `waitForRelease`)

### Systematisch werken
- Ik heb in logische deelstappen gewerkt met tussentijdse checks
- Elke nieuwe functie heb ik meteen getest

### Voorbereiden testactiviteiten
Testgevallen die ik vooraf heb opgesteld:

| Test-nr | Onderdeel | Doel | Verwacht resultaat |
|---|---|---|---|
| T-01 | Opstarten | Intro en menu tonen | Introgeluid + hoofdmenu zichtbaar |
| T-02 | Menu navigatie | Navigeren met nav-knop | Selectie schuift correct |
| T-03 | Selecteren | Selectknop activeert optie | Juiste scherm/functie opent |
| T-04 | Normal mode | Reactiemeting | Tijd wordt gemeten en getoond |
| T-05 | False start normal | Te vroeg drukken | FALSE START melding |
| T-06 | Countdown mode | 3-2-1 + reactiemeting | PRESS verschijnt en tijdmeting start |
| T-07 | False start countdown | Te vroeg tijdens aftellen | FALSE START melding |
| T-08 | Timeout | Niet drukken binnen 10s | TOO SLOW melding |
| T-09 | Highscore opslag | Nieuwe betere score | Score op juiste plek in top 3 |
| T-10 | Highscore reset | Reset bevestigen | Beide scorelijsten leeg (N/A) |

### Testactiviteiten uitvoeren
Resultaten van uitgevoerde tests:

| Test-NR | Resultaat | Opmerking |
|---|---|---|
| T-01 | Geslaagd | Intro en menu werken |
| T-02 | Geslaagd | Navigatie stabiel |
| T-03 | Geslaagd | Selectie werkt correct |
| T-04 | Geslaagd | Tijdmeting in ms |
| T-05 | Geslaagd | False start correct afgehandeld |
| T-06 | Geslaagd | Countdown en startmoment correct |
| T-07 | Geslaagd | False start tijdens countdown werkt |
| T-08 | Geslaagd | Timeout na 10 seconden |
| T-09 | Geslaagd | EEPROM opslag functioneert |
| T-10 | Geslaagd | Reset inclusief bevestiging werkt (met countdown) |

### Conclusies en verbetervoorstellen
Conclusie:
- Alle belangrijke functionaliteiten werken volgens de opdracht.

Verbetervoorstellen:
- Extra moeilijkheidsniveaus met kortere reactievensters
- Statistieken (gemiddelde reactietijd over meerdere pogingen)
- Beter de planning volgen

### Aanpassingen doorvoeren
Doorgevoerde aanpassingen op basis van testen:
- false start triggert zodra er word gedrukt niet zodra er word losgelaten
- Timeout na 10 seconden


## Gebruiksinstructie voor demo
1. Sluit de arduino aan op stroom
2. Ga naar `Play`.
3. Kies `Normal` of `Countdown`.
4. Druk pas bij `PRESS!`.
5. Bekijk score en highscores.
6. Controleer resetfunctie in `Highscores`.


## Problemen die ik heb ervaren
- Knoppen deden niks
- alles stops als de buzzer niet verbonden is
- kapotte kabels

## Ter info
- De video (https://youtube.com/shorts/3XtIPdvqKEQ?feature=share voor onbekende reden als short geupload maar t moest ff zo want de video was te groot voor git) had een heleboel achtergrond geluiden, ik heb het bewerkt om te zorgen dat meeste weg zijn, maar heel af en toe hoor je daarom de buzzer ook niet.
