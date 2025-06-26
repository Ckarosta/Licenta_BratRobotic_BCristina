Lucrare de licență: BRAȚ ROBOTIC PENTRU SORTAREA OBIECTELOR CU MONITORIZARE LA DISTANȚĂ
---------------------------

Candidat: Cristina BALAN

Coordonator științific: Ș.l.dr.ing. Ana Maria DAN


Acest proiect permite controlul unui braț robotic cu sortare după culori.
Sistemul folosește 3 componente software principale:
1. Cod Arduino
2. Cod ESP32
3. Flow Node-RED

Adresa de GitHub
-------------------------
https://github.com/Ckarosta/Licenta_BratRobotic_BCristina

Pașii pentru rulare
--------------------

Programare Arduino Uno

	1. Se deschide Arduino IDE.
	2. Se descarcă bibliotecile <Adafruit_TCS34725.h> și <Servo.h>.
	3. Se încarcă fișierul Brat_robotic.ino.
	4. Se selectează placa Arduino Uno și portul serial folosit.
	5. Se încarcă codul pe placa Arduino Uno.

Programare ESP32

	1. Se deschide Arduino IDE.
	2. Se descarcă bibliotecile <WiFi.h> și <PubSubClient.h>.
	3. Se încarcă fișierul ESP32.ino.
	4. Se modifică datele de rețea
	      const char* ssid = "nume_wifi";
	      const char* password = "parola_wifi";
	      const char* mqtt_server = "IP_PC_cu_MQTT";
	5. Se selectează placa ESP32 Dev Module și portul folosit.
 	6. Se încarcă codul pe placa ESP32.
	7. Se verifică conectarea la Wi-Fi din Serial Monitor.

Configurare Node-RED

	1. Se pornește Node-RED din terminal cu comanda node-red.
	2. Se deschide browser-ul la adresa http://localhost1880.
	3. Se importă flow-ul Node-RED din fișierul flows.json.
	4. Se verifică conectarea nodurilor MQTT.
	5. Se deschide Dashboard-ul la adresa http://localhost1880/ui.

Ordinea pornirii
--------------------
1. Se verifică legăturile fizice și se alimentează Arduino și ESP32.
2. Se pornește brokerul MQTT.
3. Se pornește Node-RED și se deschide interfața.
4. Se trimit comenzi de control al brațului robotic din Dashboard.
   
Detalii utile
-------------------
	*Dacă folosești Mosquitto local, pornește-l cu comanda sudo systemctl start mosquitto sau instalează-l cu sudo apt install mosquitto.
 	*Dacă nu ai date în Node-RED, verifică porturile seriale pe care Arduino și ESP32 sunt conectate.
	*Poți folosi nodurile de debug din Node-RED pentru a vedea ce mesaje ajung în flow.

 
