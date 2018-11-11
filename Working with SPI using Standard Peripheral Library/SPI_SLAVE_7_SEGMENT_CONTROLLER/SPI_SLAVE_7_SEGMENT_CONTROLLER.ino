
/*
 *    -A--
 *   F    B
 *   |    |
 *    -G--
 *   E    C
 *   |    |
 *    -D--
 * 
 * Digits  ABCDEFGH
 * ---------------------
 * 0       11111100
 * 1       00001100
 * 2       11011010
 * 3       11110010
 * 4       01100110
 * 5       10110110
 * 6       10111110
 * 7       11100000
 * 8       11111110
 * 9       11110110
 * A       11101110
 * B       11111110
 * C       10011100
 * D       11111100
 * E       10011110
 * F       10001110
 */
 




const char segmentPins[7] = {7,6,5,4,3,2,8};
const char segment1 = 9;

const int digits[10] = {
0xFC, // 0
0x0C, // 1
0xDA, // 2
0xF2, // 3
0x66, // 4
0xB6, // 5
0xBE, // 6
0xE0, // 7
0xFE, // 8
0xF6, // 9
};



void SPI_SlaveInit(void)
{
  // Set MISO output
  pinMode(MISO, OUTPUT);
  
  // Enable SPI 
  SPCR = (1<<SPE);
  
  // Enable interrupts
  SPCR |= _BV(SPIE);
}


volatile char buff[2];
volatile char execute = 0x00;
volatile char currentData = 0x00;
volatile char index = 0;

ISR (SPI_STC_vect){
  currentData = SPDR;
  if( (currentData != 0x00) &&( execute != 0x01)){
  if(index <2){
    buff[index++] = currentData;
  }else{
    index = 0;
    buff[index] = currentData;
  }
  }else{
    if(index != 0 ){
    execute = 0x01;
    index = 0;
    }
  }
}





void setupLEDPins(){
  for(int i=0; i < 7; i++ ){
    pinMode(segmentPins[i], OUTPUT);
  }
  pinMode(segment1, OUTPUT);
}



char isBitSet(int value, int bitPosition){

  return ((value >> bitPosition) & (char)1 );
}


void setLEDOutput(char value, int digit){
digitalWrite(digit, HIGH);
for(int i=0; i < 7; i ++){

  if(isBitSet((value >> 1), i) == 0x01){
    digitalWrite(segmentPins[6-i], HIGH);
    
  }else{
    digitalWrite(segmentPins[6-i],LOW);
  }
  delay(50);
}
  
}

void clearLED(){
digitalWrite(segment1, LOW);
for(int i=0; i < 7; i ++){
    digitalWrite(segmentPins[6-i], LOW);
  delay(50);
}
  
}


void setup() {

SPI_SlaveInit();
setupLEDPins();

}


void setDigit(char digit){
  setLEDOutput(digit,segment1);
}

void setAll(){
digitalWrite(segment1, HIGH);
for(int i=0; i < 7; i ++){
    digitalWrite(segmentPins[6-i], HIGH);
}
  
}
void animate(){
  digitalWrite(segment1, HIGH);
  digitalWrite(segmentPins[0], HIGH);
  delay(50);
  digitalWrite(segmentPins[1], HIGH);
  delay(50);
  digitalWrite(segmentPins[0], LOW);
  delay(50);
  digitalWrite(segmentPins[2], HIGH);
  delay(50);
  digitalWrite(segmentPins[1], LOW);
  delay(50);
  digitalWrite(segmentPins[3], HIGH);
  delay(50);
  digitalWrite(segmentPins[2], LOW);
  delay(50);
  digitalWrite(segmentPins[4], HIGH);
  delay(50);
  digitalWrite(segmentPins[3], LOW);
  delay(50);
  digitalWrite(segmentPins[5], HIGH);
  delay(50);
  digitalWrite(segmentPins[4], LOW);
  delay(50);
  digitalWrite(segmentPins[6], HIGH);
  delay(50);
  digitalWrite(segmentPins[5], LOW);
  delay(50);
  digitalWrite(segmentPins[7], HIGH);
  delay(50);
  digitalWrite(segmentPins[6], LOW);
  delay(50);
 digitalWrite(segment1, HIGH);
  digitalWrite(segmentPins[0], HIGH);
  delay(50);
  digitalWrite(segmentPins[1], HIGH);
  delay(50);
  digitalWrite(segmentPins[0], LOW);
  delay(50);
  digitalWrite(segmentPins[2], HIGH);
  delay(50);
  digitalWrite(segmentPins[1], LOW);
  delay(50);
  digitalWrite(segmentPins[3], HIGH);
  delay(50);
  digitalWrite(segmentPins[2], LOW);
  delay(50);
  digitalWrite(segmentPins[4], HIGH);
  delay(50);
  digitalWrite(segmentPins[3], LOW);
  delay(50);
  digitalWrite(segmentPins[5], HIGH);
  delay(50);
  digitalWrite(segmentPins[4], LOW);
  delay(50);
  digitalWrite(segmentPins[6], HIGH);
  delay(50);
  digitalWrite(segmentPins[5], LOW);
  delay(50);
  digitalWrite(segmentPins[7], HIGH);
  delay(50);
  digitalWrite(segmentPins[6], LOW);
  delay(50);
}


/*
 * Commands:
 * 
 * 0x01 - Clears led, No trailing data.
 * 0x02 - SET a digit in led, 1 trailing msg contains data.
 * 0x03 - Animates the  segment display, no trailing data
 * 
 * 
 * 
 */

void commandSwitches(){

  switch(buff[0]){

    case 1 :
    clearLED();
    break;

    case 2:
    setDigit(digits[buff[1]]);
    break;

    case 3:
     animate();
    break;
    default:
   setAll();
    break;    
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  // may be we have received a command.
if ( execute == 0x01 ){
  commandSwitches();
  execute =0x00;
}


}
