#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

SoftwareSerial SIM900(10, 11);

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
char inchar;
int balchk=A2;
int temp=0;

int tank_empty;
int tank_full;
int relaypin1 = A5;

int on = A4;
int manual_off =A1;

int sensor_empty=9;
int sensor_full=8;
int fullsms=0;

int emptysms=1;
int emptysmssent=0;
int motor=A0;
int a=0,b=0,c=0,d=0;
int8_t answer;
//int onModulePin = 12;
int Fresh_Start = 1;
String readString ="";

void setup()
{ 
  
  
  lcd.begin(16, 2);
  lcd.clear();
   // pinMode(onModulePin, OUTPUT);
    //pinMode(13,OUTPUT);
    //digitalWrite(13,LOW);

   pinMode(balchk,INPUT);
  digitalWrite(balchk,HIGH);
  
  Serial.begin(19200);                 // the Sertial baud rate   
  Serial.println("Starting...");

  SIM900.begin(19200);               // the GPRS baud rate   

  delay(3000);

  //power_on();

  delay(10000);

  while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || 
    sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );

  Serial.println("INITIATION COMPLETED ......");  
  lcd.setCursor(5,0);
  lcd.print("Network");
  lcd.setCursor(3,1);
  lcd.print("Registered"); 
  delay(4000); 
   
  

pinMode(motor, OUTPUT);
digitalWrite(motor, LOW);
  
pinMode(relaypin1, OUTPUT);
digitalWrite(relaypin1, LOW);

pinMode(tank_full,OUTPUT);
digitalWrite(tank_full,LOW);  

pinMode(tank_empty,OUTPUT);
digitalWrite(tank_empty,LOW);

pinMode(on,INPUT);
digitalWrite(on,HIGH);  

pinMode(manual_off,INPUT);
digitalWrite(manual_off,HIGH);

pinMode(sensor_full,INPUT);
digitalWrite(sensor_full,HIGH);

pinMode(sensor_empty,INPUT);
digitalWrite(sensor_empty,HIGH);
lcd.clear();


  
}

void loop()
{
   readString ="";
    while(SIM900.available() > 0) 
    {
      inchar = SIM900.read(); 
      readString+=inchar;
      
       Serial.println("Incomming Characters ARE = ");           
       Serial.println(readString);                                          
    } 

    
  lcd.setCursor(0,1);
  lcd.print("ON   OFF   CHECK");
  
if(digitalRead(on)==LOW)
{
    delay(200);
   if(digitalRead(sensor_full)==HIGH)
    {
      digitalWrite(relaypin1, HIGH);
      digitalWrite(motor,HIGH);
      lcd.setCursor(0,0);
      lcd.print("   Motor ON     ");
      delay(2000);
    }
    
else
    {
          digitalWrite(relaypin1, LOW);
          digitalWrite(motor,LOW);
      
    }
  
}


if(digitalRead(manual_off)==LOW)
{
  delay(200);
  digitalWrite(relaypin1, LOW);
  digitalWrite(motor,LOW);
  lcd.setCursor(0,0);
    lcd.print("   Motor OFF    ");
    delay(2000);
  
}

else if( digitalRead(balchk)==LOW)
{
  delay(200);
  SIM900.println("ATD*400#;");
  delay(1000);
}

else if( readString.indexOf( "+CUSD" ) >= 0 )
{
           lcd.clear();
           String MSG = getValuedisplay(readString, '"' , 1);
           
           lcd.print(MSG);
           temp=1;  //-------------------------------------prevents the display of water level during balance enquiry
          
           for(int scroll=0; scroll<60;scroll++)         // Scroll 26 positions (string length) to the left:
             { 
                 lcd.setCursor(0,0);                      // Set the cursor position:               
                 lcd.scrollDisplayLeft();
                 delay(350);                             // Wait a bit:
              }
          lcd.clear(); 
          temp=0;    //-------------------------------------activates the display of water level status on lcd
}       
   
 
 // if(Fresh_Start == 1)
  //{  
     if( readString.indexOf( "+CMT" ) >= 0 )
      {
       Serial.println("Incoming SMS Received"); 
        String SMS_No = getValue(readString, '"' , 1);  
        Serial.println(SMS_No);
        delay(1000);
        //lcd.setCursor(0,0);
        //lcd.print(SMS_No);
        
        if(SMS_No=="1415" || SMS_No=="+9771415")  //Change the number
        {
          SIM900.println("ATD98xxxxxxxx;"); //change the number
          delay(20000);
          SIM900.println("ATH");

        }
        Serial.println("SMS ANSWERED ----- WAITING FOR NEW");
     
      }

      

  
    
    else if( readString.indexOf( "RING" ) >= 0 )
       { 
        //// I am looking for the word RING sent from my phone
        Serial.println("Incoming CALL Received");
        String Call_No = getValue(readString, '"' , 1);
        //String SMS_No = "+30"+Call_No ;
        Serial.println("----------- CALL VALUES ARE ------------");
        Serial.println("----------- CALL NO =  ------------");
        Serial.println(Call_No);
        Serial.println("----------- SMS NO =  ------------");
        //Serial.println(SMS_No);
        if(Call_No=="98xxxxxxxx")
        {
    
           if (a==1 && b==1 && c==0)
                        {
                          delay(3000);
                          SIM900.println("ATH");
                          delay(1000);
                          
                          digitalWrite(relaypin1, HIGH);      //turns on the motor
                          //...............................................................code to SMS informing Pump is turned ON
                          //delay(5000);...................................................delay required if SMS is activated
                          fullsms=1;
                          digitalWrite(motor,HIGH);

                          lcd.setCursor(0,0);
                          lcd.print("   Motor ON     ");
                          delay(2000);
                          
                          /*Call_No="0";
                          
                          while( SIM900.available() > 0) SIM900.read();    // Clean the input buffer*/
                          Serial.println("CALL PROCESSED AND MOTOR IS TURNED ON");
                        }
                     SIM900.println("ATH");   
          }
          else
          {
            SIM900.println("ATH");
            Serial.println("UNKNOWN NUMBER REJECTED");
          }
         Serial.println("WAITING FOR NEW CALL");
       
      }
   //}
  // else
   //{
 
    if(digitalRead(sensor_full)==LOW && digitalRead(sensor_empty)==LOW && temp==0)
     {
      digitalWrite(tank_full,HIGH);
      digitalWrite(tank_empty,HIGH);
      Serial.println("Tank is full...");
      lcd.setCursor(0,0);
      lcd.print("  Tank is FULL  ");
      
      a=0;
     
      if(fullsms==1)
      {
         SIM900.println("AT+CMGF=1");
         //insert code to send SMS saying "the tank is FULL and the Pump is Turned OFF"
     
          delay(1000);
          SIM900.println("AT+CMGS=\"98xxxxxxxx\"\r");

          delay(1000);
          SIM900.println("Tank is Full and the Water Pump is Turned OFF");
          SIM900.println((char)26);
        
          
         /* String Call_No="0";          
          while( SIM900.available() > 0) SIM900.read();    // Clean the input buffer
          delay(1000);*/

          
          fullsms=0;
          emptysmssent=0;
          emptysms=1;
          digitalWrite(relaypin1, LOW);   //Turns off the motor
          digitalWrite(13, LOW);
          lcd.setCursor(0,0);
          lcd.print("   Motor OFF    ");
          delay(2000);
          //delay(5000);
          digitalWrite(motor,LOW);
      }
     digitalWrite(relaypin1, LOW);   //Turns off the motor
     digitalWrite(motor,LOW);
     
     d=0;
    }
   else if(digitalRead(sensor_full)==HIGH && digitalRead(sensor_empty)==HIGH && temp==0)
    {
    digitalWrite(tank_full,LOW);
    digitalWrite(tank_empty,LOW);
    Serial.println("Tank is empty");
    lcd.setCursor(0,0);
    lcd.print("  Tank is EMPTY ");
    if(c==1)
    {
      c=0;
    }
   if(emptysms==1)
        {          
          SIM900.println("AT+CMGF=1");    //insert code to send SMS saying "the tank is FULL and the Pump is Turned OFF"
          delay(1000);
          SIM900.println("AT+CMGS=\"98xxxxxxxx\"\r");
          delay(1000);
          SIM900.println("Tank is about to Empty Please call to Turn ON the Water Pump");
          SIM900.println((char)26);
          
          while( SIM900.available() > 0) SIM900.read();    // Clean the input buffer
          delay(1000);
          c=1;
          
          emptysms=0; 
          emptysmssent=1;
          fullsms=1;
          b=1; 
        }

        d++;
        if(d==100)     //assuming tank empty at the initial condition when the system is turned ON
          {
            c=0;
            a=1;
          }
     }
   else if(digitalRead(sensor_empty)==LOW && digitalRead(sensor_full)==HIGH && temp==0)
     {
      digitalWrite(tank_full,LOW);
      digitalWrite(tank_empty,HIGH);
      lcd.setCursor(0,0);
      lcd.print("  Medium Level  ");
      Serial.println(" Tank have water");
      //if (emptysmssent==0)
      
        emptysms=1;
      
      if(digitalRead(sensor_full)==HIGH)
      {
        a=1;
        b=1;
      }
     
      d=0;
     }
//}
 // }
  //lcd.clear(); 
}

void power_on(){

  Serial.println("POWERING ON");   
  uint8_t answer=0;

  // checks if the module is started
  answer = sendATcommand("AT", "OK", 2000);
  if (answer == 0)
  {
    // power on pulse
    //digitalWrite(onModulePin,HIGH);
   // delay(3000);
    //digitalWrite(onModulePin,LOW);
    Serial.println("Connected to the network...");
    // waits for an answer from the module
    while(answer == 0){     // Send AT every two seconds and wait for the answer
      answer = sendATcommand("AT", "OK", 2000); 
      Serial.println("Trying Again...");   
    }
  }

}


int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout){

  uint8_t x=0,  answer=0;
  char response[100];
  unsigned long previous;

  memset(response, '\0', 100);    // Initialize the string

  delay(100);

  while( SIM900.available() > 0) SIM900.read();    // Clean the input buffer

  SIM900.println(ATcommand);    // Send the AT command 
  Serial.println(ATcommand);

  x = 0;
  previous = millis();

  // this loop waits for the answer
  do{
    if(SIM900.available() != 0){    
      // if there are data in the UART input buffer, reads it and checks for the asnwer
      response[x] = SIM900.read();
      x++;
      // check if the desired answer  is in the response of the module

      Serial.println(response);

      if (strstr(response, expected_answer) != NULL)    
      {
        answer = 1;
      }
    }
    // Waits for the asnwer with time out
  }
  while((answer == 0) && ((millis() - previous) < timeout));   

  return answer;
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 
    0, -1        };
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}


String getValuedisplay(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 
    0, -1        };
  int maxIndex = 50;//data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
