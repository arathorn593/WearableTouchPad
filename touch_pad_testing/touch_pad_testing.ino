#include <CapPin.h>

#include <Wire.h>
#include <Adafruit_ADS1015.h>

#define t 0
#define r 1
#define b 2
#define l 3

#define avg_size 15
#define loop_delay 0
#define read_delay 0
#define max_val 800 //maximum value for a valid touch

#define cap_thresh 3000 //min value for a touch to register on the cap sensors

#define zero 120.0
#define d 280.0

CapPin start_pin = CapPin(0);
CapPin end_pin = CapPin(1);
CapPin hr1_pin = CapPin(2);
CapPin am9_pin = CapPin(3);
CapPin pm3_pin = CapPin(4);
CapPin pm8_pin = CapPin(5);

bool in_drawing;

bool start_state;
bool end_state;
bool hr1_state;
bool am9_state;
bool pm3_state;
bool pm8_state;

bool start_state_old;
bool end_state_old;
bool hr1_state_old;
bool am9_state_old;
bool pm3_state_old;
bool pm8_state_old;

//int i;
float t_vals[avg_size];
float r_vals[avg_size];
float b_vals[avg_size];
float l_vals[avg_size];
float t_val;
float r_val;
float b_val;
float l_val;
bool touch_valid;

Adafruit_ADS1015 ads;

void setup() {
  in_drawing = false;
  start_state = false;
  start_state_old = false;
  end_state = false;
  end_state_old = false;
  hr1_state = false;
  hr1_state_old = false;
  am9_state = false;
  am9_state_old = false;
  pm3_state = false;
  pm3_state_old = false;
  pm8_state = false;
  pm8_state_old = false;
  
  /*
  t_val = float(analogRead(t));
  r_val = float(analogRead(r));
  b_val = float(analogRead(b));
  l_val = float(analogRead(l));  
  for(int i = 0; i < avg_size; i++){
    t_vals[i] = t_val;
    r_vals[i] = r_val;
    b_vals[i] = b_val;
    l_vals[i] = l_val;
  }
  */
  touch_valid = false;
  //i = 0;
  Serial.begin(9600);

  ads.begin();

}

float x;
float y;
float t_total;
float b_total;
float r_total;
float l_total;

float linearize(float val) {
    
    return exp((map(val, 0, 1023, 0, 20)) - 4);
}

void getAverages(){
    touch_valid = true;
    t_val = 0;
    b_val = 0;
    r_val = 0;
    l_val = 0;

    t_total = 0;
    b_total = 0;
    r_total = 0;
    l_total = 0;
    
    for(int i = 0; i < avg_size; i++){
        t_val = float(ads.readADC_SingleEnded(t));
        b_val = float(ads.readADC_SingleEnded(b));
        r_val = float(ads.readADC_SingleEnded(r));
        l_val = float(ads.readADC_SingleEnded(l));

        if(t_val > max_val || b_val > max_val ||
           r_val > max_val || l_val > max_val){
            touch_valid = false;
            return;
        } else {
            t_total += t_val;
            b_total += b_val;
            r_total += r_val;
            l_total += l_val;
        }
    }
    
    t_val = t_total / float(avg_size);
    b_val = b_total / float(avg_size);
    l_val = l_total / float(avg_size);
    r_val = r_total / float(avg_size);
}

float getX() {
    float tl_val = t_val - zero;
    float tr_val = r_val - zero;
    float br_val = b_val - zero;
    float bl_val = l_val - zero;

    float t_x = (tl_val * tl_val - tr_val * tr_val + d * d) / (2 * d);
    float b_x = (bl_val * bl_val - br_val * br_val + d * d) / (2 * d);

    return t_x + b_x;
}

float getY() {
    float tl_val = t_val - zero;
    float tr_val = r_val - zero;
    float br_val = b_val - zero;
    float bl_val = l_val - zero;

    float l_y = (tl_val * tl_val - bl_val * bl_val + d * d) / (2 * d);
    float r_y = (tr_val * tr_val - br_val * br_val + d * d) / (2 * d);

    return l_y + r_y;
}

float linearizeY(float x, float y) {
    float a = 200.0;
    return y * 100.0 / ((a / 2.0) * (exp(x / a) + exp (-x/a)));
}

float linearizeX(float x, float y) {
    float a = 110.0;
    return x * 150.0 / ((a / 2.0) * (exp(y / a) + exp(-y / a)));
}

void loop() {
  getAverages();
  /*
  if(touch_valid){
    x = getX();
    y = getY();
    Serial.print(int(x)); Serial.print(",");
    Serial.println(int(y));
  } else {
    Serial.println("null");
  }

  Serial.print(t_val); Serial.print(",");
  Serial.print(r_val); Serial.print(",");
  Serial.print(b_val); Serial.print(",");
  Serial.println(l_val);

  //Serial.println(r_val);
*/
  if(in_drawing){
    Bean.setLed(255, 0, 0);
  }
  float tl_val = t_val;
  float tr_val = r_val;
  float br_val = b_val;
  float bl_val = l_val;

  float x_offset = 20.0;
  float y_offset = 10.0;
  if(touch_valid){
      x = tr_val + br_val - (tl_val + bl_val);
      y = (tr_val + tl_val) - (br_val + bl_val);
      x += x_offset;
      y += y_offset;
      float y_new = linearizeY(x, y);
      float x_new = linearizeX(x, y);
      Serial.print(int(x_new)); Serial.print(",");
      Serial.println(int(y_new));
  } else {
      //if no writing, check buttons
      start_state = start_pin.readPin(2000) > cap_thresh;
      end_state = end_pin.readPin(2000) > cap_thresh;
      hr1_state = hr1_pin.readPin(2000) > cap_thresh;
      am9_state = am9_pin.readPin(2000) > cap_thresh;
      pm3_state = pm3_pin.readPin(2000) > cap_thresh;
      pm8_state = pm8_pin.readPin(2000) > cap_thresh;

      if(start_state && !start_state_old){
        Serial.println("start");
        in_drawing = true;
      } else if(end_state && !end_state_old && in_drawing) {
        Serial.println("end");
        in_drawing = false;
      } else if(hr1_state && !hr1_state_old && in_drawing) {
        Serial.println("1hr");
        in_drawing = false;
      } else if(am9_state && !am9_state_old && in_drawing) {
        Serial.println("9am");
        in_drawing = false;
      } else if(pm3_state && !pm3_state_old && in_drawing) {
        Serial.println("3pm");
        in_drawing = false;
      } else if(pm8_state && !pm8_state_old && in_drawing) {
        Serial.println("8pm");
        in_drawing = false;
      }


      start_state_old = start_state;
      end_state_old = end_state;
      hr1_state_old = hr1_state;
      am9_state_old = am9_state;
      pm8_state_old = pm3_state;
      pm8_state_old = pm8_state;
      
  }
  
/*
  float total_avg = (t_val + l_val + b_val + r_val) / 4;
  Serial.println(total_avg);
*/
  delay(loop_delay);
/*
  if(t_val < 1000){
  Serial.print("x: ");
  if(x < -9) {
    Serial.print(x);
  } else if(x <0){
    Serial.print(" "); Serial.print(x);
  } else if(x > 9){
    Serial.print(" "); Serial.print(x);
  } else {
    Serial.print("  "); Serial.print(x);
  }

  
  Serial.print(" y: ");
  if(y < -9) {
    Serial.print(y);
  } else if(y <0){
    Serial.print(" "); Serial.print(y);
  } else if(y > 9){
    Serial.print(" "); Serial.print(y);
  } else {
    Serial.print("  "); Serial.print(y);
  }
  Serial.println();
  }else{
    Serial.println("none");
  }
  delay(100);
  /*
  Serial.println(t_val);
  delay(5);
    /*
  Serial.print("t: "); Serial.print(t_val);
  Serial.print(" r: "); Serial.print(r_val);
  Serial.print(" b: "); Serial.print(b_val);
  Serial.print(" l: "); Serial.println(l_val);

  if(x < 0)
    Serial.print("x: ");
  else
    Serial.print("x:  ");
  Serial.print(x, 4);

  if(y < 0)
    Serial.print(" y: ");
  else
    Serial.print(" y:  ");
  Serial.println(y, 4);
  //Serial.println(tl_val);
  */
}
