#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <Milkcocoa.h>

#define WLAN_SSID       ""  // Wi-Fi
#define WLAN_PASS       ""  // Wi-Fi

#define MOTORPIN 5  // Wio-Node IO5 => MOTOR
#define LEDPIN 4    // Wio-Node IO4 => WS2812
#define NUMPIXELS 5   // Number of LEDs
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

WiFiClient client;

#define MILKCOCOA_APP_ID      ""  // MilkCocoa Application ID
#define MILKCOCOA_DATASTORE   ""  // MilkCocoa Datastore Name
#define MILKCOCOA_SERVERPORT  1883

const char MQTT_SERVER[] PROGMEM    = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ MILKCOCOA_APP_ID;

Milkcocoa milkcocoa = Milkcocoa(&client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);

int red, green, blue, strength, timer;

void onpush(DataElement *elem) {
  Serial.println("onpush");
  Serial.print("r:");
  Serial.print(elem->getInt("r"));
  Serial.print(",g:");
  Serial.print(elem->getInt("g"));
  Serial.print(",b:");
  Serial.print(elem->getInt("b"));
  Serial.print(",s:");
  Serial.print(elem->getInt("s"));
  Serial.print(",t:");
  Serial.print(elem->getInt("t"));
  Serial.println();

  red = elem->getInt("r");
  green = elem->getInt("g");
  blue = elem->getInt("b");
  strength = elem->getInt("s");
  timer = elem->getInt("t");

  if (strength) {
//    countdown(red, green, blue);
    countdown(255, 0, 0);
    digitalWrite(MOTORPIN, HIGH); 
    delay(5000);
    digitalWrite(MOTORPIN, LOW); 
  } else {
    led(0, 255, 0);
  }
}

void led(int v_r, int v_g, int v_b) {
  strip.setBrightness(50);
  for(int i=0;i<NUMPIXELS;i++){
    strip.setPixelColor(i, strip.Color(v_r, v_g, v_b));
    strip.show();
    delay(50);
  }
}

void countdown(int v_r, int v_g, int v_b) {
  strip.setBrightness(50);
  int v_t;
  if (timer) {
    v_t = timer;
  } else {
    v_t = NUMPIXELS;
  }
  for(int i=0;i<v_t;i++){
    strip.setPixelColor(i, strip.Color(v_r, v_g, v_b));
    strip.show();
  }
  for(int i=0;i<v_t;i++){
    strip.setPixelColor(v_t-i-1, strip.Color(0, 0, 0));
    strip.show();
    delay(1000);
  }
}

void setupWiFi() {
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  pinMode(15, OUTPUT);  // for Wio-Node
  digitalWrite(15, 1);  // for Wio-Node
  Serial.begin(115200);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  setupWiFi();
  Serial.println( milkcocoa.on(MILKCOCOA_DATASTORE, "push", onpush) );
  pinMode(MOTORPIN, OUTPUT); 
}

void loop() {
  milkcocoa.loop();
  led(255,255,255);
}

