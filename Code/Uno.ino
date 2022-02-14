#include <Servo.h>      // Library สำหรับ Servo
#define sensor 4        // ขา Sensor
#define manualsw 5      // ขา Switch

Servo servodoor;        // Define servo

void setup()
{
  pinMode(manualsw,INPUT_PULLUP);   // Set ขาสัญญาณ switch
  pinMode(sensor,INPUT);            // Set ขาสัญญาณ sensor
  servodoor.attach(9);              // กำหนดตำแหน่งขาของ Servo
  delay(500);                       // delay 0.5 ms
  servodoor.write(0);               // ย้ายตำแหน่ง Servo ไปที่ 0 องศา
  delay(500); 
}
void loop() {
  if( (digitalRead (manualsw)==LOW) || (digitalRead (sensor)==LOW) ){ // ถ้ามีการกด switch และค่าของ sensor สามารถตรวจจับได้
    servodoor.write(180);   // ย้ายตำแหน่ง Servo ไปที่ 180 องศา
    delay(500);
  }else{ 
    servodoor.write(0);     // ย้ายตำแหน่ง Servo ไปที่ 0 องศา
  }
}
