#include <WiFi.h>  
#include <FirebaseESP32.h>

FirebaseData fbdb;

#define btn1 18
#define btn2 19

int relay1 = 16; //Relay1
int relay2 = 17; //Relay2
int btnm =13;

int LDRSensor = 34;
int Sensordata, btnmstate;
int load1=1, load2=1, Power, Power1;

#define WIFI_SSID "Paul Nguyen" //Tên WiFi
#define WIFI_PASSWORD "12345678" //Password

//#define WIFI_SSID "Hung Nguyen" //Tên WiFi
//#define WIFI_PASSWORD "19082179" //Password

#define FIREBASE_HOST "https://sen2-14973-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "0SIBxnWnBMCJAwcbfZAqo7tkHRltaUlexUcqrFrk"

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void connect_wifi(){
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println ("");
  Serial.println ("Connected Succerfully with IP:");
  Serial.println(WiFi.localIP()); //WiFi.localIP() this command to get IP 
  Serial.println();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LDRSensor, INPUT);
  pinMode(relay1, OUTPUT);  
  pinMode(relay2, OUTPUT);  
  pinMode(btn1,INPUT_PULLUP);
  pinMode(btn2,INPUT_PULLUP);
  pinMode(btnm, INPUT_PULLUP);
  //connect to wifi
  connect_wifi();
  //connect to firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  // put your main code here, to run repeatedly:
  btnmstate = digitalRead(btnm);
  Sensordata = analogRead (LDRSensor);
  Firebase.setInt(fbdb,"LightSen", Sensordata);
  if (btnmstate == 1){
    //Manual_Mode_Pressbutton
    Led_Control();
  }else{
    //Auto_Mode
    datalightsen();
    Read_UART();
  }
//Print the sensor value on your serial monitor window
  Serial.print("Sensor value:");
  Serial.println(Sensordata);
  Serial.print("Mode State:");
  Serial.println(btnmstate);
  Serial.println("----------------");
  //Delay for 1 second to get clear output on the serial monitor
  delay(1000);
}

void Led_Control(){
   Btn_Read();
  if(Firebase.get(fbdb,"/Led1Status")){
    if(fbdb.dataType() == "string"){
      Power = fbdb.stringData().toInt();
    }
  }
  Relays();
  
  Btn_Read();
  if(Firebase.get(fbdb,"/Led2Status")){
    if(fbdb.dataType() == "string"){
      Power1 = fbdb.stringData().toInt();
    }
  }
  Relays();
}

void Btn_Read(){
  if(digitalRead(btn1)==LOW){
    Power=0;
    Firebase.setString(fbdb,"/Power", String(Power));
    load1 =! load1;
    Relays();
    Firebase.setString(fbdb, "Led1Status", String(load1));  
  }

  else if(digitalRead(btn2) == LOW){
    Power1=0;
    Firebase.setString(fbdb,"/Power1", String(Power1));
    load2 = !load2;
    Relays();
    Firebase.setString(fbdb, "/Led2Status", String(load2));
  }
}

void Relays(){
  if(Power==0){
     digitalWrite(relay1, load1);
  }else{
     digitalWrite(relay1, HIGH);
  }
  if(Power1==0){
     digitalWrite(relay2, load2);
  }else{
     digitalWrite(relay2, HIGH);
  }
}

void datalightsen(){
  if (Sensordata <= 1500 ){
    digitalWrite(relay1, LOW);
    delay(1000);
  }
  if (Sensordata > 1500 ) {
    digitalWrite(relay1, HIGH);
    delay(1000);
  }
}
void Read_UART()
  {
   while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.print("Data nhan la:");
    Serial.println(inputString);
    if(inputString.indexOf("A") >= 0)
    {
      Serial.println("ON");
      digitalWrite(relay2,HIGH);
    }
    else if(inputString.indexOf("B") >= 0)
      {
      Serial.println("OFF");
      digitalWrite(relay2,LOW);
      }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}
