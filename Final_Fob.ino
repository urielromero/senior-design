//This program will detect a certain temperature and
//turn an LED a certain color when that temparature is reached.
 
#include <LowPower.h>

#include <SoftEasyTransfer.h>

#include <Automaton.h>
//#include "Atm_blink.h"
#include "pitches.h"

#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

SoftEasyTransfer ET;  // create bluetooth object

    

//Defined LED for Overall State
int ledGreen = 7;

//Define LED for Temperature
int redTemp = 4;

//Define LED for Buckle switch
int redBuckle = 5;

// Define LEDs for Signal loss
int redSignal = 6;

//Define LED for Voltage
int redPower = 10;

//Alarm Sound
int alarmBuzz = 2;

//Power Consumption Analog Pin
int voltageRead = A2;



//Distance Variable
boolean signalLoss;


//Structure created to follow easier


//Counter
int count = 0;

//Atm class controls for LEDs
Atm_led ledT, ledB, ledS, ledV;
boolean reset = false;

//Data  Structure. Thsi should be the same as on the other arduino

struct RECEIVE_DATA_STRUCTURE{
     int temperature;
     boolean buckled;
     };

    RECEIVE_DATA_STRUCTURE data;  // data structure name



    

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); // 9600 bits per second

  //Wirelss Communication initialized and defined
  //start the library, pass in the data details and the name of the serial port.
  mySerial.begin(9600);
  
  ET.begin(details(data), &mySerial);

  //Pins Initialized
  ledT.begin(redTemp);
  ledB.begin(redBuckle);
  ledS.begin(redSignal);
  ledV.begin(redPower);

  ledT.blink(100, 200);
  ledB.blink(100, 200);
  ledS.blink(100, 200);
  ledV.blink(100, 200);

  //green led cannot be bliked with ATM class b/c sleep mode
  // use delays to blink
  
  pinMode(ledGreen, OUTPUT);
  pinMode(alarmBuzz, OUTPUT);

  //boolean reset = false; Redundant

  //initialize variable 
  
  
}


void loop() {


  // receive struct variables from other bluetooth 


  // Receive data only when availabl if not go into slee mode and signal with quick green blink 
// if (ET.receiveData()){
//  
//  Serial.println("Data Received");
//  
//  }else{
//    
//       Serial.println("No Data, Going to sleep");
//       digitalWrite(ledGreen, HIGH);
//       delay(50);
//       digitalWrite(ledGreen, LOW);
//       delay(50);
//       digitalWrite(ledGreen, HIGH);
//       delay(50);
//       digitalWrite(ledGreen, LOW);
//       LowPower.idle(SLEEP_4S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
//                  SPI_OFF, USART0_OFF, TWI_OFF);
//    }


    //Alternative
 // ET.receiveData();


    
//Signal loss code

  //count++;



// Test variables 

    int temp = 700;
    boolean buckled = true;
    signalLoss = false;
    
  //Voltage, checking battery
  //int voltage = analogRead(voltageRead); //Limit is 300, if < 300 then turn LED on.
    int voltage = 400;   // low if < 300


    
  //Change LED color based on condition or state
  int var;

// If anything wrong alert issue, else go to green and sleep mode
if ( temp > 80 || temp < 60 || buckled == false || signalLoss == true || voltage < 300) {



  if (reset == false) {
    if ((temp > 80 || temp < 60) && buckled == true  && signalLoss == false && voltage > 300){
      var = 1; //high temp
      
    }
    if (buckled == false && temp < 80 && temp > 60 && signalLoss == false && voltage > 300){
      var = 2; //unbuckled

    }
    if (signalLoss == true && temp < 80 && temp > 60 && buckled == true && voltage > 300){
     var = 3; //signal lost
     //add function to increase PA to reach babysafe
     
    } 
    if (voltage < 300 && temp < 80 && temp > 60 && buckled == true && signalLoss == false){
      var = 4; //low voltage (battery)
      
    }
    if((temp > 80 || temp < 60) && buckled == false && signalLoss == false && voltage > 300){
      var = 5;  //high temp & unbuckled
      
    }
    if((temp > 80 || temp < 60) && buckled == true && signalLoss == true && voltage > 300){
      var = 6;  //high temp and signal lost
      //add function to increase PA to reach babysafe 
       
    }
    if((temp > 80 || temp < 60) && buckled == true && signalLoss == false && voltage < 300){
      var = 7;  //high temp & low voltage(battery)
      
    }
    if(temp < 80 && temp > 60 && buckled == false && signalLoss == true && voltage > 300){
      var = 8;  //unbuckled & signal lost
      
    }
    if(temp < 80 && temp > 60 && buckled == false && signalLoss == false && voltage < 300){
      var = 9;  //unbuckled and low voltage(battery)
      
    }
    if(temp < 80 && temp > 60 && buckled == true && signalLoss == true && voltage < 300){
      var = 10; //signal lost & low voltage(battery)
      
    }
    if((temp > 80 || temp < 60) && buckled == false && signalLoss == true && voltage > 300){
      var = 11; //high temp & unbuckled & signal lost
      //add function to increase PA to reach babysafe
      
    }
    if((temp > 80 || temp < 60) && buckled == false && signalLoss == false && voltage < 300){
      var = 12; //high temp & unbuckled & low voltage(battery)
      
    }
    if((temp > 80  || temp < 60) && buckled == true && signalLoss == true && voltage < 300){
      var = 13; //high temp & signal lost & low voltage(battery)
      
    }
    if(temp < 80 && temp > 60 && buckled == false && signalLoss == true && voltage < 300){
      var = 14; //unbuckled & signal lost & low voltage(battery)
      
    }
    if((temp > 80  || temp < 60) && buckled == false && signalLoss == true && voltage < 300){
      var = 15; //All issues
    }


  }
  //Reset period before proceeding to new state
  if(reset == true){
      
      ledT.trigger(ledT.EVT_OFF);
      ledB.trigger(ledB.EVT_OFF);
      ledS.trigger(ledS.EVT_OFF);
      ledV.trigger(ledV.EVT_OFF);
      var = 0;  
  }
  switch(var){
    case 0:
    reset = false;
    break;

    case 1: //high temp
   
    ledT.trigger(ledT.EVT_BLINK);
    ledB.trigger(ledB.EVT_OFF);
    ledS.trigger(ledS.EVT_OFF);
    ledV.trigger(ledV.EVT_OFF);
    reset = true;
    break;
    
    case 2:     //unbuckled
    
    ledT.trigger(ledT.EVT_OFF);
    ledB.trigger(ledB.EVT_BLINK);
    ledS.trigger(ledS.EVT_OFF);
    ledV.trigger(ledV.EVT_OFF);
    reset = true;
    break;
    
    case 3:       //signal lost
    
    ledT.trigger(ledT.EVT_OFF);
    ledB.trigger(ledB.EVT_OFF);
    ledS.trigger(ledS.EVT_BLINK);
    ledV.trigger(ledV.EVT_OFF);
    reset = true;
    break;
    case 4:       //low voltage
    
    ledT.trigger(ledT.EVT_OFF);
    ledB.trigger(ledB.EVT_OFF);
    ledS.trigger(ledS.EVT_OFF);
    ledV.trigger(ledV.EVT_BLINK);
    reset = true;
    break;
    case 5:     //high temp & unbuckled
   
    ledT.trigger(ledT.EVT_BLINK);
    ledB.trigger(ledB.EVT_BLINK);
    ledS.trigger(ledS.EVT_OFF);
    ledV.trigger(ledV.EVT_OFF);
    reset = true;
  
    break;
    case 6: //high temp & signal lost
    
    ledT.trigger(ledT.EVT_BLINK);
    ledB.trigger(ledB.EVT_OFF);
    ledS.trigger(ledS.EVT_BLINK);
    ledV.trigger(ledV.EVT_OFF);
    reset = true;
    break;
    
    case 7: //high temp & low voltage
    
    ledT.trigger(ledT.EVT_BLINK);
    ledB.trigger(ledB.EVT_OFF);
    ledS.trigger(ledS.EVT_OFF);
    ledV.trigger(ledV.EVT_BLINK);
    reset = true;
    break;
    
    case 8:   // unbuckled & signal lost
    
    ledT.trigger(ledT.EVT_OFF);
    ledB.trigger(ledB.EVT_BLINK);
    ledS.trigger(ledS.EVT_BLINK);
    ledV.trigger(ledV.EVT_OFF);
    reset = true;
    break;
    
    case 9: // unbuckled & low voltage
    
    ledT.trigger(ledT.EVT_OFF);
    ledB.trigger(ledB.EVT_BLINK);
    ledS.trigger(ledS.EVT_OFF);
    ledV.trigger(ledV.EVT_BLINK);
    reset = true;
    break;
    
    case 10: //signal lost & low voltage
  
    ledT.trigger(ledT.EVT_OFF);
    ledB.trigger(ledB.EVT_OFF);
    ledS.trigger(ledS.EVT_BLINK);
    ledV.trigger(ledV.EVT_BLINK);
    reset = true;
    break;
    
    case 11: // high temp & unbuckled & signal lost
    
    ledT.trigger(ledT.EVT_BLINK);
    ledB.trigger(ledB.EVT_BLINK);
    ledS.trigger(ledS.EVT_BLINK);
    ledV.trigger(ledV.EVT_OFF);
    reset = true;
    break;
    
    case 12: //high temp & unbuckled & low voltage
   
    ledT.trigger(ledT.EVT_BLINK);
    ledB.trigger(ledB.EVT_BLINK);
    ledS.trigger(ledS.EVT_OFF);
    ledV.trigger(ledV.EVT_BLINK);
    reset = true;
    break;
    
    case 13: //high temp & signal lost & low voltage
    
    ledT.trigger(ledT.EVT_BLINK);
    ledB.trigger(ledB.EVT_OFF);
    ledS.trigger(ledS.EVT_BLINK);
    ledV.trigger(ledV.EVT_BLINK);
    reset = true;
    break;
    
    case 14: //unbuckled & signal lost & low voltage
    
    ledT.trigger(ledT.EVT_OFF);
    ledB.trigger(ledB.EVT_BLINK);
    ledS.trigger(ledS.EVT_BLINK);
    ledV.trigger(ledV.EVT_BLINK);
    reset = true;
    break;
    
    case 15:     //All issues
    
    ledT.trigger(ledT.EVT_BLINK);
    ledB.trigger(ledB.EVT_BLINK);
    ledS.trigger(ledS.EVT_BLINK);
    ledV.trigger(ledV.EVT_BLINK);
    reset = true;
    break;
    
    
    default:
    reset = true;
    break;  
  }
  
  //Print Message and results
  Serial.print("Case: "); 
  Serial.println(var); 
  Serial.print("\ntemp: "); 
  Serial.println(temp);
  Serial.print("Buckled: "); 
  Serial.println(buckled);  //Buckle Switch Sensor
  Serial.print("Signal loss "); 
  Serial.println(signalLoss); 
  Serial.print("Voltage: "); 
  Serial.println(voltage); 
 
  Serial.print("Counter: "); Serial.print(count); Serial.print("\n");
  delay(1500);

  } else {
    
    // Everything is ok, blink green led and go to sleep mode 

       ledT.trigger(ledT.EVT_OFF);
       ledB.trigger(ledB.EVT_OFF);
       ledV.trigger(ledV.EVT_OFF);
        
       
  
    LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
                  SPI_OFF, USART0_OFF, TWI_OFF);
    
        }
}
