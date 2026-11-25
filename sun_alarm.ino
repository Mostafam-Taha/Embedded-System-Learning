#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>

// ---- Settings: change these ----
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
String topic = "my-sun-alarm-48271";   // must match the topic in the ntfy app
int limit = 2500;                      // light above this = sun is up
// --------------------------------

const int LED = 26;
const int BUZZER = 27;
const int BUTTON = 25;
const int LDR = 34;

bool silenced = false;
bool alarmOn = false;
bool notified = false;
int light = 0;

WebServer server(80);

void sendNotification(String msg) {
  HTTPClient http;
  http.begin("http://ntfy.sh/" + topic);
  http.POST(msg);
  http.end();
}

String page() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<meta http-equiv='refresh' content='2'>";
  html += "<style>body{font-family:sans-serif;text-align:center;margin-top:40px;}";
  html += "h1{font-size:48px;}a{display:inline-block;margin-top:20px;padding:15px 30px;";
  html += "background:#333;color:#fff;text-decoration:none;border-radius:10px;font-size:20px;}</style>";
  html += "</head><body>";
  if (alarmOn) html += "<h1 style='color:orange'>SUN IS UP!</h1>";
  else html += "<h1 style='color:gray'>Waiting...</h1>";
  html += "<p>Light: " + String(light) + "</p>";
  html += "<p>Limit: " + String(limit) + "</p>";
  html += "<a href='/silence'>Silence</a>";
  html += "</body></html>";
  return html;
}

void handleRoot() { server.send(200, "text/html", page()); }

void handleSilence() {
  silenced = true;
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Open this on your phone: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/silence", handleSilence);
  server.begin();
}

void loop() {
  server.handleClient();

  light = analogRead(LDR);

  if (digitalRead(BUTTON) == LOW) silenced = true;

  bool bright = light > limit;
  if (!bright) {
    silenced = false;
    notified = false;
  }

  alarmOn = bright && !silenced;

  if (bright && !notified) {
    sendNotification("Sun is up! Light = " + String(light));
    notified = true;
  }

  digitalWrite(LED, alarmOn ? HIGH : LOW);
  digitalWrite(BUZZER, alarmOn ? HIGH : LOW);

  delay(50);
}
