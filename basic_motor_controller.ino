   /* Tanks 
  Half Orange              11/tanke
  Orange                   12/tankf
              
  Reserve Low              9
  Reserve Full             10
  First Brown is Ground
  
  */
  


             const int  buttonPinmode = 4;
             int buttonState = 0;

             int manual = 0;
             int automode = 0;

             int buttonPushCounter1;   
             int buttonState1;         
             int lastButtonState1;     
     

  

            
           

             

             
            
             
             unsigned long timesystemdisplay1 =  millis();
              int togglesystemdisplay1 = 1;

             

           





                                  //INPUT SWITCHES for INDIVIDUAL PUMPS
             int manual_on1 = 2;


            

            
             


                                            //OUTPUT
             int relaypin1 = A3;
          
             int full_1 = 8;
             int low_1 = 7;
              int reserve_led =6;






                                //SENSOR TANK 1
             int sensor_empty1 = 11;
             int sensor_full1 = 12;

             

                                // Reserve Tank
            int sensor_reserve_low = 9;
            int sensor_reserve_full =10;  

             int reserve =0;     
                                
             




     void setup()
    {


               Serial.begin(9600);

               pinMode(buttonPinmode, INPUT);
               digitalWrite(buttonPinmode, HIGH);

              
               
               pinMode(relaypin1, OUTPUT);
               digitalWrite(relaypin1, LOW);




         
           

               pinMode(full_1, OUTPUT);
               digitalWrite(full_1, LOW);

               pinMode(low_1, OUTPUT);
               digitalWrite(low_1, LOW);

                 pinMode(reserve_led, OUTPUT);
               digitalWrite(reserve_led, LOW);

               


               pinMode(manual_on1, INPUT);
               digitalWrite(manual_on1, HIGH);

       


                                 //Initialization of Sensor of tank 1

                pinMode(sensor_full1, INPUT);
                digitalWrite(sensor_full1, HIGH);

                pinMode(sensor_empty1, INPUT);
                digitalWrite(sensor_empty1, HIGH);


                
                              //Initialization of Reserve Tank's Sensor

                   pinMode(sensor_reserve_low, INPUT);
                   digitalWrite(sensor_reserve_low, HIGH);

                   pinMode(sensor_reserve_full, INPUT);
                   digitalWrite(sensor_reserve_full, HIGH);





                

                  }

         void loop()
       {

         if (digitalRead(sensor_reserve_low) == HIGH  )
      {

        reserve=0;

        digitalWrite(reserve_led,HIGH);

      }

      if (digitalRead(sensor_reserve_full)==LOW)

      {
        reserve=1;
        digitalWrite(reserve_led,LOW);
        
      }

      
        if ((millis() - timesystemdisplay1 > 150) && digitalRead(sensor_empty1) == HIGH)
      {
        togglesystemdisplay1 = ! togglesystemdisplay1;


        digitalWrite(low_1, togglesystemdisplay1);
        timesystemdisplay1 = millis();
      }
      
     


     if (digitalRead(sensor_full1) == LOW && digitalRead(sensor_empty1) == LOW)
      {
         digitalWrite(full_1,HIGH);
          digitalWrite(low_1,LOW);
      }
      else if (digitalRead(sensor_full1) == HIGH)
      {
         digitalWrite(full_1,LOW);
      }

       if ( digitalRead(sensor_empty1) == LOW && digitalRead(sensor_full1) == HIGH)
       {
        digitalWrite(low_1,HIGH);
       }

        buttonState = digitalRead(buttonPinmode);

        if (buttonState == HIGH)
     {
           Serial.print(" button: ");
           Serial.println(automode);
      
            automode = 1 ;
            manual = 0;



      }
      
          else if (buttonState == LOW)
      {
             manual = 1;
             automode = 0;

      } 


      if (reserve==0)
      {
            digitalWrite(relaypin1, LOW);
           
            buttonPushCounter1 = 0;
           
           
            }
             
              else if (reserve==1)

         
       {
      
        
       


    if (automode == 1)
    {

     if (digitalRead(sensor_full1) == LOW)
      {

        digitalWrite(relaypin1, LOW);
       
         

      
      }
     

      else if ( digitalRead(sensor_empty1) == HIGH)
      {

        digitalWrite(relaypin1, HIGH);
        

      }



      buttonPushCounter1 = 0;
     
    }


    else if (manual == 1)

{

      
       buttonState1 = digitalRead(manual_on1);
       
       delay(50);

      if (buttonState1 != lastButtonState1)
      {

        if (buttonState1 == LOW)
        {
          if (buttonPushCounter1 == 2)
          {
            buttonPushCounter1 = 0;
          }
          buttonPushCounter1++;
        }
        else
        {
          Serial.println("off");
        }
           }
           
      lastButtonState1 = buttonState1;
      if (buttonPushCounter1 == 2 || digitalRead(sensor_full1) == LOW )
      {
        
        digitalWrite(relaypin1, LOW);
        buttonPushCounter1 = 0;
      }
      else if ( buttonPushCounter1 == 1  && (digitalRead(sensor_empty1) == HIGH || digitalRead(sensor_empty1) == LOW))
      {
        digitalWrite(relaypin1, HIGH);

      }         
     }
       }
       }
