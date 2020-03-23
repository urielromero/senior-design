  
    #include <LowPower.h>
    //#include "LowPower.h"

    #include <SoftEasyTransfer.h>
    
    #include <Automaton.h>
    //#include "Atm_blink.h"

    #include <SoftwareSerial.h>
    SoftwareSerial mySerial(2, 3);

    SoftEasyTransfer ET;  // create bluetooth object

 int tempPin = 0;
      class LM335
    {
        float cal;
        int pin;
      public:
        LM335(float mCal, int mPin);
        float measureV();
        float measureK();
        float measureC();
        float measureF();
        float measureRankine();
    }; 
    

//  int tempPin = 0;
    LM335::LM335(float mCal, int mPin)
    {
      cal = mCal;
      pin = mPin;
    }
    float LM335::measureV()
    {
      float retVal = (float) analogRead(pin);
      retVal = (retVal * cal) / 1024.0;
      return retVal;
    }
    float LM335::measureF()
    {
      return (((measureV() / 0.01) - 273.15) * 1.8) + 38;// original 32 but changed to 55 to calibrate.
    }
    // use 3.3 for when not using usb power supply
    LM335 mTemp(5, 0);//supply volts, analog pin





    //Analog Hall Effect Input
    int hallPin = A1;            // Read value from 0 to 180 from  pin A1
    int hallValue = 0;          // Assigns analog read to hallValue variable
    boolean tempHall = false;   // send boolean to Fob to cotrol LED

  
    // Analog Power Consumption Pin
    int voltageRead = A2;

  
    //int masterGreen = 9;  IN ARDUINI UNO
    int masterGreen = 8;
    
    // 4 red LEDs to indicate issue
    int tempLED = 6 ;
    int buckleLED = 4;
    int voltageLED = 5;
  
  
    Atm_led ledT, ledB, ledV;  // GREEN NOT NEEDED
    boolean reset = false;



     // Data Struct

     
     struct SEND_DATA_STRUCTURE{
     //put your variable definitions here for the data you want to send
     //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
     int temperature;
     boolean buckled;
     };

    SEND_DATA_STRUCTURE data;  // data structure name
  
  
  void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    //
    mySerial.begin(9600);
    //start the library, pass in the data details and the name of the serial port.
    ET.begin(details(data), &mySerial);

  
    // mini 3.3 V supply pin
    // used as 3.3V Supply
    pinMode(7,OUTPUT);
    digitalWrite(7,HIGH);


    // green LED set as output
    pinMode(masterGreen,OUTPUT);
    
    // set up red LEDs using automaton
    ledT.begin(tempLED).blink(100,200);
    ledB.begin(buckleLED).blink(100,200);
    ledV.begin(voltageLED).blink(100,200);
 
  }
  
  void loop() {

    
  automaton.run();

  
  //Temperature
  int tempF = mTemp.measureF();
  
  int tempReading = analogRead(tempPin);

  //int tempF = 700;



//   int tempReading = analogRead(tempPin);
//  // Call thermometer function
//  int tempF=thermometer(tempReading);



 

  //Buckle
  hallValue = analogRead(hallPin); // Assign analog value to variable
  
  //hallValue = 600;  // Buckled if < 100

  boolean buckled;
  if (hallValue < 100){ buckled = true;} else {buckled = false;}

  //Voltage
  //int voltage = analogRead(voltageRead); // Limit is 300,  if > or < 300 turn on LED
  int voltage = 1000;  // NOT active if > 300


    data.temperature=tempF;
      data.buckled = buckled;

       
      
      ET.sendData();


    // if anything is wrong go to RED LEDS, start communication ELSE go into sleep mode for 8 sec and turn on Grenn LED not using automation
    if (tempF > 80 || tempF < 60 || hallValue > 100 || voltage < 300){

   
         // blink red LEDs

                int var;
              if (reset == false) {
            
                if ( (tempF > 80 || tempF < 60) && buckled && voltage > 300) {
                  var = 1;    //HIGH-LOW TEMPERATURE
                  tempHall = true;
                }
                if (hallValue > 100 && tempF < 80 && tempF > 60 && voltage > 300) {
                  tempHall = false;
                  var = 2;     //Not BUCKLED
                }
                if (voltage < 300 && tempF < 80 && tempF > 60 && hallValue < 100) {
                  var = 4;      //VOLTAG
                  tempHall = true;
                }
                if ((tempF > 80 || tempF < 60) && hallValue > 100 && voltage > 300) {
                  var = 5;      // HIGH-LOW TEMP & NOT BUCKLED
                  tempHall = false;
                }
                if ((tempF > 80 || tempF < 60) && hallValue < 100 && voltage < 300  ) {
                  var = 7;      // HIGH-LOW TEMP & LOW VOLTAGE
                  tempHall = true;
                }
                if (tempF < 80 && tempF > 60 && hallValue > 100 && voltage < 300  ) {
                  var = 9;    //  NOT BUCKLED && LOW VOLTAGE
                  tempHall = false;
                  //var =17;
                }
                if ((tempF > 80 || tempF < 60) && hallValue > 100 && voltage < 300  ) { // new
                  var = 12 ;   //  HIGH TEMP && NOT BUCKLED && LOW VOLTAGE
                  tempHall = false;   // All RED LEDs light up
               
              }
            
              //Reset period before going to new case
              if (reset == true) {
               // ledG.trigger(ledG.EVT_OFF);
                ledT.trigger(ledT.EVT_OFF);
                ledB.trigger(ledB.EVT_OFF);
                ledV.trigger(ledV.EVT_OFF);
                
                var = 0;
              }
            
              switch (var) {
                case 0:
                  reset = false;
                  ET.sendData();
                  break;
            
                case 1:   // TEMPERATURE
                 // ledG.trigger(ledG.EVT_OFF);
                  ledT.trigger(ledT.EVT_BLINK);
                  ledB.trigger(ledB.EVT_OFF);
                  ledV.trigger(ledV.EVT_OFF);
                  ET.sendData();
                  reset = true;
                  break;
                case 2:     //BUCKLE
                 // ledG.trigger(ledG.EVT_OFF);
                  ledT.trigger(ledT.EVT_OFF);
                  ledB.trigger(ledB.EVT_BLINK);
                  ledV.trigger(ledV.EVT_OFF);
                  ET.sendData();
                  reset = true;
                  break;
                case 4:       // LOW VOLTAGE
                 // ledG.trigger(ledG.EVT_OFF);
                  ledT.trigger(ledT.EVT_OFF);
                  ledB.trigger(ledB.EVT_OFF);
                  ledV.trigger(ledV.EVT_BLINK);
                  ET.sendData();
                  reset = true;
                  break;
                case 5:     // HIGH TEMP & NOT BUCKLED
                //  ledG.trigger(ledG.EVT_OFF);
                  ledT.trigger(ledT.EVT_BLINK);
                  ledB.trigger(ledB.EVT_BLINK);
                  ledV.trigger(ledV.EVT_OFF);
                  ET.sendData();
                  reset = true;
            
                  break;
                case 7:// HIGH TEMP & LOW VOLTAGE
               //   ledG.trigger(ledG.EVT_OFF);
                  ledT.trigger(ledT.EVT_BLINK);
                  ledB.trigger(ledB.EVT_OFF);
                  ledV.trigger(ledV.EVT_BLINK);
                  ET.sendData();
                  reset = true;
                  break;
                case 9: // NOT BUCKLED LOW VOLATGE
                //  ledG.trigger(ledG.EVT_OFF);
                  ledT.trigger(ledT.EVT_OFF);
                  ledB.trigger(ledB.EVT_BLINK);
                  ledV.trigger(ledV.EVT_BLINK);
                  ET.sendData();
                  reset = true;
                  break;
                case 12: //  HIGH TEMP && NOT BUCKLED && LOW VOLTAGE
                // ALL LEDs Light Up
                //  ledG.trigger(ledG.EVT_OFF);
                  ledT.trigger(ledT.EVT_BLINK);
                  ledB.trigger(ledB.EVT_BLINK);
                  ledV.trigger(ledV.EVT_BLINK);
                  ET.sendData();
                  reset = true;
                  break;
                default:
                  reset = true;
                  ET.sendData();
                  break;
              }
              
           
            //  delay(500);
              Serial.print("Temp: ");
              Serial.println(tempF);
              Serial.print("Buckled: ");
              Serial.println(hallValue);
              Serial.print("Voltage: ");
              Serial.println(voltage);
              Serial.print("Case: ");
              Serial.println(var);
            //  delay(500);

              }

      // start wireless communication  to FOB and cellphone.

      // define struct variable and send over to other bluetooth
    

      
      delay(250);           // this delat affects the blinking of leds. they may need to be turned off before sending data and going into the delay. 
      
      }else{
        
        //Turn on/blink green led using delays instead and go to sleep mode for 8 seconds


       //turn off red LEDs
       ledT.trigger(ledT.EVT_OFF);
       ledB.trigger(ledB.EVT_OFF);
       ledV.trigger(ledV.EVT_OFF);
       
       Serial.print("Powering down for 8 seconds\n");
       digitalWrite(masterGreen, HIGH);
       delay(100);
       digitalWrite(masterGreen, LOW);
       delay(50);
       digitalWrite(masterGreen, HIGH);
       delay(100);
       digitalWrite(masterGreen, LOW);
  
    delay(1000);
       //LowPower.powerDown(SLEEP_4S, ADC_OFF,BOD_OFF);   // Sleep for 8 seconds 
       //OR idle line below
//       LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
//                  SPI_OFF, USART0_OFF, TWI_OFF);
                  
        
        software_Reset();
        
        }
  
 }  
void software_Reset(){
  asm volatile(" jmp 0");
}

int thermometer (int temp_x){
       double tempK = log(10000.0 * ((1024.0 / temp_x - 1)));
       tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );//  Temp Kelvin
       float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
       float tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
       return tempF;
   }

