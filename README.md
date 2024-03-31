# FlowerWatch - Keep your plant alive!

Előfordult már veled, hogy megfeledkeztél a növényedről? Itt a megoldás!
Ebben a projektben egy figyelőrendszert készítek, mellyel webes felületen követheted növénykéd napi tevékenységét. Beállításaid alapján a rendszer jelezni fog emailben, hogy meg kell locsolni a növényt, így nem fogod többé elfelejteni az öntözést.

A projekt az ipar 4.0 tantárgyam beadandó feladataként készül, de szeretném, hogy ezen túlmutasson a későbbiekben és egy hobbisták számára könnyen összerakható és könnyen használható rendszer legyen a végeredmény.

Változtatások
-------
- 2024.03.04. - src/main.cpp: DHT11 értéke hiba esetén nem kerül GUI-ra
- 2024.03.04. - src/main.cpp: TEMT6000 és SOILCAPv1.2 értéke beolvasásra, értelmezésre és kiírásra kerül
- 2024.03.04. - src/main.cpp: a szenzorok értéke két másodpercenként kerül csak frissítésre
- 2024.03.04. - src/main.cpp: frissítés csak csatlakozott kliensek jelenlétekor kerül küldésre
- 2024.03.04. - src/main.cpp: stringek ahol lehet referenciával adódnak át
- 2024.03.04. - src/main.cpp: email_data struktúra példányai referenciával adódnak át
- 2024.03.04. - lib/Photo and email module/camera_email.cpp: email_data struktúra referenciaként adódik át
- 2024.03.04. - lib/Photo and email module/camera_email.h: email_data struktúra referencia az argumentumokban
- 2024.03.18. - src/main.cpp: Weblap handler eljárások ki lettek szervezve a setupból és régiókba lettek rendezve
- 2024.03.18. - src/main.cpp: JSON string builderek régióba rendezve
- 2024.03.18. - lib/Photo and email module/camera.cpp: capturePhotoSaveLittleFS és captureAndSendPhoto argumentuma bool értéket (is) kér későbbi vaku funkcionalitáshoz
- 2024.03.18. - lib/Photo and email module/camera.h: capturePhotoSaveLittleFS és captureAndSendPhoto argumentuma bool értéket (is) kér későbbi vaku funkcionalitáshoz
- 2024.03.18. - lib/Photo and email module/camera.cpp: a "flash" bool változó True értéke esetén fényképezéskor a 4-es számú pin (vaku) 1 értéket vesz fel, majd visszaáll 0-ra
- 2024.03.31. - lib/Photo and email module/camera.h: email_data struktúrában a recipient és a htmlMsg adattagok String típusúak lettek
- 2024.03.31. - src/main.cpp: nincs delay a loop()-ban, helyette változók segítségével időréselés történik
- 2024.03.31. - lib/Webpage/index.h - Vaku gomb hozzáadva az oldalhoz és a forráslinkek frissítve
- 2024.03.31. - src/main.cpp: a loop()-on belül meghívódik a captureAndSendPhoto(email, flash) metódus, hogyha a talaj túl száraz vagy a hőmérséklet túl magas
