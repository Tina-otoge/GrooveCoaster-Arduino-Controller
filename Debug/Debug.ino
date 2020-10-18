const int MAX_PIN = 16;

void setup()
{
	Serial.begin(9600);
	for (int i = 0; i <= MAX_PIN; i += 1)
		pinMode(i, INPUT);
}

void loop()
{
	for (int i = 0; i <= MAX_PIN; i += 1) {
		if (!digitalRead(i))
			continue;
		Serial.write("Pin ");
		Serial.write(String(i).c_str());
		Serial.write(" pressed\r\n");
	}
	Serial.flush();
	delay(1000);
}
