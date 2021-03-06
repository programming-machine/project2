#include <Arduino.h>
#include <SimpleDHT.h>
#include <math.h>

using byte = unsigned char; 
using namespace std; 



#define      t_LED 2
#define      t_RED 5
#define      t_EOL 255
#define      t_D13 13
#define       t_ON 1
#define      t_OFF 100
#define    t_BLINK 3
#define    t_GREEN 4
#define      t_SET 6
#define   t_STATUS 7
#define     t_LEDS 8
#define  t_VERSION 9
#define     t_HELP 10
#define     t_TEMP 20
#define     t_ADD  254
#define     token_length 15


byte lookUp_array[token_length][4] = 
{{'l','e',3,t_LED},{'r','e',3,t_RED},{'e','o',3,t_EOL},{'d','1',3,t_D13},{'o','n',2,t_ON},
{'o','f',3,t_OFF},{'b','l',5,t_BLINK},{'g','r',5,t_GREEN},{'s','e',3,t_SET},{'s','t',6,t_STATUS},
{'l','e',4,t_LEDS},{'v','e',7,t_VERSION},{'h','e',4,t_HELP},{'t','e',4,t_TEMP},{'a','d',3,t_ADD}};

int pinDHT22 = 7;
SimpleDHT22 dht22(pinDHT22);

char buffer [40];
byte newChar;
byte job = 0; 
byte index = 0 ; 
byte length = 0; 
byte tokenBuffer [30];
byte first_char;
byte second_char;
byte count_letter = 0;
byte token_buffer_count =0;
byte command_length =0;
byte count_space = 0;
byte final_length = 0 ; //just added
byte last_color = t_RED;
byte led = 13;
word interval = 500;
byte count_digits = 0;
byte number_array [10];
byte done_counting = 0 ;
byte temp = 0;
byte status_led = 0;
byte status_d13 = 0;
byte blink_state_LED = 0; 
long blink_time_LED = 0;

byte blink_state_d13 = 0; 
long blink_time_d13 = 0;





// project 2 variables ***************************************
byte count_Space_ADD = 0;
long temp_read_time = 0;
float temperature = 0;
float humidity = 0;
byte number_array1_add [10] = {0};
byte number_array2_add [10] = {0};
byte count_digits_number1 = 0;
byte count_digits_number2 = 0;
int number1_Add = 0;
int number2_Add = 0;
int addition = 0;
byte number1taken = 0;
byte addition_array[10] = {0};



byte temp1 = 0;
byte temp2 = 0;




// project 2 funtions
void ReadTempHumid(){

        
      if (millis() - temp_read_time >  5000){
            


        int err = SimpleDHTErrSuccess;
          if ((err = dht22.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
          Serial.print("Read DHT22 failed, err="); 
          Serial.println(err);
          return;
        }
  
        temp_read_time = millis();

        }
        
    
}













// project 1 funtions

void turn_on_D13 (){
    digitalWrite(led, HIGH );
}

void turn_off_D13 (){
    digitalWrite(led, LOW );
}








void blink_D13 ( ){

    
    if (blink_state_d13 == 0){

        return;
    }
    if (blink_state_d13 == 1){
        turn_on_D13();
        if (millis() - blink_time_d13 >  interval){
            blink_state_d13 ++;
            blink_time_d13 = millis();
        }
    }
    if (blink_state_d13 == 2){
        turn_off_D13();
        if (millis() - blink_time_d13 >  interval){
            blink_state_d13 --;
            blink_time_d13 = millis();
        }
    }
}



void turn_red_LED (){
    digitalWrite(3, LOW);
    digitalWrite(2, HIGH );


}

void turn_green_LED (){
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH );

}


void turn_on_LED (){
    if (last_color == t_GREEN){
        turn_green_LED();
    }else{
        turn_red_LED();
    }

}


void turn_off_LED (){
    
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH );

}




void blink_led () {

    if (blink_state_LED == 0){

        return;
    }
    if (blink_state_LED == 1){
        if ( last_color == t_GREEN ){
            turn_green_LED();
        }else{
            turn_red_LED();
        }
        if (millis() - blink_time_LED >  interval){
            blink_state_LED ++;
            blink_time_LED = millis();
        }
    }
    if (blink_state_LED == 2){
        turn_off_LED();
        if (millis() - blink_time_LED >  interval){
            blink_state_LED --;
            blink_time_LED = millis();
        }
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(led, OUTPUT);
    pinMode(2,OUTPUT);
    pinMode(3,OUTPUT);

}

void loop()
{
    //blinking LED
    blink_led();
    blink_D13 ();
    ReadTempHumid();
    switch (job){
        case 0: 
            Serial.print("Enter a command: ");
            job ++;
            break;
        case 1:
            while (Serial.available()){
                newChar = Serial.read(); // read the incoming data as string
                buffer[index] = newChar;
                Serial.print( buffer[index]);
                
                // impelenting the backspace

                if(newChar != 13){
                    index ++;
                }else{
                    buffer[index] = 0;
                    length = index;
                    job ++;
                    Serial.print('\n');
                    
                    //Serial.print((byte)buffer[length]); // works
                    index = 0;
                    //count_letter = 0;
                    break;
                }
            }

                break; 
        case 2:
                
                for (byte i = 0 ; i < length  ; i++){
                        
                    if (buffer[i] != ' ' ){
                        count_space = 0;
                        if (count_letter == 0){
                                first_char  = (byte)buffer[i];
                                //Serial.print((char)first_char);  // works
                        }else if( count_letter == 1){
                                second_char = (byte)buffer[i];
                                //Serial.print((char)second_char); // works
                        }

                        if (buffer[i] != 0){
                            count_letter ++;
                        }

                        if ( i == length -1 )
                        {
                        command_length = count_letter;    
                        }
                              
                    }else{
                        count_space ++;
                        command_length = count_letter;    
                        // }
                        count_letter = 0;
                    }

                   if ( (count_space == 1|| i == length -1) &&  command_length !=0  ) {
                      final_length = command_length; 
                      //Serial.println( final_length );

                            // look up
                        for (byte i = 0; i < token_length ; i ++){
                             if (lookUp_array[i][0] == first_char && lookUp_array[i][1] == second_char && 
                                lookUp_array[i][2] == final_length ){
                                //Serial.println("hi");
                                tokenBuffer[token_buffer_count] = lookUp_array[i][3];
                                token_buffer_count ++; 
                            }
                        }

                    }
                    if (tokenBuffer[0] == t_SET && tokenBuffer[1] == t_BLINK){
                            
                        if ( buffer[i] >= 48 && buffer[i] <= 57 ){
                            number_array[count_digits] = buffer[i];
                            count_digits ++;
                            
                        }
                                        
                    }

                    if (tokenBuffer[0] == t_ADD){
                  

                      if ( buffer[i] >= 48 && buffer[i] <= 57 && number1taken == 0) { 
              
                            number_array1_add[count_digits_number1] = buffer[i];
                            count_digits_number1 ++;
                      }else{
                        if (count_digits_number1 > 0){
                          number1taken = 1;
                        }
                      }

                      if (number1taken == 1 && buffer[i] >= 48 && buffer[i] <= 57) {
                            number_array2_add[count_digits_number2] = buffer[i];
                            count_digits_number2 ++;
                      }

                    }
                    

                }


                //Serial.print( interval );

                Serial.println();

                tokenBuffer[token_buffer_count] = t_EOL;

                switch (tokenBuffer[0])
                {
                case t_LED:
                    switch (tokenBuffer[1])
                    {
                    case t_RED:
                        status_led = 1;
                        turn_red_LED();
                        last_color = t_RED;
                        break;
                    case t_GREEN:
                        status_led = 1;
                        turn_green_LED();
                        last_color = t_GREEN;
                        break;
                    case t_BLINK:    
                        status_led = 2;
                        blink_state_LED = 1;
                        blink_time_LED = millis();
                        break;
                    case t_ON:
                        blink_state_LED = 0;
                        status_led = 1;
                        turn_on_LED();
                        break;
                    case t_OFF:
                        status_led = 0;
                        turn_off_LED();
                        blink_state_LED = 0;
                        break;
                    default:
                        break;
                    }
                break;
                case t_HELP:
                    Serial.println();
                    Serial.println();
                    Serial.println( "led: bi-color LED on the board" );
                    Serial.println( "D13: D13 LED on the beard" );
                    Serial.println( "on: turns on" );
                    Serial.println( "off: turns off" );
                    Serial.println( "green: truns Green" );
                    Serial.println( "red: truns RED" );
                    Serial.println( "blink: LED blinks" );
                    Serial.println( "set blink: setting up the interval for blinking" );
                    Serial.println( "Status leds: Prints the status of LEDs" );
                    Serial.println( "Version: Prints the version of program" );
                    Serial.println();
                    Serial.println();
                break;

                case t_D13:

                    switch (tokenBuffer[1])
                    {
                    case t_ON:
                        status_d13 = 1;
                        blink_state_d13 = 0;
                        turn_on_D13();
                        break;
                    case t_OFF:
                        status_d13 = 0;
                        blink_state_d13 = 0;
                        turn_off_D13();
                        break;
                    case t_BLINK:
                        status_d13 = 2;
                        blink_state_d13 = 1; 
                        blink_time_d13 = millis();
                        break;
                        
                    default:
                        break;
                    }
                break;

                case t_SET:
                    if ( tokenBuffer[1] == t_BLINK ){
                        temp = count_digits - 1 ;
                        interval = 0;
                        for (byte i = 0; i < count_digits ; i ++) {
                        interval += ( number_array[i]-48 ) * pow(10,temp ) ;
                        temp --;
                            if ( temp < 0 ){
                                 break;
                            }
                        }
                    }
                break;

                case t_STATUS:
                    
                    if (tokenBuffer[1] == t_LEDS){
                        if (status_led == 0 ){
                            Serial.println("LED is off");
                        }else if ( status_led == 1 ){
                            Serial.println("LED is on");
                        }else if ( status_led == 2 ){
                            Serial.println("LED is blinking");
                        }
                        if (status_d13 == 0){
                            Serial.println("D13 is off");
                        }else if ( status_d13 == 1 ){
                            Serial.println("D13 is on");
                        }else if ( status_d13 == 2 ){
                            Serial.println("D13 is blinking");
                        }
                    }         
                break;

                case t_VERSION:
                    Serial.println( "VERSION 3" );

                break;

                case t_TEMP:
                      Serial.print((float)temperature); Serial.print(" *C, ");
                      Serial.print((float)humidity); Serial.println(" RH%");
                break; 

                case t_ADD:
                        
                        temp1 = count_digits_number1 - 1 ;
                        number1_Add = 0;
                        for (byte i = 0; i < count_digits_number1 ; i ++) {
                        number1_Add += ( number_array1_add[i]-48 ) * pow(10,temp1 ) ;
                        temp1 --;
                            if ( temp1 < 0 ){
                                 break;
                            }
                        }


                        temp2 = count_digits_number2 - 1 ;
                        number2_Add = 0;
                        for (byte i = 0; i < count_digits_number2 ; i ++) {
                        number2_Add += ( number_array2_add[i]-48 ) * pow(10,temp2 ) ;
                        temp2 --;
                            if ( temp2 < 0 ){
                                 break;
                            }
                        }

                        // for (int c = 9 ; c >= 0 ; c-- ){
                        //  for (int i = count_digits_number1 - 1; i >= 0; i-- ){
                        //   for (int j = count_digits_number2 -1 ; j >= 0 ; j--){
                            
                        //     if ( ( number_array2_add[j]-48 ) + ( number_array1_add[i]-48 ) < 10 ){
                        //       addition_array[c] = ( number_array2_add[j]-48 ) + ( number_array1_add[i]-48 ) ;
                        //     }else{
                        //       addition_array[c] = ( number_array2_add[j]-48 ) + ( number_array1_add[i]-48 ) -10 ;
                        //        addition_array[c - 1] = addition_array[c - 1] + 1 ;
                               
                        //     }
                        //     } //
                        //   }

                        //    //Serial.print("jo");
                        //  }

                        addition = number1_Add + number2_Add ;

                         Serial.println("adding: ");
                        // for (byte i = 0; i < 9; i ++ ){
                        //   if (addition_array[i] != 0){
                        //     Serial.print(addition_array[i]);
                        //   }
                        // }
                        Serial.print(addition); 

                break;
                     
                default:
                    Serial.println("INVALID command");
                    break;

                }
                

                Serial.println(  );
                job = 0;
                count_letter = 0;
                command_length = 0;
                for (byte i = 0; i < length; i ++){
                        buffer[i] = 0;
                    }
                token_buffer_count = 0;

                for (byte i = 0; i < token_buffer_count; i ++){
                        tokenBuffer[i] = 0;
                    }
                //interval = 0;
                count_digits = 0;
                done_counting = 0;

                // project2
                count_digits_number1 = 0;
                count_digits_number2 = 0;
                number1taken = 0;
                number1_Add = 0;
                number2_Add = 0;
                addition = 0;

                for (byte i =0; i < count_digits_number1; i ++){
                  number_array1_add[i] = 0 ;
                }

                for (byte i =0; i < count_digits_number2; i ++){
                  number_array2_add[i] = 0 ;
                }

                for (byte i =0; i < 10; i ++){
                  addition_array[i] = 0 ;
                }

                break;
        default:
                break;  
    }
}