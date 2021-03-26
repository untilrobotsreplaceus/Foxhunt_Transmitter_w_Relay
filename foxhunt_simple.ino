/*
  This sketch is designed to be used with an Arduino, 5v relay, transistor, and a Baofeng. 
  This sketch was adapted by AC3DS from the work of VA6MCP and VE6BTC. 
  March 2021

*/

#define tonehz 500       // The approximate frequency of the tones in hz. 
#define dit 90          // The length of the dit in milliseconds. The dah and pauses are derived from this.
#define rest 30000       // The amount of time between transmits in ms 60000 = 60seconds
#define longlength 4000  // The initial tone which is played to indicate the beginning of a transmission. length of long tone in milliseconds 4000 = 4 seconds
#define ptt_switch 7     // Arduino Digital Pin 7 is controlling the base (eBc) of the (2n2222A) transistor, which opens the relay to ground. When the relay connects to ground, the relay's internal switch moves from NC to NO (normally open), and activates the ptt_switch on the Baofeng.
#define audio 5          // Arduino Digital Pin 5 is the morse code output going to the mic. 


// 1 = dit , 2 = dah, 0 = space
// Letters/#                 A     B      C     D    E    F      G     H      I     J     K      L    M     N    O     P      Q      R     S    T    U      V     W      X     Y      Z       0        1      2       3       4       5       6       7       8       9
String morseTable[] = {"0", "12", "2111", "2121", "211", "1", "1121", "221", "1111", "11", "1222", "212", "1211", "22", "21", "222", "1221", "2212", "121", "111", "2", "112", "1112", "122", "2112", "2122", "2211", "22222", "12222", "11222", "11122", "11112", "11111", "21111", "22111", "22211", "22221"};


String formMorse(String input);
void playcode(String input);
void playtone(int note_duration);
String Text = "M     M     M     M     AC3DS";  // CHANGE THIS TO YOUR CALL SIGN
String code = formMorse(Text);
int duration;
int note = 1000000 / tonehz; //converts the frequency into period


void setup() { //set the pins to output mode
  pinMode(ptt_switch, OUTPUT);
  pinMode(audio, OUTPUT);
  pinMode(13, OUTPUT);               //this is to see how the cw looks with on board led pin 13
} 




void loop() {
  digitalWrite(ptt_switch, HIGH);     // By applying a voltage/current to the transistor's base, the transistor's emitter and collector are connected. 
  playtone(longlength);
  delay(250);
  playcode(code);
  digitalWrite(ptt_switch, LOW);  
  delay(rest);          
} 




void playtone(int note_duration) {
  long elapsed_time = 0;
  long startTime = millis();
  if (note_duration > 0) {
    digitalWrite(13, HIGH);                  // Lights up the LED for confirmation of what is being sent
    while (elapsed_time < note_duration) {
      digitalWrite(audio, HIGH);
      delayMicroseconds(note / 2);
      digitalWrite(audio, LOW);
      delayMicroseconds(note / 2);
      elapsed_time = millis() - startTime;
    }
    digitalWrite(13, LOW);
  }
  else { //if it's a pause this will run

    delay(dit * 2);
  }
}


void playcode(String input) {
  for (int i = 0; i < input.length(); i++) {
    if (input[i] == '0') { //See if it's a pause
      duration = 0;
    }
    else if (input[i] == '1') { //See if it's a dit
      duration = dit;
    }
    else if (input[i] == '2') { //See if it's a dah
      duration = dit * 3;
    }
    playtone(duration);
    delay(dit); // this is the morse code gap between each letter
} 
}




String formMorse(String input) {
  input.toUpperCase();
  String output = "";
  for (int i = 0; i < input.length() ; i++) {
    if (input[i] >= 65 && input[i] <= 90)
      output = output + morseTable[input[i] - 64] + '0';
    else if (input[i] >= 48 && input[i] <= 57)
      output = output + morseTable[input[i] - 21] + '0';
    else if (input[i] == 32)
      output = output + morseTable[0];
  }
  return output;
} 
