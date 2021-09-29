const int LEDB_PIN = 7;
const int LEDG_PIN = 5;
const int LEDR_PIN = 3;
const int xAxis = A1;
const int yAxis = A3;
const int pushButton = 1;
String serialReceived;
bool manual_mode;
int xReading;
int yReading;

void setup() {
  manual_mode = false;
  xReading = 512;
  yReading = 512;
  pinMode(LEDB_PIN, OUTPUT);
  pinMode(LEDG_PIN, OUTPUT);
  pinMode(LEDR_PIN, OUTPUT);
  digitalWrite(LEDB_PIN, LOW);
  digitalWrite(LEDG_PIN, LOW);
  digitalWrite(LEDR_PIN, LOW);
  Serial.begin(9600);
  Serial.print("Welcome to Object Trackers.");
  //Il joystick parte da 512,512
}

void loop() {
  if (Serial.available()) {
    serialReceived = Serial.readStringUntil('\n');
    if (serialReceived.equals("ping")) {
      Serial.print("Ack\n");
      digitalWrite(LEDB_PIN, HIGH);
    } else if (serialReceived.equals("detected")) {
      Serial.print("Ack\n");
      digitalWrite(LEDR_PIN, LOW);
      digitalWrite(LEDG_PIN, HIGH);
    } else if (serialReceived.equals("not_detected")) {
      Serial.print("Ack\n");
      digitalWrite(LEDG_PIN, LOW);
      digitalWrite(LEDR_PIN, HIGH);
    } else if (serialReceived.equals("mode_manual")) {
      Serial.print("Ack\n");
      manual_mode = true;
      digitalWrite(LEDB_PIN, LOW);
      digitalWrite(LEDB_PIN, HIGH);
    } else if (serialReceived.equals("mdoe_auto")) {
      Serial.print("Ack\n");
      manual_mode = false;
    } else if (serialReceived.equals("disconnect")) {
      Serial.print("Ack\n");
      digitalWrite(LEDB_PIN, LOW);
      digitalWrite(LEDG_PIN, LOW);
      digitalWrite(LEDR_PIN, LOW);
    } else if (serialReceived.equals("xAxis")) {
      if (manual_mode) {
        xReading = analogRead(xAxis);
        Serial.print(xReading); //auto
      }
    } else if (serialReceived.equals("yAxis")) {
      if (manual_mode) {
        yReading = analogRead(yAxis);
        Serial.print(yReading); //auto
      }
    }
  }
}
