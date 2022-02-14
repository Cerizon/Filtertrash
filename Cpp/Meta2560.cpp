#include <Wire.h>
#include <Servo.h> 
#include <HX711.h> 
#include <LiquidCrystal_I2C.h>  // Library สำหรับจอ LCD

float z ;
Servo servodoor; 
Servo servobase; 

#define calibration_factor -380000
#define DOUT 3      // Load cell
#define CLK 2       // Load cell
#define infar1 12 
#define infar2 43 
#define LED 13 
#define prox 4 
#define prox2 42 
#define RELAY_LED1 48  // LED for ขวดแก้ว
#define RELAY_LED2 50  // LED for กระป๋อง
#define RELAY_LED3 52  // LED forพลาสติก

HX711 scale;
LiquidCrystal_I2C lcd(0x27, 20, 4);

int glassUp = 0 ; 
int PlasticUp = 0 ; 
int canUp = 0; 
void setup() {
  Serial.begin(115200); 
  Serial.println("ArduinoAll Calibrating..."); 
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor); 
  scale.tare(); 
  pinMode(LED, OUTPUT); 
  pinMode(prox, INPUT);     // ค่า proximity1 ในการตรวจเช็คกระป๋อง
  pinMode(prox2, INPUT);    // ค่า proximity2 ในการตรวจเช็คกระป๋อง
  pinMode(infar1, INPUT);   // ค่า Photo sensor ในการตรวจเช็ควัตถุ
  pinMode(infar2, INPUT);   // ค่า Photo sensor ในการตรวจเช็ควัตถุ
  lcd.begin();              // เริ่มแสดง LCD
  lcd.setCursor(4, 0);      // กำหนดตำแหน่ง LCD 
  lcd.print("BOTTLE TYPE"); // แสดงข้อความบนหน้าจอ LCD
  lcd.setCursor(0, 1); 
  lcd.print("Can = "); 
  lcd.setCursor(0, 2); 
  lcd.print("Plastic bottle = "); 
  lcd.setCursor(0, 3); 
  lcd.print("Glass bottle = "); 
  servodoor.attach(9);      //  กำหนดขาของ Servo
  delay(500); 
  servobase.attach(8); 
  delay(500); 
  servodoor.write(0); 
  delay(500); 
  servodoor.write(80);      //  ย้ายตำแหน่ง Servo ไปที่ 80 องศา
  delay(500); 
  servobase.write(0); 
  delay(1000); 
  servobase.write(98); 
  delay(1000); 
  scale.set_scale(calibration_factor); // ปรับค่า calibration factor
  scale.tare();                        // Reset ค่าน้ำหนักเป็น 0
  pinMode(RELAY_LED1,OUTPUT);          // สั่งให้ RELAY_LED1 เป็น OUTPUT
  pinMode(RELAY_LED2,OUTPUT); 
  pinMode(RELAY_LED3,OUTPUT); 
  digitalWrite(RELAY_LED1, HIGH);       // สั่งให้ RELAY_LED1 สถานะเป็น HIGH
  digitalWrite(RELAY_LED2, HIGH); 
  digitalWrite(RELAY_LED3, HIGH); 
  digitalWrite(LED, HIGH); 
}
/* ---------   กรณีวัตถุเป็นกระป๋อง  -----------    */
void can_type() {
  servobase.write(40);  //  ย้ายตำแหน่ง Servo ไปที่ 40 องศา
  delay(500); 
  servodoor.write(0);   //  ย้ายตำแหน่ง Servo ไปที่ 0 องศา
  delay(500); 
  servodoor.write(80);  //  ย้ายตำแหน่ง Servo ไปที่ 80 องศา
  delay(500); 
  canUp++; 
  lcd.setCursor(17, 1); //  กำหนดตำแหน่ง LCD
  lcd.print(canUp); 
  delay(100); 
  digitalWrite(RELAY_LED2,LOW); 
  servobase.write(98);  //  ย้ายตำแหน่ง Servo ไปที่ 90 องศา
  delay(500); 
  digitalWrite(RELAY_LED2,HIGH); 
  scale.tare();         // Reset ค่าน้ำหนักเป็น 0
}

/* ---------   กรณีวัตถุเป็นขวดแก้ว  -----------    */
void Glass_bottle_type() {
  delay(500); 
  servodoor.write(0);             //  ย้ายตำแหน่ง Servo ไปที่ 0 องศา
  delay(500);     
  servodoor.write(80);            //  ย้ายตำแหน่ง Servo ไปที่ 80 องศา
  delay(500); 
  glassUp++;           
  lcd.setCursor(17, 3);           //  กำหนดตำแหน่ง LCD
  lcd.print(glassUp); 
  delay(500); 
  digitalWrite(RELAY_LED1,LOW);   // สั่งให้ RELAY_LED1 สถานะเป็น LOW
  delay(500); 
  digitalWrite(RELAY_LED1,HIGH);  // สั่งให้ RELAY_LED1 สถานะเป็น HIGH
  scale.tare();                   // Reset ค่าน้ำหนักเป็น 0
  delay(500); 
}

/* ---------   กรณีวัตถุเป็นพลาสติก  -----------    */
void plastic_bottle_type() {
  if((z < 20)&&( z > 4 )&&(digitalRead(prox)==HIGH)){
    delay(500); 
    servobase.write(160);   //  ย้ายตำแหน่ง Servo ไปที่ 160 องศา
    delay(500); 
    servodoor.write(0);     //  ย้ายตำแหน่ง Servo ไปที่ 0 องศา
    delay(500); 
    servodoor.write(80);    //  ย้ายตำแหน่ง Servo ไปที่ 80 องศา
    delay(500); 
    PlasticUp++; 
    lcd.setCursor(17, 2);    //  กำหนดตำแหน่ง LCD
    lcd.print( PlasticUp);   //  แสดงจำนวนพลาสติกบนจอ LCD
    delay(100); 
    digitalWrite(RELAY_LED3,LOW);  
    servobase.write(98); 
    delay(500); 
    digitalWrite(RELAY_LED3,HIGH); 
    scale.tare(); 
  }
}
void loop() {
  delay(100); 
  if((digitalRead(infar2)==LOW)){
    Serial.print(scale.get_units()); // แสดงค่าน้ำหนักทาง Serial
    Serial.print("\t");
    z = abs(1000*(scale.get_units()));  // Z= ค่าน้ำหนัก x 1000
    Serial.println(z); 
    Serial.print("\t");
    delay(100);
    if( z > 46 ){     
      Serial.println("Glass_bottle"); 
      Glass_bottle_type() ; 
    }else if(((z < 20)&&(z>4))&&(digitalRead(prox2)==HIGH)){
      // ถ้าค่า 20 < z < 0.4 และ Prox สถานะ HIGH
      delay(1000); 
      Serial.println("plastic_bottle"); 
      plastic_bottle_type(); 
    }
  }
  if((digitalRead(prox2)==LOW) && ((z < 20) && ( z > 4 ))){
    Serial.println("can"); 
    can_type(); 
  }
}
