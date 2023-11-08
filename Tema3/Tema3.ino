// // Declare all the joystick pins
// const int pinSW = 2; // Digital pin connected to switch output
// const int pinX = A0; // A0 - Analog pin connected to X output
// const int pinY = A1; // A1 - Analog pin connected to Y output
 
// // Declare all the segments pins
// const int pinA = 12;
// const int pinB = 10;
// const int pinC = 9;
// const int pinD = 8;
// const int pinE = 7;
// const int pinF = 6;
// const int pinG = 5;
// const int pinDP = 4;
 
// const int segSize = 8;
// int index = 0;
 
// bool commonAnode = false; // Modify if you have common anode
 
// byte state = HIGH;
 
 
// byte dpState = LOW;
 
// byte swState = LOW;
// byte lastSwState = LOW;//debounce
 
// int xValue = 0;
// int yValue = 0;
 
// bool joyMoved = false;
// bool joyMovedFirst = false;
// //sa vedem daca am dat in stanga sau in dreapta, min in dreapta
// int minThreshold = 400;
// int maxThreshold = 600;
 
// //decimalPoint = 7;
 
// int segments[segSize] = {
//   pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
// };
 
// int position=7;
 
// bool stopDpBlink = false;
 
 
// void setup() {
//   // TODO: Initialize joystick pins
//   pinMode(pinSW, INPUT_PULLUP);
//   // Initialize all the pins
//   for (int i = 0; i < segSize; i++) {
//     pinMode(segments[i], OUTPUT);
//   }
 
// }
 
// void loop() {
 
//   xValue = analogRead(pinX);
//   yValue = analogRead(pinY);
//   swState = digitalRead(pinSW);
 
//   if(joyMoved == false && stopDpBlink == false){
//       blink(segments[position], state);
//   }
 
//   if(xValue > maxThreshold && joyMovedFirst == false ){
//     digitalWrite(segments[position], LOW);
//     position = 2;
//     joyMovedFirst = true;
//     stopDpBlink = true;
//     xValue = 0;
//   }
//   blink(segments[position], state);
//   //enter this direct on first
//   switch(position){
 
//     case 0: //stanga
//             if(xValue > maxThreshold && joyMoved == false){
//               position = 5;
//               joyMoved = true;
//               xValue = 0;
//             }//dreapta
//             else if(xValue < minThreshold && joyMoved == false){
//               position = 1;
//               joyMoved = true;
//               xValue = 0;
//             }
//             else if(yValue < minThreshold && joyMoved == false){
//               position = 6;
//               joyMoved = true;
//               yValue = 0;  
//             }
//     case 1: //stanga
//             if(xValue > maxThreshold && joyMoved == false){
//               position = 5;
//               joyMoved = true;
//               xValue = 0;
//             }//sus
//             else if(yValue > maxThreshold && joyMoved == false){
//               position = 6;
//               joyMoved = true;
//               yValue = 0;
//             }
//             else if(yValue < minThreshold && joyMoved == false){
//               position = 6;
//               joyMoved = true;
//               yValue = 0;  
//             }      
 
//     case 2: //stanga
//             if(xValue > maxThreshold && joyMoved == false){
//               position = 4;
//               joyMoved = true;
//               xValue = 0;
//             }//sus
//             else if(yValue > maxThreshold && joyMoved == false){
//               position = 6;
//               joyMoved = true;
//               yValue = 0;
//             }//jos
//             else if(yValue < minThreshold && joyMoved == false){
//               position = 3;
//               joyMoved = true;
//               yValue = 0;  
//             }
//     break;
//     case 3:
//             if(xValue > maxThreshold && joyMoved == false){
//               position = 4;
//               joyMoved = true;
//               xValue = 0;
//             }else if(yValue > maxThreshold && joyMoved == false){
//               position = 6;
//               joyMoved = true;
//               yValue = 0;
//             }else if(xValue < minThreshold && joyMoved == false){
//               position = 2;
//               joyMoved = true;
//               yValue = 0;  
//             }
         
 
 
//     break;
//     case 4: //dreapta
//             if(xValue < minThreshold && joyMoved == false){
//               position = 2;
//               joyMoved = true;
//               xValue = 0;
//             }else if(yValue > maxThreshold && joyMoved == false){
//               position = 6;
//               joyMoved = true;
//               yValue = 0;
//             }else if(yValue < minThreshold && joyMoved == false){
//               position = 3;
//               joyMoved = true;
//               yValue = 0;  
//             }
 
   
//     break;
//     case 5: //dreapta
//             if(xValue < minThreshold && joyMoved == false){
//               position = 1;
//               joyMoved = true;
//               xValue = 0;
//             }//sus
//             else if(yValue > maxThreshold && joyMoved == false){
//               position = 0;
//               joyMoved = true;
//               yValue = 0;
//             }//jos
//             else if(yValue < minThreshold && joyMoved == false){
//               position = 6;
//               joyMoved = true;
//               yValue = 0;  
//             }
 
   
//     break;
//     case 6: //sus
//             if(yValue > maxThreshold && joyMoved == false){
//               position = 0;
//               joyMoved = true;
//               yValue = 0;
//             }//jos
//             else if(yValue < minThreshold && joyMoved == false){
//               position = 3;
//               joyMoved = true;
//               yValue = 0;  
//             }
 
   
//     break;
//     default:
 
//     break;
//   }
//    Serial.println(position);
//   // if(xValue > maxThreshold && joyMoved == false ){
//   //   digitalWrite(segments[7], LOW);
//   //   joyMoved = true;
//   // }
//   if (xValue >= minThreshold && xValue <= maxThreshold) {
//     joyMoved = false;
//   }
//   if (yValue >= minThreshold && yValue <= maxThreshold) {
//     joyMoved = false;
//   }
 
// }

 
// void blink(int segment, byte decimalPointState){
//   digitalWrite(segment, decimalPointState);
//   delay(300);
//   digitalWrite(segment, !decimalPointState);
//   delay(300);
// }