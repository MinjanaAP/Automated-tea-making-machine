#include<Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define NOTE_CS8 4435
#define NOTE_CS7 2217
#define NOTE_AS7 3729
#define NOTE_AS6 1865
#define NOTE_F7  2794

#define ONE_WIRE_BUS 11
#define ONE_WIRE_BUS2 10

float Temp = 0;
float Htemp =0;
char key2;

LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the LCD address

 byte rowPins[4] = {9,8,7,6};
byte colPins[4] = {5,4,3,2};

char keys[4][4] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'},
};

Keypad key = Keypad( makeKeymap(keys), rowPins, colPins,4,4);

int val, honeyVal,cinnamonVal,boilerVal;
Servo servo;
Servo Servo01;

void setup() {
 
  pinMode(53,OUTPUT); // blender
  pinMode(51,OUTPUT); //fan
  pinMode(37,OUTPUT); // water Heater
  pinMode(49,OUTPUT);// water pump to blender 12v
  pinMode(47,OUTPUT); // distill motor IN
  pinMode(45,OUTPUT); // distill motor OUT
  pinMode(43,OUTPUT); // Honey
  pinMode(41,OUTPUT); // Cinoman
  pinMode(39,OUTPUT); // 12 valve hot water
  pinMode(35,OUTPUT); //boiler
  pinMode(13,OUTPUT); // buzzer
  pinMode(33,OUTPUT); // 24v waste valve

  pinMode(A7,INPUT); // water bucket
  pinMode(A1,INPUT);// cinnamon level
  pinMode(A3,INPUT);//Honey level
  pinMode(A0,INPUT); //Waste
  pinMode(A5,INPUT); // distilation water level
  pinMode(A6,INPUT);// boiler water level

  servo.attach(14);
  Servo01.attach(12);
  servo.write(0);

  lcd.begin();
  lcd.backlight();
  
  arrow();
  lcdSetup();
  // waterHeater();
  Serial.begin(9600);
  tone();


  tone2();
     
}

void loop() { //check water level and temperature of water
  checking();
    mixWater();
      addFlavors();
      servedTea();
  if(key2 == 9){
  char key1 = key.waitForKey();
  // arrow();
  if(key1 == '6' ){
    waterLevel();
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Water Level : ");
    lcd.setCursor(14,1);
    lcd.print(val);
    delay(3000);
    honeyLevel();
  lcd.setCursor(1,3);
  lcd.print("Honey Level : ");
    lcd.setCursor(14,3);
    lcd.print(honeyVal);

  delay(2000);
  cinnamonLevel();
  lcd.setCursor(1,3);
  lcd.print("Cinnamon Level : ");
    lcd.setCursor(17,3);
    lcd.print(cinnamonVal);

  delay(2000);

    // lcdSetup();
  }else if(key1 == '5'){
      addLeaves();
      char key1 = key.waitForKey();
      if(key1 == '0'){ // A denotes end of tea leaves adding process
      blendLeaves();
      boilLeaves();
      distilVapor();
      mixWater();
      addFlavors();
      servedTea();
    }
      else if(key1 == 'B'){ // still adding tea leaves
        addLeaves();
    }  
  }else if(key1 == '8'){
      boilLeaves();
      distilVapor();
      mixWater();
      addFlavors();
      servedTea();    

  }else if(key1 == '#'){
    clean();
  }
  lcdSetup();

}else{
  checking();
}
}

int waterLevel(){
  val = analogRead(A7);
  Serial.println(val);
  return(val);

 }

 int honeyLevel(){
   honeyVal = analogRead(A3);
   return(honeyVal);
 }

 int cinnamonLevel(){
   cinnamonVal = analogRead(A1);
   return(cinnamonVal);
 }


void lcdSetup(){
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("WELCOME!");
  lcd.setCursor(2,1);
  lcd.print("Natural~~Tea");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("The Taste Of Nature!");
  delay(2000);
   lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("The Taste Of Nature!");

  waterLevel();
  lcd.setCursor(1,3);
  lcd.print("Water Level : ");
  if(val > 950){
    lcd.setCursor(14,3);
    lcd.print("ERROR");
  }else if(val > 680){
    lcd.setCursor(14,3);
    lcd.print("LOW");
  }
  
  else{
    lcd.setCursor(14,3);
    lcd.print("Normal");
  }
  delay(2000);
  honeyLevel();
  lcd.setCursor(1,3);
  lcd.print("Honey Level : ");
    
    // lcd.print(honeyVal);
    if(honeyVal > 850){
      lcd.setCursor(14,3);
      lcd.print("LOW  ");
    }else{
         lcd.setCursor(14,3);
    lcd.print("Normal");
    }

  delay(2000);
  cinnamonLevel();
  lcd.setCursor(0,3);
  lcd.print("Cinnamon Lev : ");
    
    // lcd.print(cinnamonVal);
       if(cinnamonVal > 850){
         lcd.setCursor(14,3);
      lcd.print("LOW  ");
    }else{
         lcd.setCursor(14,3);
    lcd.print("Normal");
    }

  delay(2000);


  
  heaterTemp();
  lcd.setCursor(1,2);
  lcd.print("Water in Heater "); // print water temperature
  lcd.setCursor(0,3);
  lcd.print("Temperature :");
  lcd.setCursor(14,3);
  lcd.print(Htemp);
  lcd.setCursor(19,3);
  lcd.print("C");
  delay(2000);

    waterTemp();
  lcd.setCursor(1,2);
  lcd.print("Distillation Water "); // print water temperature
  lcd.setCursor(0,3);
  lcd.print("Temperature :");
  lcd.setCursor(14,3);
  lcd.print(Temp);
  lcd.setCursor(19,3);
  lcd.print("C");
  delay(2000);



}


void arrow(){
  for(int i=0;i<19;i++){
  lcd.setCursor(i,1);
  lcd.print("-~");
  lcd.setCursor(i,2);
  lcd.print("-~");
  delay(200);
  lcd.setCursor(i,1);
  lcd.print("  ");
   lcd.setCursor(i,2);
  lcd.print("  ");
  }
}

void addLeaves(){ //Add tea leaves to blender
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Add 3 or more");
  lcd.setCursor(0,2);
  lcd.print("Fresh Tea Leaves ...");
  delay(6000);
  lcd.setCursor(0,3);
  lcd.print("Finish Adding?(Y/N)");

}

void blendLeaves(){ //blend tea Leves , power up the relay of blender untill blend the tea leaves.
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Adding Water...");
  digitalWrite(49,HIGH); // turn on water motor 1
  delay(2000);
  digitalWrite(49,LOW);// turn off th motor 1
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Tea Leaves is");
  lcd.setCursor(0,2);
  lcd.print("Blending...");
  delay(1000);
  digitalWrite(53,HIGH);//power up blender rellay
  delay(2000); //time that get for blending.
  digitalWrite(53,LOW); // power off the blender relay
  digitalWrite(49,HIGH); // turn on water motor 1
  delay(4000);
  digitalWrite(49,LOW);// turn off th motor 1
  delay(1000);  
 
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Adding tea leaves ");
  lcd.setCursor(0,2);
  lcd.print("to boiler...");
  
  for(int i=0; i<=100; i++){
  servo.write(i);
  delay(20); // power on the servo
  }
  delay(10000);
  
  for(int j=100; j>=0; j--){
  servo.write(j);
  delay(20);} // power off the servo
  lcd.clear();
}

int boilerWLevel(){
  boilerVal = analogRead(A6);
  return boilerVal;
}



// void distilation(){
//   // if(analogRead(A5)<500){
//   //   digitalWrite(47,HIGH);
//   //    digitalWrite(45,LOW);
//   //    delay(2000);
//   //    digitalWrite(47,LOW);
     
//   // }else{
//   //   digitalWrite(47,HIGH);
//   //   digitalWrite(45,HIGH);
//   // }
//    digitalWrite(47,HIGH);
//    delay(1000);
//      digitalWrite(47,LOW);
//      digitalWrite(45,HIGH);
//      delay(1000);
//   digitalWrite(45,LOW);

// }

void boilLeaves(){     
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Boil the water... &");
  lcd.setCursor(0,2);
  lcd.print("Collecting vapor...");
  if(analogRead(A6)<750){
    digitalWrite(35,HIGH);
     delay(208000);
     // time that get for boilling.. //start to boil the 2nd tank
  }else{
    alert();
    lcd.setCursor(0,3);
  lcd.print("Boiler is Empty..");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Check the boiler $");
  lcd.setCursor(0,2);
  lcd.print("Press 9");
  char key2 = key.waitForKey();
  }

  // digitalWrite(35,LOW);
  //send vapor to distill..
}

void distilVapor(){
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Collect vapor &");
  lcd.setCursor(0,2);
  lcd.print("Distilling....");
 
 
  while(analogRead(A5)>390){
    digitalWrite(45,HIGH);
   digitalWrite(47,HIGH);
    waterTemp();
    Serial.println(Temp);
    delay(1000);
    if(Temp>28){
       digitalWrite(51,HIGH);
    }
   
}
 digitalWrite(45,LOW);
  digitalWrite(47,LOW);

while(analogRead(A5)<780){
  digitalWrite(45,HIGH);
}
digitalWrite(45,LOW);

while(analogRead(A5)>390){
    digitalWrite(45,HIGH);
   digitalWrite(47,HIGH);
    waterTemp();
    Serial.println(Temp);
    delay(1000);
    if(Temp>28){
       digitalWrite(51,HIGH);
    }
   
}
 digitalWrite(45,LOW);
  digitalWrite(47,LOW);

while(analogRead(A5)<780){
  digitalWrite(45,HIGH);
}
digitalWrite(45,LOW);

  // delay(92000);

  // delay(187000); // boiling and distilling vapor
  // digitalWrite(47,LOW);
  // digitalWrite(45,LOW);
   digitalWrite(35,LOW);
  //  digitalWrite(51,LOW);

  //  distilation();
  // while(analogRead(A5)>570){
  //    digitalWrite(47,LOW);
  //   digitalWrite(45,HIGH);
  //        delay(2000);
  //    digitalWrite(45,LOW);
  // }
  // digitalWrite(47,LOW);
  //   digitalWrite(45,LOW);
}

void mixWater(){ //add boiling water and mix
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Mix with Hot Water..");
  digitalWrite(39,HIGH);
  delay(6000);
  digitalWrite(39,LOW);
  lcd.setCursor(0,2);
  lcd.print("Thanks for Waiting..");
  digitalWrite(39,HIGH);
  delay(2000);
  digitalWrite(39,LOW);
  // turn on solenoid valve 2 and add boiled water
}

void addFlavors(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Adding Flavors..");
  lcd.setCursor(0,1);
  lcd.print("Cinnoman(C) or ");
  lcd.setCursor(0,2);
  lcd.print("Honey(D)");
  lcd.setCursor(0,3);
  lcd.print("Both(B)-No flavor(#)");
  char key1 = key.waitForKey();
  // delay(4000);
      waterTemp();
    if(Temp<40){
      Serial.println(Temp);
      digitalWrite(51,LOW);
    }



  if(key1 == 'D'){
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Adding Cinoman...");
    digitalWrite(41,HIGH);
    delay(2000);
    digitalWrite(41,LOW);

  }else if(key1 == 'C'){
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Adding Honey...");
      digitalWrite(43,HIGH);
    delay(2000);
    digitalWrite(43,LOW);

  }else if(key1 == 'B'){
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Adding Both...");
    digitalWrite(41,HIGH);
    digitalWrite(43,HIGH);
    delay(2000);
    digitalWrite(41,LOW);
    digitalWrite(43,LOW);

  }else{
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Without Flavors...");
  }
  //on suitable water pump/s...

  delay(6000);
}

void servedTea(){
    lcd.clear();
  lcd.setCursor(0,2);
  lcd.print("Mixing...");

  servoTurnLeft();
  delay(500);
  servoTurnRight();
  delay(500);
  servoTurnLeft();
  delay(500);
  servoTurnRight();
  delay(500);
   servoTurnLeft();
  delay(500);
  servoTurnRight();
  delay(500);
  servoTurnLeft();
  delay(500);
  servoTurnRight();
  delay(500);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Your Tea is Ready...");
  lcd.setCursor(5,1);
  lcd.print("Taste the");
  lcd.setCursor(3,2);
  lcd.print("Taste of Nature!");
  delay(10000); 
  digitalWrite(51,LOW);
  // turn on last water pump and pump tea to cup--->
  loop();
}



void clean(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Cleaning Process....");
  digitalWrite(49,HIGH);// 12v motor add water
  delay(2000);
  digitalWrite(49,LOW);
  digitalWrite(53,HIGH);//blender
  delay(2000);
  digitalWrite(53,LOW);
  digitalWrite(49,HIGH);// 12v motor add water
  delay(2000);
  digitalWrite(49,LOW);

  for(int i=0; i<=100; i++){
  servo.write(i);
  delay(20); // power on the servo
  }
  delay(12000);
  digitalWrite(53,HIGH);//blender
  delay(2000);
  digitalWrite(53,LOW);
  
  for(int j=100; j>=0; j--){
  servo.write(j);
  delay(20);} 

  digitalWrite(33,HIGH); //power on waste valve
  delay(30000);
  digitalWrite(33,LOW);

  // open 2nd valve 
  arrow();
}

int waterTemp(){
   
  OneWire oneWire(ONE_WIRE_BUS);

  DallasTemperature sensors(&oneWire);
    sensors.requestTemperatures();
  Temp = sensors.getTempCByIndex(0);
  return Temp;

}
int heaterTemp(){
  
  OneWire oneWire(ONE_WIRE_BUS2);

  DallasTemperature sensors(&oneWire);
  sensors.requestTemperatures();
  Htemp = sensors.getTempCByIndex(0);
  return Htemp;
}
void tone(){

tone(13,  NOTE_CS7, 80);
delay(100);
tone(13,  NOTE_AS7, 80);
delay(200);

}

void tone2(){

  tone(13,  NOTE_CS8, 180);
delay(600);
tone(13,  NOTE_CS7, 180);
delay(100);
tone(13,  NOTE_AS7, 80);
delay(100);
tone(13,  NOTE_AS6, 80);
delay(100);
  tone(13,  NOTE_F7, 180);
delay(100);

}

void alert(){
      digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
    delay(100);
        digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
    delay(100);
        digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
    delay(100);
        digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
    delay(100);
}

void servoTurnLeft(){
    for(int i = 0; i<=150; i++){
  Servo01.write(i);
  delay(10);
  }
}

void servoTurnRight(){
  for(int i=150; i>=1; i--){
  Servo01.write(i);
  delay(10);}

}

void checking(){
   waterLevel();
   lcd.clear();
   lcd.setCursor(0,1);
   lcd.print("Checking....");
   delay(1000);

  if(val > 680){
    lcd.clear();
    lcd.setCursor(0,1);
    // lcd.print(val);
    lcd.print("Water Level is LOW");
    alert();
    lcd.setCursor(1,2);
    lcd.print("Add water & press 9");
     key2 = key.waitForKey();
    checking();  
  }else{
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Water Level Normal");
        delay(2000);

    
      honeyLevel();
      if(honeyVal > 850){
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Honey Level is LOW");
         alert();
        lcd.setCursor(1,2);
        lcd.print("Add Honey & press 9");
         key2 = key.waitForKey();
        checking();  
      }else{
            lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Honey Level Normal");
        delay(2000);

          cinnamonLevel();
          if(cinnamonVal > 850){
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("Cinnamon Level LOW");
             alert();
            lcd.setCursor(1,2);
            lcd.print("Add Cinnamon");
            lcd.setCursor(1,3);
            lcd.print("And press 9");
            char key2 = key.waitForKey();
             checking();
          }else{
                        lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("Cinnamon Lev Normal");
            delay(2000);

            waterTemp();
            if(Temp>35){
                lcd.clear();
                lcd.setCursor(0,1);
                lcd.print("Water Temp is HIGH");
                 alert();
                lcd.setCursor(1,2);
                lcd.print("Please Wait...");
                delay(2000);
                while(Temp>32){
                  digitalWrite(51,HIGH);
                  delay(2000);
                  waterTemp();
                   Temp = Temp;
                 lcd.setCursor(0,3);
                lcd.print(Temp);
                 
                }
                digitalWrite(51,LOW);
                 checking();
                key2 = 9;
            }else{
                lcd.clear();
                lcd.setCursor(1,0);
                lcd.print("Distillation Water "); // print water temperature
                lcd.setCursor(0,1);
                lcd.print("Temperature :");
                lcd.setCursor(14,1);
                lcd.print(Temp);
                lcd.setCursor(19,1);
                lcd.print("C (COLD)");
                delay(2000);

            
                if(analogRead(A0)<50){
                  lcd.clear();
                  lcd.setCursor(0,1);
                  lcd.print("Waste is full");
                  alert();
                  lcd.setCursor(1,2);
                  lcd.print("Empty the trash");
                  lcd.setCursor(1,3);
                  lcd.print("And press 9");
                  delay(2000);
                   char key2 = key.waitForKey();
             checking();


                }else{
                  lcd.clear();
                  lcd.setCursor(0,1);
                  lcd.print("Waste is Empty..");
                  delay(2000);
                    heaterTemp();
                  if(Htemp<80){
                    lcd.clear();
                    lcd.setCursor(1,0);
                      lcd.print("Water in Heater ");
                      lcd.setCursor(0,1);
                      lcd.print("Temperature is Low");
                      lcd.setCursor(1,2);
                      lcd.print("Please Wait...");
                      delay(2000);
                      while(Htemp<80){
                        digitalWrite(37,HIGH);
                        heaterTemp();
                        Htemp = Htemp;
                        lcd.setCursor(0,3);
                      lcd.print(Htemp);
                        lcd.setCursor(6,3);
                      lcd.print("C");
                      delay(1000);
                      }
                      digitalWrite(37,LOW);
                      // checking();
                                        
                  }else{
                       heaterTemp();
                      lcd.clear();
                    lcd.setCursor(1,0);
                      lcd.print("Water in Heater ");
                      lcd.setCursor(0,1);
                      lcd.print("Temperature ");
                       lcd.setCursor(0,3);
                      lcd.print(Htemp);
                        lcd.setCursor(6,3);
                      lcd.print("C");
                      delay(2000);
                    if(analogRead(A5)>880){
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Distillation");
                      lcd.setCursor(0,1);
                      lcd.print("Water Level is LOW");
                      alert();
                      lcd.setCursor(1,2);
                      lcd.print("Add water & press 9");
                      key2 = key.waitForKey();
                      checking();  
                    }else{
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Distillation");
                      lcd.setCursor(0,1);
                      lcd.print("Water Lev is Normal");
                      delay(2000);
                        lcd.clear();
                        lcd.setCursor(0,1);
                        lcd.print("Everything is OK");
                        tone2();
                        lcd.setCursor(1,2);
                        lcd.print("Ready for Tea..");
                        lcd.setCursor(1,3);
                        lcd.print("Press  5 ");
                        key2 = 9;
                    }
                  }
              
                }
            }
          }
      }
  }
}







