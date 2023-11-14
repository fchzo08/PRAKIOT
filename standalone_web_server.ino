#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#define analogPin A0
int adcValue = 0;

int pinRelay = 13; //D7 
int pinPir=15; //D8
int pirValue;
const char* ssid = "anj";
const char* password = "123456789";
bool connectedInternet = true;
WiFiServer server(80);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  // lcd.begin();
  // pinMode(LED_BUILTIN, OUTPUT);
  // pinMode(pinRelay, OUTPUT);
  // digitalWrite(pinRelay, HIGH);

  // Serial.println();
  // Serial.print("Connecting to ");
  // Serial.println(ssid);

  // WiFi.begin(ssid, password);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Waiting Internet");
  int i=0;
 /* while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if(i==16){
      for(int j=15;j>=0;j--){
        lcd.setCursor(j,1);
        delay(250);
        lcd.print(" ");
      }
      i=0;  
    }
    lcd.setCursor(i,1);
    lcd.print(".");i++;

  }
*/
  // Serial.println("WiFi connected");
  // for(int j=15;j>=0;j--){
  //       lcd.setCursor(j,1);
  //       delay(100);
  //       lcd.print(" ");
  //   }
  // lcd.print("Wifi Connected");
  // delay(500);
  // lcd.clear();
  // lcd.setCursor(0, 0);
  // lcd.print("Current IP :");
  // lcd.setCursor(0, 1);
  // lcd.print(WiFi.localIP());
  // server.begin();
  // Serial.println("Web server running. Waiting for the ESP IP...");

  // delay(1000);
  // Serial.println(WiFi.localIP());
}

void loop() {
  adcValue = analogRead(analogPin);
  Serial.print("ADC Value = ");
  Serial.println(adcValue);
  delay(1000);
  WiFiClient client = server.available();
  if(WiFi.status()!=WL_CONNECTED){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Disconnected");
    delay(5000);
    lcd.clear();
    lcd.print("Waiting Internet");
    delay(500);
    int i=0;
    while(WiFi.status()!=WL_CONNECTED){
      delay(500);
    if(i==16){
      for(int j=15;j>=0;j--){
        lcd.setCursor(j,1);
        delay(250);
        lcd.print(" ");
      }
      i=0;  
    }
    lcd.setCursor(i,1);
    lcd.print(".");i++;
    }
    
  }
  pirValue=digitalRead(pinPir);
  if(pirValue==1){
    digitalWrite(pinRelay, LOW);//lampu nyala
    return;
  }
  if(pirValue==0){
    digitalWrite(pinRelay,HIGH);//high lampu mati
  }
  if (client) {
    Serial.println("New client");
    boolean blank_line = true;

    // Membaca request dari client
    String request = client.readStringUntil('\n');

    // Jika request mengandung "/lampunyala", panggil fungsi lampunyala
    if (request.indexOf("/lampunyala") != -1) {
      lampunyala();
    }
    if (request.indexOf("/lampumati") != -1) {
      lampumati();
    }
    // Membuat string untuk menyimpan pesan HTML
    String htmlMessage =
  "<!DOCTYPE html>"
  "<html>"
  "<head>"
  "<title>Lampu Kamar</title>"
  "<script>"
  "function nyala() {"
  "  alert('Menyalakan lampu...');"
  "  var xhr = new XMLHttpRequest();"
  "  xhr.open('GET', '/lampunyala', true);"
  "  xhr.send();"
  "}"
  "function mati() {"
  "  alert('Mematikan lampu...');"
  "  var xhr = new XMLHttpRequest();"
  "  xhr.open('GET', '/lampumati', true);"
  "  xhr.send();"
  "}"
  "</script>"
  "</head>"
  "<body>"
  "<button type=\"button\" onclick=\"nyala()\">Lampu Nyala</button>"
  "<button type=\"button\" onclick=\"mati()\">Lampu Mati</button>"
  "</body>"
  "</html>";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (c == '\n' && blank_line) {
          // Kirim respons HTTP
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();

          // Mengirim pesan HTML menggunakan client.print
          client.print(htmlMessage);

          break;
        }

        if (c == '\n') {
          blank_line = true;
        } else if (c != '\r') {
          blank_line = false;
        }
      }
    }

    // Menunggu sebentar sebelum menutup koneksi
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }
}

void lampumati() {
  Serial.println("Lampu Mati");
  //digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(pinRelay, HIGH);
}
void lampunyala() {
  Serial.println("Lampu Nyala");
  //digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(pinRelay, LOW);
}
