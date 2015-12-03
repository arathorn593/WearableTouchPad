// Example by Tom Igoe 
 
import processing.serial.*; 
 
Serial myPort;    // The serial port
PFont myFont;     // The display font
String inString;  // Input string from serial port
int lf = 10;      // ASCII linefeed 
int xPos = 0; 
float oldX = 0.0;
float oldY = 0.0;
boolean inCurve = false;
void setup() { 
  size(800,800); 
  // You'll need to make this font with the Create Font Tool 
  //myFont = loadFont("ArialMS-18.vlw"); 
  //textFont(myFont, 18); 
  // List all the available serial ports: 
  printArray(Serial.list()); 
  // I know that the first port in the serial list on my mac 
  // is always my  Keyspan adaptor, so I open Serial.list()[0]. 
  // Open whatever port is the one you're using. 
  myPort = new Serial(this, Serial.list()[0], 9600); 
  myPort.bufferUntil(lf); 
  background(0);
} 

void draw() { 
  //background(0);
  // get the ASCII string:


   if (inString != null) {
     // trim off any whitespace:
     inString = trim(inString);

       String[] vals = inString.split(",");
     if(vals.length == 2){
       // convert to an int and map to the screen height:
       float x = float(vals[0]);
       float y = float(vals[1]);       
       
       x = map(x, -150, 150, 0, height);
       y = map(y, -150, 150, 0, width);
       // draw the line:
       stroke(127,34,255);
       strokeWeight(4);
       arc(x-2, y-2, 4, 4, 0, 2*PI);
       if(inCurve){
          line(oldX, oldY, x, y); 
       }
       
       inCurve = true;
       oldX = x;
       oldY = y;
     } else {
        inCurve = false; 
     }
   } else {
     inCurve = false;
   }
} 

void mouseClicked() {
  println("clear");
  background(0); 
}
 
void serialEvent(Serial p) { 
  inString = p.readStringUntil(lf); 
} 