/* KerbalSimPitHelloWorld
   A very barebones Hello World sketch, that doesn't require
   any extra hardware. It periodically sends EchoRequest packets
   to the game, and uses the EchoReply packets to switch the
   on-board LED on and off. Note that the game only activates the
   echo handler during the flight scene.
   Great for confirming basic connectivity.

   Peter Hardy <peter@hardy.dropbear.id.au>
*/
#include "KerbalSimPit.h"

KerbalSimPit mySimPit(115200);

bool state = false;
unsigned long lastSent = 0;
unsigned int sendInterval = 1000;

void setup() {
  Serial.begin(115200);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  if(mySimPit.init()) {
    mySimPit.inboundHandler(packetHandler);
  } else {
    // Don't know what to do. Abort.
    digitalWrite(13, HIGH);
    while (true);
  }
}

void loop() {
  unsigned long now = millis();
  if (now - lastSent >= sendInterval) {
    if (state) {
      mySimPit.send(ECHO_REQ_PACKET, "low", 4);
    } else {
      mySimPit.send(ECHO_REQ_PACKET, "high", 5);
    }
    lastSent = now;
    state = !state;
  }
  mySimPit.update();
}

void packetHandler(byte packetType, byte *msg, byte msgSize) {
  if (packetType == ECHO_RESP_PACKET) {
    if (strcmp(msg, "low")) {
      digitalWrite(13, LOW);
    } else {
      digitalWrite(13, HIGH);
    }
  }
}
