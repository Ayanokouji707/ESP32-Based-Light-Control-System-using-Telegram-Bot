#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>  
#include <ArduinoJson.h>

const char* ssid = "JARVIS 9933";
const char* password = "10101010";

#define BOTtoken "7295651280:AAEUKZmdkNaTlonyKGx8potN1fXmupKSGUQ"  
#define CHAT_ID "832408383"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPins[] = {2, 15, 4, 5};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

void turnOffAllLeds() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void applyPatternWithDelay(String pattern, int delayTime) {
  turnOffAllLeds();
  for (int j = 0; j < numLeds; j++) {
    if (pattern[j] == '1') {
      digitalWrite(ledPins[j], HIGH);
    } else {
      digitalWrite(ledPins[j], LOW);
    }
  }
  delay(delayTime);
  turnOffAllLeds();  // Turn off LEDs after delay
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/led_on to turn GPIO ON \n";
      welcome += "/led_off to turn GPIO OFF \n";
      welcome += "/state to request current GPIO state \n";
      welcome += "/pattern <binary pattern> <delay in ms> to apply LED pattern\n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text.startsWith("/pattern")) {
      int spaceIndex = text.indexOf(' ');
      int secondSpaceIndex = text.indexOf(' ', spaceIndex + 1);
      
      if (spaceIndex != -1 && secondSpaceIndex != -1) {
        String pattern = text.substring(spaceIndex + 1, secondSpaceIndex);
        int delayTime = text.substring(secondSpaceIndex + 1).toInt();

        if (pattern.length() == numLeds && delayTime > 0) {
          bot.sendMessage(chat_id, "Applying pattern with delay...", "");
          applyPatternWithDelay(pattern, delayTime);
        } else {
          bot.sendMessage(chat_id, "Invalid pattern or delay time. Use /pattern <binary pattern> <delay in ms>.", "");
        }
      } else {
        bot.sendMessage(chat_id, "Invalid command format. Use /pattern <binary pattern> <delay in ms>.", "");
      }
    }

    if (text == "/state") {
      String stateMessage = "Current LED states: ";
      for (int i = 0; i < numLeds; i++) {
        stateMessage += String(digitalRead(ledPins[i]));
        if (i < numLeds - 1) stateMessage += ", ";
      }
      bot.sendMessage(chat_id, stateMessage, "");
    }
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");
    client.setTrustAnchors(&cert);
  #endif

  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
