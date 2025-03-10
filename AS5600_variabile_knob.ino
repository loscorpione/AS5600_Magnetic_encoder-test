#include <Wire.h>
#include <AS5600.h>

#define I2C_SDA 32
#define I2C_SCL 33

 
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL SerialUSB
  #define SYS_VOL   3.3
#else
  #define SERIAL Serial
  #define SYS_VOL   5
#endif
 
AMS_5600 ams5600;


volatile int Var = 0; // Variabile
volatile int stepSize = 10; // Step incremento/decremento 
volatile int lastAngle = 0; // inizializzazione angolo
volatile int Sogl = 20; // Soglia per evitare piccole variazioni 


void setup() 
{
  SERIAL.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);
  SERIAL.println(">>>>>>>>>>>>>>>>>>>>>>>>>>> ");
  if(ams5600.detectMagnet() == 1 ){
    while(1){
        if(ams5600.detectMagnet() == 1 ){
            SERIAL.print("Current Magnitude: ");
            SERIAL.println(ams5600.getMagnitude());
            break;
        }
        else{
            SERIAL.println("Can not detect magnet");
        }
        delay(1000);
    }
  }
    lastAngle = ams5600.getRawAngle();
}

void loop() {
    int currentAngle = ams5600.getRawAngle();
    int delta = currentAngle - lastAngle;
    
    if (delta > 2048) delta -= 4096;
    if (delta < -2048) delta += 4096;
    
    if (abs(delta) > Sogl) { // Soglia per evitare piccole variazioni
        Var += (delta > 0) ? stepSize : -stepSize;
        lastAngle = currentAngle;
        Serial.print("Variabile: ");
        Serial.println(Var);
    }
    delay(10);
}
