#include <Arduino.h>

#define NUM_SENSORS 4
const int HALL_SENSOR_PINS[NUM_SENSORS] = {32, 33, 34, 35};  // Define pins for 4 hall sensors
#define LED_PIN 26  // Built-in LED pin (adjust if necessary)

void setup() {
    Serial.begin(115200);
    delay(1000);  // Increase delay to ensure Serial Monitor initializes
    Serial.println("Serial Monitor Initialized");  // Debug message

    // Initialize hall sensor pins
    for (int i = 0; i < NUM_SENSORS; i++) {
        pinMode(HALL_SENSOR_PINS[i], INPUT);
        Serial.print("Hall Sensor Pin ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(HALL_SENSOR_PINS[i]);
    }

    pinMode(LED_PIN, OUTPUT);  // Set LED pin as output

    // Blink LED to confirm the board is running
    for (int i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(200);
        digitalWrite(LED_PIN, LOW);
        delay(200);
    }
}

// Function to calculate air gap based on sensor voltage
float calculateAirGap(float voltage) {
    // Placeholder calibration formula: air gap (in mm) = k / voltage
    // Replace 'k' with a calibrated constant based on your sensor and setup
    const float k = 10.0;  // Example constant, adjust based on calibration
    if (voltage > 0) {
        return k / voltage;  // Calculate air gap
    } else {
        return -1;  // Return -1 for invalid voltage
    }
}

// Function to calculate magnetic field strength in Tesla
float calculateMagneticField(float voltage) {
    // Placeholder calibration formula: magnetic field (in Tesla) = voltage * sensitivity
    // Replace 'sensitivity' with the sensor's sensitivity in Tesla/Volt from the datasheet
    const float sensitivity = 0.01;  // Example sensitivity (10 mT/V), adjust based on calibration
    return voltage * sensitivity;
}

void loop() {
    digitalWrite(LED_PIN, HIGH);  // Turn LED on
    delay(500);
    digitalWrite(LED_PIN, LOW);   // Turn LED off
    delay(500);

    // Read and process data from all hall sensors
    for (int i = 0; i < NUM_SENSORS; i++) {
        int sensorValue = analogRead(HALL_SENSOR_PINS[i]);  // Read sensor value (0-4095)
        Serial.print("Raw Sensor Value (Sensor ");
        Serial.print(i);
        Serial.print("): ");
        Serial.println(sensorValue);  // Debug: Print raw value

        // Check if raw value is zero
        if (sensorValue == 0) {
            Serial.print("Warning: Sensor ");
            Serial.print(i);
            Serial.println(" is returning 0. Check wiring or sensor.");
        }

        // Convert to voltage
        float voltage = (sensorValue / 4095.0) * 3.3;  // Convert to voltage
        Serial.print("Hall Sensor Voltage (Sensor ");
        Serial.print(i);
        Serial.print("): ");
        Serial.print(voltage);
        Serial.println(" V");

        // Calculate air gap
        float airGap = calculateAirGap(voltage);
        if (airGap > 0) {
            Serial.print("Air Gap (Sensor ");
            Serial.print(i);
            Serial.print("): ");
            Serial.print(airGap);
            Serial.println(" mm");
        } else {
            Serial.print("Air Gap (Sensor ");
            Serial.print(i);
            Serial.println("): Invalid voltage");
        }

        // Calculate magnetic field strength
        float magneticField = calculateMagneticField(voltage);
        Serial.print("Magnetic Field Strength (Sensor ");
        Serial.print(i);
        Serial.print("): ");
        Serial.print(magneticField);
        Serial.println(" Tesla");

        // Check if sensor behaves like a digital sensor
        int threshold = 2000;  // Default threshold
        if (sensorValue > threshold) {
            Serial.print("Digital Sensor (Sensor ");
            Serial.print(i);
            Serial.println("): Magnet detected (HIGH)");
        } else {
            Serial.print("Digital Sensor (Sensor ");
            Serial.print(i);
            Serial.println("): No magnet detected (LOW)");
        }
    }

    delay(1000);  // Increase delay to avoid flooding the Serial Monitor
}
