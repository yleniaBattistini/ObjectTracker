const int LEDB_PIN = 7;
const int LEDY_PIN = 5;
const int LEDG_PIN = 3;
const int X_AXIS = A1;
const int Y_AXIS = A3;
const int PUSH_BUTTON = 1;

void setup() {
  pinMode(LEDB_PIN, OUTPUT);
  pinMode(LEDG_PIN, OUTPUT);
  pinMode(LEDY_PIN, OUTPUT);
  pinMode(X_AXIS, INPUT);
  pinMode(Y_AXIS, INPUT);
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

double normalize(int readValue) {
  return (readValue / 1023.0) * 2 - 1;
}

void loop() {
  if (Serial.available()) {
    String serialReceived = Serial.readStringUntil('\n');
    if (serialReceived.equals("ping")) {
      digitalWrite(LEDB_PIN, HIGH);
      sendAck();
    } else if (serialReceived.equals("detected")) {
      digitalWrite(LEDG_PIN, HIGH);
      sendAck();
    } else if (serialReceived.equals("not_detected")) {
      digitalWrite(LEDG_PIN, LOW);
      sendAck();
    } else if (serialReceived.equals("mode_manual")) {
      digitalWrite(LEDY_PIN, HIGH);
      sendAck();
    } else if (serialReceived.equals("mode_auto")) {
      digitalWrite(LEDY_PIN, LOW);
      sendAck();
    } else if (serialReceived.equals("disconnect")) {
      digitalWrite(LEDB_PIN, LOW);
      digitalWrite(LEDG_PIN, LOW);
      digitalWrite(LEDY_PIN, LOW);
      sendAck();
    } else if (serialReceived.equals("offset")) {
      double x = normalize(analogRead(X_AXIS));
      double y = normalize(analogRead(Y_AXIS));
      sendResponse(String(x) + " " + String(y));
    }
  }
}
