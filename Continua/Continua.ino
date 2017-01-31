#include <SoftwareSerial.h>

float Sensibilidad=0.141;
int k;

void setup() {
  
  Serial.begin(9600);
  //mySerial.begin(115200);
  pinMode(7,OUTPUT);
  pinMode(A0,INPUT);
  
}

float get_corriente(int n_muestras)
{
  float voltajeSensor;
  float corriente=0;
  for(int i=0;i<n_muestras;i++)
  {
    voltajeSensor = analogRead(A0) * (5.0 / 1023.0);////lectura del sensor
    corriente=corriente+(voltajeSensor-2.5)/Sensibilidad; //EcuaciÃ³n  para obtener la corriente
  }
  corriente=corriente/n_muestras;
  Serial.print("Corriente: ");
  Serial.println(corriente,3);
  return(corriente);
}


void loop() {
  
  float I=get_corriente(200);//obtenemos la corriente promedio de 500 muestras 
  Serial.print("Corriente: ");
  Serial.println(I,3);
  
    String r;
    r = String(analogRead(A0)) + "\r";
    if (mySerial.available()) {
    Serial.println(mySerial.read());
  }
    mySerial.println(r);
    Serial.println(r);
    //mySerial.write(&r);
  
  digitalWrite(7,LOW);
  
  delay(1000);     
}



