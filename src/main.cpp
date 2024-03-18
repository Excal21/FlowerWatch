#include <Arduino.h>
#include <WiFiManager.h>
#include "soc/rtc_wdt.h"

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// MCP3008
#include <Wire.h>
#include <Adafruit_MCP3008.h>

// DHT11
#include "DHTesp.h" //https://github.com/beegee-tokyo/DHTesp

#include <Arduino_JSON.h>
// Mentés flashre
#include <Preferences.h>

// Kamerakép emailben
#include "camera_email.h"

#include "index.h"

#define DHTPIN 14
#define DHTTYPE DHT11

const char* html_soil = "soilin";
const char* html_temp = "tempin";
const char* html_led = "ledin";
const char* html_email = "emailin";


email_data email;
// Szenzorok
DHTesp dht;
Adafruit_MCP3008 adc;

WiFiManager wm;
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

Preferences p;

// https://randomnerdtutorials.com/esp32-cam-send-photos-email/
// https://github.com/mobizt/ESP-Mail-Client

//Beállítások
int minsoil = 30;
int maxtemp = 40;
bool led = true;
String emailaddress;

//Segédváltozók (runtime)
float temp;
float humidity;
float lux;
float soil;
bool mts = false;

JSONVar jsonValues;
JSONVar jsonPrefs;

unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

unsigned long lastTimeReadDHT11 = 0;


void setEmailParams(){
  email.name = "";
  email.recipient = "";

  email.sender = "";
  email.password = "";
  email.subject = "";
  email.htmlMsg = "";
}

void readSettings(email_data& email){
  p.begin("Settings", true);
  maxtemp = p.getInt("maxtemp");
  minsoil = p.getInt("minsoil");
  led = p.getBool("led");
  email.recipient = p.getString("emailaddress").c_str();
  p.end();
}



#pragma region StringBuilders

String getPreferences(){
  jsonPrefs["prefemail"] = emailaddress;
  Serial.print("Led: "); Serial.println(led);
  jsonPrefs["prefled"] = led ? 1 : 0;
  jsonPrefs["prefsoilmin"] = minsoil;
  jsonPrefs["preftempmax"] = maxtemp;

  String jsonstring = JSON.stringify(jsonPrefs);
  return jsonstring;
}


String getSensorReadings()
{
  jsonValues["temp"] = temp;
  jsonValues["humidity"] = humidity;
  jsonValues["soil"] = soil;
  jsonValues["light"] = int(lux);

  String jsonString = JSON.stringify(jsonValues);
  return jsonString;
}

#pragma endregion StringBuilders

#pragma region WEBHandlers

void notifyClients(const String &sensorReadings)
{
  ws.textAll(sensorReadings);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;

    // Check if the message is "getReadings"
    if(strcmp((char*)data, "getPreferences")){
      const String& prefs = getPreferences();
      Serial.println(prefs);
      notifyClients(prefs);
      Serial.println("Beállítások elküldve");
    }

    if (strcmp((char *)data, "getReadings") == 0)
    {
      // if it is, send current sensor readings
      const String& sensorReadings = getSensorReadings();
      Serial.println(sensorReadings);
      notifyClients(sensorReadings);
    }
    else if(strcmp((char *)data, "sendPhoto") == 0){
      mts = true;
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    handleWebSocketMessage(arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void onSettings(AsyncWebServerRequest* request)
{
  p.begin("Settings");
  int params = request->params();
  for (int i = 0; i < params; i++)
  {
    AsyncWebParameter *param = request->getParam(i);

    // HTTP POST input1 value (direction)
    if (param->name() == html_soil)
    {
      if (p.getInt("minsoil") != param->value().toInt())
      {
        p.putInt("minsoil", param->value().toInt());
        Serial.println("Talajnedvesség mentve flashre!");
      }
      minsoil = param->value().toInt();
      Serial.print("Beállított nedvesség: ");
      Serial.println(minsoil);
    }
    // HTTP POST input2 value (steps)
    if (param->name() == html_led)
    {
      if (p.getBool("led") != (bool)(param->value().toInt()))
      {
        p.putBool("led", (bool)(param->value().toInt()));
        Serial.println("LED állapota mentve flashre!");
      }
      Serial.print("LED beállítása: ");
      led = param->value().toInt();
      Serial.println(led);
    }
    if (param->name() == html_temp)
    {
      if (p.getInt("maxtemp") != param->value().toInt())
      {
        p.putInt("maxtemp", param->value().toInt());
        Serial.println("Maximum hőmérséklet mentve flashre!");
      }
      Serial.print("Hőmérséklet beállítása: ");
      maxtemp = param->value().toInt();
      Serial.println(param->value());
    }
    if (param->name() == html_email)
    {
      if (p.getString("emailaddress") != param->value() && param->value() != "")
      {
        p.putString("emailaddress", param->value().c_str());
        Serial.println("Email cím mentve flashre!");
      }
      Serial.print("Beállított emailcím: ");
      if (param->value() != "")
        emailaddress = param->value();
      Serial.println(param->value());
    }
  }
  p.end();
  request->send_P(200, "text/html", home);
}


void onGet(AsyncWebServerRequest* request){
  Serial.println("ESP32 Web Server: Új kérés:"); // for debugging
  Serial.println("GET /");                       // for debugging
  request->send_P(200, "text/html", home);
}


void initWebSocket()
{

  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

#pragma endregion WEBHandlers


void setup()
{
  Serial.begin(9600);
  pinMode(33, OUTPUT); //Beépített LED
  pinMode(4, OUTPUT); //Vaku
  digitalWrite(33, 0);
  readSettings(email);
  dht.setup(2, DHTesp::DHT11);
  //     (sck, mosi, miso, cs);
  adc.begin(14, 13, 15, 12);
  delay(1000);



  //Kezdeti mérések induláskor
  temp = dht.getTemperature();
  humidity = dht.getHumidity();
  float read_voltage = adc.readADC(1) / 1024. * 3.28;
  lux = read_voltage * 100. / 0.6;
  soil = 100. - adc.readADC(0) / 530.;



  Serial.println("Szenzorok inicializálva!");
  Serial.print("Kezdeti hőmérséklet: "); Serial.println(temp);
  Serial.print("Kezdeti páratartalom: "); Serial.println(humidity);


  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Brownout érzékelő kikapcsolása

  bool res;
  res = wm.autoConnect("ESP32 Virágfigyelő");
  if (!res)
  {
    Serial.println("Kapcsolat sikertelen");
    wm.resetSettings();
    ESP.restart();
  }
  else
  {
    Serial.println("Csatlakozva WiFi hálózatra");
    // Fájlrendszer inicializálása
    delay(1000);
    ESP_MAIL_DEFAULT_FLASH_FS.begin();
    initCamera();
    setEmailParams();
    initWebSocket();

    delay(200);
    server.on("/", HTTP_GET, onGet);
    server.on("/get", HTTP_GET, onSettings);

    server.begin();

    delay(2000);

  }
  digitalWrite(33, 1);
}



void loop()
{
  if (ws.count() > 0 && (millis() - lastTime) > timerDelay)
  {
    //30 másodpercenként fut
    const String& sensorReadings = getSensorReadings();
    Serial.println(sensorReadings);
    notifyClients(sensorReadings);

    lastTime = millis();
    
    ws.cleanupClients();
  }

  if ((millis() - lastTimeReadDHT11) > 2000)
  {
    //2 másodpercenként fut
    float current_temp = dht.getTemperature();
    float current_humidity = dht.getHumidity();
    if (!(isnan(current_temp) || isnan(humidity)))
    {
      temp = current_temp;
      humidity = current_humidity;
    }
    float read_voltage = adc.readADC(1) / 1024. * 3.28;
    lux = read_voltage * 100. / 0.6;

    soil = 100. - ((adc.readADC(0) - 130) / 5.30);

    lastTimeReadDHT11 = millis();
  }

  if (mts)
  {
    captureAndSendPhoto(email, 1);
    mts = false;
  }
  if(led){
    digitalWrite(33,0);
    delay(500);
    digitalWrite(33, 1);
    delay(400);
  }
  delay(50);
}
