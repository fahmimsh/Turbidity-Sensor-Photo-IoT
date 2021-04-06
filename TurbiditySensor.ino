#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Logof.h"

/*  deklarasi pin I/O  */
//input
#define PTtrans1 A0
#define PTresis1 A1
#define PTdioda1 A2
#define PTtrans2 A3
#define PTresis2 A6
#define PTdioda2 A7

//Output
#define LED_hijau 4
#define LED_merah 5
#define LED_Sensor 6 //tidak digunakan
#define Valve1_motor 7
#define Valve2 8

//deklarasi variable
const byte mulai = 2;
const byte reset = 3;
int kondisi = 0;
int baca=0;
float PT_Trans1, PT_Trans2;
float PT_Resis1, PT_Resis2;
float PT_Dioda1, PT_Dioda2;
volatile byte time_counter = 0;
int Waktu_timer = 300; //>>>waktu timer pengisian air


Adafruit_SSD1306 display(-1);

void setup() 
{
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  analogReference(INTERNAL);
  
  //inisialisasi I/O
  pinMode(LED_hijau, OUTPUT);
  pinMode(LED_merah, OUTPUT);
  pinMode(Valve1_motor, OUTPUT);
  pinMode(Valve2, OUTPUT);
  
  pinMode(mulai, INPUT_PULLUP);
  pinMode(reset, INPUT_PULLUP);
  //get callibration
  attachInterrupt(digitalPinToInterrupt(mulai), Baca_sensor, CHANGE);
  attachInterrupt(digitalPinToInterrupt(reset), Reset_sensor, CHANGE);

  digitalWrite(Valve1_motor , HIGH);
  digitalWrite(Valve2 , HIGH);
  
  display.clearDisplay();
  display.drawBitmap(0, 0,  bitmap1, Width, height, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0,24);
  display.setTextSize(2);
  display.println(" TURBIDITY");
  display.println("  SENSOR");
  display.display();
  
  while(Serial.read()!= 'x'){
    
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0,24);
  display.setTextSize(2);
  display.println("   WIFI");
  display.println(" CONNECTED");
  display.display();
  delay(1000);
  
  Serial.println("WIFI sudah terkoneksi...");
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.setTextSize(2);
  display.println("CALIBRASI");
  display.println(" SENSOR");
  display.display();
  delay(3000);
  //perintah
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("TEKAN TOMBOl START u/");
  display.println(" MEMULAI BACA SENSOR");
  display.println("\nTEKAN TOMBOT STOP u/");
  display.println(" MERESET BACA SENSOR");
  display.display();
}

void loop() 
{
 if (kondisi == 1)
 {
  get_datasensor();
  lcd_serial_data();
 }

 if (kondisi == 2)
 {
  digitalWrite(Valve1_motor , HIGH);
  digitalWrite(Valve2 , HIGH);
  digitalWrite(LED_hijau, LOW);
  digitalWrite(LED_merah, LOW);
  display.clearDisplay(); 
  display.setTextColor(WHITE);
  display.setCursor(0,0); 
  display.setTextSize(2);
  display.println("RESET BACA");
  display.println("  SENSOR"); 
  display.println("TURBIDITY");
  display.display();  
 }
 
 if (baca == 1)
 {
  display.clearDisplay(); 
  display.setTextColor(WHITE);
  display.setCursor(0,0); 
  display.setTextSize(2);
  display.println("MULAI BACA");
  display.println("  SENSOR"); 
  display.println(" ISI AIR");
  display.display(); Serial.println("Mulai Baca Sensor ...");
  
  digitalWrite(Valve1_motor, LOW);
  digitalWrite(Valve2, LOW);
  
  delay(10000);

  
  digitalWrite(Valve1_motor , HIGH);
  
/*--------------------------------------------)
   MASUKKAN RUMUS UNTUK PERBANDINGAN


*---------------------------------------------)*/


  if (PT_Trans1 <= 1000)
  {
      digitalWrite(LED_hijau, HIGH);
      display.clearDisplay(); 
      display.setTextColor(WHITE);
      display.setCursor(0,0); 
      display.setTextSize(2);
      display.println("HASIL BACA");
      display.println("  SENSOR"); 
      display.println("AIR JERNIH");
      display.display();
      delay(3000);
    } 
    
  if (PT_Trans1 >= 1000)
  {
      digitalWrite(LED_merah, HIGH);
      display.clearDisplay(); 
      display.setTextColor(WHITE);
      display.setCursor(0,0); 
      display.setTextSize(2);
      display.println("HASIL BACA");
      display.println("  SENSOR"); 
      display.println("AIR KERUH");
      display.display();
      delay(3000);
  }
  baca = 0;
 }
}

float getPTtrans(unsigned int trans)
{
  unsigned int adcIn=analogRead(trans);
  float data_trans=((float)adcIn/1024)*1.1*100;//>> ubah rumus baca sensor
  
  return data_trans;
}

float getPTresis(unsigned int resis)
{
  unsigned int adcIn=analogRead(resis);
  float data_resis=((float)adcIn/1024)*1.1*100;//>> ubah rumus baca sensor
  
  return data_resis;
}

float getPTdioda(unsigned int dioda)
{
  unsigned int adcIn=analogRead(dioda);
  float data_dioda=((float)adcIn/1024)*1.1*100;//>> ubah rumus baca sensor
  
  return data_dioda;
}

void Reset_sensor()
{
  kondisi = 2;
}

void Baca_sensor()
{
  kondisi = 1;
  baca = 1;
}

void get_datasensor()
{
  PT_Trans1 = getPTtrans(PTtrans1);
  PT_Trans2 = getPTtrans(PTtrans2);
  PT_Resis1 = getPTresis(PTresis1);
  PT_Resis2 = getPTresis(PTresis2);
  PT_Dioda1 = getPTresis(PTresis1);
  PT_Dioda2 = getPTresis(PTresis2); 
}
void lcd_serial_data()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  display.print("PTtrans1: "); 
  display.println(PT_Trans1);
  display.print("PTresis1: "); 
  display.println(PT_Resis1);
  display.print("PTdioda1: "); 
  display.println(PT_Dioda1);
  display.print("PTtrans2: "); 
  display.println(PT_Trans2);
  display.print("PTresis2: "); 
  display.println(PT_Resis2);
  display.print("PTdioda1: "); 
  display.println(PT_Dioda2);
  display.display();
  //kirim data untuk diparsing
  Serial.print("*"); 
  Serial.print(PT_Trans1);
  Serial.print(","); 
  Serial.print(PT_Resis1);
  Serial.print(","); 
  Serial.print(PT_Dioda1);
  Serial.print(","); 
  Serial.print(PT_Trans2);
  Serial.print(","); 
  Serial.print(PT_Resis2);
  Serial.print(","); 
  Serial.print(PT_Dioda2);
  Serial.println("#");
}
