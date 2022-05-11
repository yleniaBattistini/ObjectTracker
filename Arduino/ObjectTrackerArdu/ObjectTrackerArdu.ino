const int LEDB_PIN = 7;
const int LEDY_PIN = 5;
const int LEDG_PIN = 3;

void setup() {
  pinMode(LEDB_PIN, OUTPUT);
  pinMode(LEDG_PIN, OUTPUT);
  pinMode(LEDY_PIN, OUTPUT);
  digitalWrite(LEDB_PIN, LOW);
  digitalWrite(LEDG_PIN, LOW);
  digitalWrite(LEDY_PIN, LOW);
  Serial.begin(9600);
  // Serial.print("Welcome to Object Trackers.");
}

void sendResponse(String response) {
  Serial.print(response);
  Serial.print("\n");
}

void sendAck() {
  sendResponse("Ack");
}

void loop() {
  if (Serial.available()) {
    String serialReceived = Serial.readStringUntil('\n');
    if (serialReceived.equals("ping")) {
      digitalWrite(LEDB_PIN, HIGH);
      sendAck();
    } else if (serialReceived.equals("detected")) {
      digitalWrite(LEDG_PIN, HIGH);
    } else if (serialReceived.equals("not_detected")) {
      digitalWrite(LEDG_PIN, LOW);
    } else if (serialReceived.equals("calibration_on")) {
      digitalWrite(LEDY_PIN, HIGH);
      delay(1000);
      digitalWrite(LEDY_PIN, LOW);
      delay(1000); 
    } else if (serialReceived.equals("calibration_off")) {
      digitalWrite(LEDY_PIN, LOW);
    } else if (serialReceived.equals("disconnect")) {
      digitalWrite(LEDB_PIN, LOW);
      digitalWrite(LEDG_PIN, LOW);
      digitalWrite(LEDY_PIN, LOW);
      sendAck();
    }
  }
}
