// ESP32-C3 "TMP102" demo (simulated) for Wokwi
// - Prints a line every 5 seconds (like the ESP-IDF app)
// - Occasionally shows simulated I2C errors (timeout/NACK)

float c = 20.5;
unsigned long lastMs = 0;
unsigned intervalMs = 5000;

int16_t toRaw12(float x) { return (int16_t)round(x / 0.0625f); }

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("TMP102 demo startar...");
  Serial.println("TMP102 init OK.");
}

void loop() {
  unsigned long now = millis();
  if (now - lastMs < intervalMs) return;
  lastMs = now;

  float r = random(0, 100) / 100.0;
  if (r < 0.05) { Serial.println("Fel: I2C timeout"); return; }
  if (r < 0.10) { Serial.println("Fel: I2C NACK/busfel"); return; }

  c += (random(-5, 6)) / 100.0;   // -0.05..+0.05
  if (c < 15.0) c = 15.0;
  if (c > 30.0) c = 30.0;

  int16_t raw12 = toRaw12(c);
  float cQ = raw12 * 0.0625f;
  Serial.printf("Temp: raw=%d, %.2f C\n", raw12, cQ);
}
