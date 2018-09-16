#include<LiquidCrystal.h> 
#include <SoftwareSerial.h> 

SoftwareSerial gps(3,2);// rx tx  // gps gsm
LiquidCrystal lcd(6,7,8,9,10,11); //RS,EN,D4,D5,D6,D7    // initialize the library with the numbers of the interface pins
#define but 5
#define buzzer 4

#define ap1  A0
#define ap2  A1
#define ap3  A2

int sv1 = 0;   //x axis      
int sv2 = 0;   //y axis
int sv3 = 0;   //z axis


int butt = 0,duration=0; //on off button
      
char str[70];
String gpsString="";
char *test="$GPGGA";
String latitude="No Range      ";// location
String longitude="No Range     ";
int temp=0,i;  
boolean Serial_status=0;

void setup()
{
 lcd.begin(16,2);
 
 lcd.setCursor(0,0);
 lcd.print("HELLO");
 delay(1000);  // MILLISECOND
 lcd.clear();
 Serial.begin(9600); // BPS
 gps.begin(9600);    // BAUDRATE BPS
 pinMode(but,INPUT_PULLUP);
 pinMode(buzzer,OUTPUT);
} 
void loop()
{
 accel();  
 
 if((sv1<=380&&sv1>=320)&&(sv2<=380&&sv2>=320)&&(sv3<=450&&sv3>=410)) //  left side
 {
  lcd.clear();
  lcd.setCursor(0,0);
   digitalWrite(buzzer,HIGH);
 lcd.print("ACCIDENT OCCUR");
  get_gps();   //////////////////////////////////////////////////////
  msg1();
  tracking();
  msg2();
  tracking();
  trigger();
   lcd.clear();
 } 
 else if((sv1<=380&&sv1>=320)&&(sv2<=380&&sv2>=320)&&(sv3<=330&&sv3>=310))// right
 {
 lcd.clear();
 digitalWrite(buzzer,HIGH);
 lcd.setCursor(0,0);
 lcd.print("ACCIDENT OCCUR");
 get_gps();////////////////////////////////////////////////////////
  msg3();
  tracking();
  msg4();
  tracking();
  trigger();
   lcd.clear();
 }
 else
 {
  digitalWrite(buzzer,LOW);
  lcd.setCursor(0,0);
  lcd.print("life is precious");
  lcd.setCursor(0,1);
  lcd.print("Go slow.......  ");     
 }
 
 
}

void gpsEvent()
{
  gpsString="";
  while(1)
  {
   while (gps.available()>0)            //checking serial data from GPS
   {
    char inChar = (char)gps.read();
     gpsString+= inChar;                    //store data from GPS into gpsString
     i++;
     if (i < 7)                      
     {
      if(gpsString[i-1] != test[i-1])         //checking for $GPGGA sentence
      {
        i=0;
        gpsString="";
      }
     }
    if(inChar=='\r')
    {
     if(i>65)
     {
       Serial_status=1;
       break;
     }
     else
     {
      i=0;
     }
    }
  }
  if(Serial_status)
  break;
  }
}

void get_gps()
{
 Serial_status=0;
 int x=0;
 while(Serial_status==0)
 {
  gpsEvent();
  int str_lenth=i;
  latitude="";
  longitude="";
  int comma=0;
  while(x<str_lenth)
  {
   if(gpsString[x]==',')
   comma++;
   if(comma==2)        //extract latitude from string
   latitude+=gpsString[x+1];     
   else if(comma==4)        //extract longitude from string
   longitude+=gpsString[x+1];
   x++;
  }
  int l1=latitude.length();
  latitude[l1-1]=' ';
  l1=longitude.length();
  longitude[l1-1]=' ';
  lcd.clear();
  lcd.print("Lat:");
  lcd.print(latitude);
  lcd.setCursor(0,1);
  lcd.print("Long:");
  lcd.print(longitude);
  i=0;x=0;
  str_lenth=0;
  delay(20);
 }
}

  void accel()
  {
   sv1 = analogRead(ap1);            
   delay(2);                     
  sv2 = analogRead(ap2);            
  delay(2);                 
  sv3 = analogRead(ap3);            
   Serial.print("\n Xsensor1 = " );                       
   Serial.print(sv1);      
   Serial.print("\t Ysensor2 = " );                       
   Serial.print(sv2);      
   Serial.print("\t Zsensor3 = " );                       
   Serial.print(sv3);      
   //delay(1000);
  }
  
 void init_sms1()
 {
  gps.println("AT+CMGF=1");
  delay(400);
  gps.println("AT+CMGS=\"+919540136238\"");   // use your 10 digit cell no. here
  delay(400);
 }
 
  void init_sms2()
 {
  gps.println("AT+CMGF=1");
  delay(400);
  gps.println("AT+CMGS=\"+919540136238\"");   // use your 10 digit cell no. here
  delay(400);
 }
 
void send_data(String message)
{
  gps.println(message);
  delay(200);
}

void send_sms()
{
  gps.write(26);
}

void msg1()
{
  init_sms1();
  send_data("Accident occured at location left side :-\n");  
}
void msg2()
{
  init_sms2();
  send_data("Accident occured at location left side :-\n");  
}

void msg3()
{
  init_sms1();
  send_data("Accident occured at location right side:-\n");  
}

void msg4()
{
  init_sms2();
  send_data("Accident occured at location right side:-\n");  
}

void tracking()
{
    send_data("Latitude:");
    send_data(latitude);
    send_data("Longitude:");
    send_data(longitude);
    send_sms();
    delay(5000);
}
void trigger()
{
    lcd.clear();
   duration=0;
 do{
  lcd.setCursor(0,0);
  lcd.print("if you are alive");
  lcd.setCursor(0,1);
  lcd.print("press button");
  butt=digitalRead(but);
  duration++;
  delay(500);
  }while(duration<10);
 butt=digitalRead(but);
 if(butt==LOW)
 {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("person is alive");
  lcd.setCursor(0,1);
  lcd.print("Tank God");
  msg1();
  send_data("person in normal condition");
  send_sms();
  delay(5000);
  msg2();
  send_data("person in  normal condition");
  send_sms();
  delay(6000);
  gps.println("ATD9540136238;");
   delay(10000); 
  lcd.clear();
  Serial.println("fine");
 }
 if(butt==HIGH)
 {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("critical state");
  lcd.setCursor(0,1);
  lcd.print("oops...");
  msg1();
  send_data("person in critical condition");
  send_sms();
  delay(5000);
  msg2();
  send_data("person in critical condition");
  send_sms();
  delay(6000);
  gps.println("ATD8130737341;");
   delay(10000); 
  lcd.clear();
  Serial.println("critical condtion");
 }
}
