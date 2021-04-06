#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
//deklarasi id
const char* ssid = "fahmi"; //isi nama wifi
const char* password = "bersiapsiapya"; //isi wifi password
unsigned long nomorChannel = 1347676;
const char* apiKey = "5Q669HZ0QHL0U300";
int keyIndex = 0;
//deklarasi variable
String dataIn, dt[10], myStatus = "";;
int data1 = 0, data2 = 0, data3 = 0, data4 = 0, data5 = 0, data6 = 0;
//inisial client
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);
  
  Serial.println();
  Serial.println("Sedang Konek ke ...");
  
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.println(".");
  }
  Serial.println("x");
  Serial.println("WiFi Connected");
  ThingSpeak.begin(client);
}

void loop() {
  //konek wifi lagi untuk verifikasi jika ada putus wifi
  reconnectwifi();
  ThingSpeak.setField(1, dt[0]);
  ThingSpeak.setField(2, dt[1]);
  ThingSpeak.setField(3, dt[2]);
  ThingSpeak.setField(4, dt[3]);
  ThingSpeak.setField(5, dt[4]);
  ThingSpeak.setField(6, dt[5]);

  if(dt[0]> dt[1])
  {
    myStatus = String("field1 is greater than field2"); 
  } 
  else if(dt[0] < dt[1])
  {
    myStatus = String("field1 is less than field2");
  } 
  else
  {
    myStatus = String("field1 equals field2");
  }
  
  ThingSpeak.setStatus(myStatus);
  int x = ThingSpeak.writeFields(nomorChannel, apiKey);
  
  if(x == 200)
  {
    Serial.println("Update Channel Berhasil");
  }
  else
  {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  parsingData();
  delay(20000);
}
void reconnectwifi(){
    if(WiFi.status() != WL_CONNECTED)
    {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    
    while(WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, password);
      Serial.print(".");
      delay(5000);     
    }
    Serial.println("x");
    Serial.println("\nConnected.");
  }
}
void parsingData(){ //*1,2,3,4,5,6#\n
  int i;
  boolean parsing=false; 
  while (Serial.available()>0) 
  {
    char inChar = (char)Serial.read();
    dataIn += inChar;
    
    if (inChar == '\n') 
    {
    parsing = true;
    }
    if(parsing)
    {
    int j=0;
    Serial.println("data masuk : " + dataIn);
    
    dt[j]="";
    for(i=1;i<dataIn.length();i++)
    {
      if ((dataIn[i] == '#') || (dataIn[i] == ','))
      {
        j++;
        dt[j]="";
      } 
      else 
      {
        dt[j] = dt[j] + dataIn[i];
      }
    }
    Serial.println("Field1: " + dt[0]);
    Serial.println("Field2: " + dt[1]);
    Serial.println("Field3: " + dt[2]);
    Serial.println("Field4: " + dt[3]);
    Serial.println("Field5: " + dt[4]);
    Serial.println("Field6: " + dt[5]);
    
    parsing=false;
    dataIn="";
    }
  }
}
