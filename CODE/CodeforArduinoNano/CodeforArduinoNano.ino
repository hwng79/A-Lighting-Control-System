#include <SoftwareSerial.h>
#define Rx 11
#define Tx 12

SoftwareSerial mySerial(Rx, Tx);
#include "DS3231.h"//Memanggil RTC3231 Library
#include <Wire.h>  // iC Conection Library
#include <LiquidCrystal_I2C.h> //Libraries
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27,16,2);

#define bt_clock  A0
#define bt_up     A1
#define bt_down   A2
#define bt_timer  A3


// Init DS3231
DS3231  rtc(SDA, SCL);

// Init a Time-data structure
Time  t; //pencacah string time()

int hh = 0, mm = 0, ss = 0, dd = 0, bb = 0, set_day;
int yy = 0;
String Day = "  ";


struct timer {
  int StartHH = 0;
  int StartMM = 0;
  int FinishHH = 0;
  int FinishMM = 0;
  int timer = 0;
};
struct timer RS,T1,T2,T3,T4;

int setMode = 0, setAlarm = 0, alarmMode=1;


int stop =0, mode=0, flag=0;

void setup(){
Serial.begin(9600);
mySerial.begin(9600);
rtc.begin(); // memulai koneksi i2c dengan RTC

pinMode(bt_clock, INPUT_PULLUP);
pinMode(bt_up,    INPUT_PULLUP);
pinMode(bt_down,  INPUT_PULLUP);
pinMode(bt_timer, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0,0);  //Show "TIME" on the LCD

  lcd.setCursor (0,0);
  lcd.print(" Real Time Clock ");
  lcd.setCursor (0,1);
  lcd.print("  4Timer 1Relay  ");
  delay (2000);
  lcd.clear();

stop=EEPROM.read(50);  
if(stop==0){  
}else{
mode=1;WriteEeprom ();delay (20);
mode=2;WriteEeprom ();delay (20);
mode=3;WriteEeprom ();delay (20);
mode=4;WriteEeprom ();delay (20);
mode=0;
}
EEPROM.write(50,0); 

ReadEeprom();
//Set RTC Untuk Pertama kali
//rtc.setDOW(2);     // Set Day-of-Week to SUNDAY
//rtc.setTime (00, 9, 50); 
//rtc.setDate(12, 11, 2017);  
}

void loop(){  

    t = rtc.getTime();
    Day = rtc.getDOWStr(1);

    if (setMode == 0){
        hh = t.hour,DEC;
        mm = t.min,DEC;
        ss = t.sec,DEC;
        dd = t.date,DEC;
        bb = t.mon,DEC;
        yy = t.year,DEC;
        //if(t.hour>12){hh=t.hour-12;}// for removing 24 houre
        //else{hh=t.hour;}
    }  

    if(setAlarm==0){
        lcd.setCursor(0,0); 
        lcd.print((hh/10)%10);
        lcd.print(hh % 10); 
        lcd.print(":");
        lcd.print((mm/10)%10);
        lcd.print(mm % 10);
        lcd.print(":");
        lcd.print((ss/10)%10);
        lcd.print(ss % 10); 
        lcd.print("  T:"); 
        lcd.print(rtc.getTemp(),0);
        lcd.write(223); 
        lcd.print("C");
        lcd.print("  "); 

        lcd.setCursor(1,1);
        lcd.print(Day);
        lcd.print(" ");
        lcd.print((dd/10)%10);
        lcd.print(dd % 10); 
        lcd.print("/");
        lcd.print((bb/10)%10);
        lcd.print(bb % 10);
        lcd.print("/"); 
        lcd.print((yy/1000)%10);
        lcd.print((yy/100)%10);
        lcd.print((yy/10)%10);
        lcd.print(yy % 10);
    }


    setupClock();
    setTimer();
    delay (100);
    blinking();

    //T1.timer ON
    if (T1.timer==1 && alarmMode==1 && hh==T1.StartHH && mm==T1.StartMM)  {mySerial.print("A\n");}
    //T1.timer OFF
    if (T1.timer==1 && alarmMode==1 && hh==T1.FinishHH && mm==T1.FinishMM){mySerial.print("B\n");}

    //T2.timer ON
    if (T2.timer==1 && alarmMode==1 && hh==T2.StartHH && mm==T2.StartMM)  {mySerial.print("A\n");}
    //T2.timer OFF
    if (T2.timer==1 && alarmMode==1 && hh==T2.FinishHH && mm==T2.FinishMM){mySerial.print("B\n");}


    //T3.timer ON
    if (T3.timer==1 && alarmMode==1 && hh==T3.StartHH && mm==T3.StartMM)  {mySerial.print("A\n");}
    //T3.timer OFF
    if (T3.timer==1 && alarmMode==1 && hh==T3.FinishHH && mm==T3.FinishMM){mySerial.print("B\n");}

    //T4.timer ON
    if (T4.timer==1 && alarmMode==1 && hh==T4.StartHH && mm==T4.StartMM)  {mySerial.print("A\n");}
    //T4.timer OFF
    if (T4.timer==1 && alarmMode==1 && hh==T4.FinishHH && mm==T4.FinishMM){mySerial.print("B\n");}

    delay (100);
}


void blinking (){
//BLINKING SCREEN
//Set Clock
    if (setAlarm <2 && setMode == 1){lcd.setCursor(0,0);  lcd.print("  ");}
    if (setAlarm <2 && setMode == 2){lcd.setCursor(3,0);  lcd.print("  ");}
    if (setAlarm <2 && setMode == 3){lcd.setCursor(6,0);  lcd.print("  ");}
    if (setAlarm <2 && setMode == 4){lcd.setCursor(1,1);  lcd.print("   ");}
    if (setAlarm <2 && setMode == 5){lcd.setCursor(5,1);  lcd.print("  ");}
    if (setAlarm <2 && setMode == 6){lcd.setCursor(8,1);  lcd.print("  ");}
    if (setAlarm <2 && setMode == 7){lcd.setCursor(11,1); lcd.print("    "); }

    //Set Timer
    if (setMode == 0 && setAlarm == 1 && mode==0){lcd.setCursor(2,1); lcd.print("  "); }
    if (setMode == 0 && setAlarm == 2 && mode==0){lcd.setCursor(6,1); lcd.print("  "); }
    if (setMode == 0 && setAlarm == 3 && mode==0){lcd.setCursor(10,1); lcd.print("  "); }
    if (setMode == 0 && setAlarm == 4 && mode==0){lcd.setCursor(13,1); lcd.print("  "); }

    if (setMode == 0 && setAlarm == 1 && mode>0){lcd.setCursor(11,0); lcd.print("  "); }
    if (setMode == 0 && setAlarm == 2 && mode>0){lcd.setCursor(14,0); lcd.print("  "); }
    if (setMode == 0 && setAlarm == 3 && mode>0){lcd.setCursor(11,1); lcd.print("  "); }
    if (setMode == 0 && setAlarm == 4 && mode>0){lcd.setCursor(14,1); lcd.print("  "); }
}

//Seting Jam ,Tanggal,Alarm/Timer
void setupClock (void) {
    
        if (setMode == 8){
            lcd.setCursor (0,0);
            lcd.print ("Set Time Finish ");
            lcd.setCursor (0,1);
            lcd.print ("Set Date Finish ");
            delay (1000);
            rtc.setTime (hh, mm, ss);
            rtc.setDate (dd, bb, yy);  
            lcd.clear();
            setMode = 0;
        }
        
    if (setAlarm >0){alarmMode=0;}
        
    if(digitalRead (bt_clock) == 0 && flag==0) {
        flag=1;
        if(setAlarm>0){WriteEeprom(); setAlarm=1; mode =5;}
        else{setMode = setMode+1;}
    }
    
    if(digitalRead (bt_timer) == 0 && flag==0){
        flag=1;
        if(setMode>0){setMode=8;}
        else{
            setAlarm = setAlarm+1;
            if(setAlarm>4){
                setAlarm=1; 
                WriteEeprom ();
                mode=mode+1;
                ReadEeprom();
            }
        } 
        lcd.clear();
    } 

    if(setAlarm == 1 && mode==5){
        lcd.setCursor (0,0);
        lcd.print ("Set Timer Finish");
        lcd.setCursor (0,1);
        lcd.print ("-EEPROM Updated-");
        delay (2000); 
        lcd.clear();
        setAlarm=0;
        mode =0;
        alarmMode=1;
    }

    if(digitalRead (bt_clock) == 1 && digitalRead (bt_timer) == 1){flag=0;}
    
    if(digitalRead (bt_up) == 0){                          
            if (setAlarm<2 && setMode==1)hh=hh+1; 
            if (setAlarm<2 && setMode==2)mm=mm+1;
            if (setAlarm<2 && setMode==3)ss=ss+1;
            if (setAlarm<2 && setMode==4)set_day=set_day+1;
            if (setAlarm<2 && setMode==5)dd=dd+1;
            if (setAlarm<2 && setMode==6)bb=bb+1;
            if (setAlarm<2 && setMode==7)yy=yy+1;
            //Timer
            if (mode==0 && setMode==0 && setAlarm==1)T1.timer=1;
            if (mode==0 && setMode==0 && setAlarm==2)T2.timer=1;
            if (mode==0 && setMode==0 && setAlarm==3)T3.timer=1;
            if (mode==0 && setMode==0 && setAlarm==4)T4.timer=1;
            
            if (mode>0 && setMode==0 && setAlarm==1)RS.StartHH=RS.StartHH+1;
            if (mode>0 && setMode==0 && setAlarm==2)RS.StartMM=RS.StartMM+1;
            if (mode>0 && setMode==0 && setAlarm==3)RS.FinishHH=RS.FinishHH+1;
            if (mode>0 && setMode==0 && setAlarm==4)RS.FinishMM=RS.FinishMM+1;

        if(hh>23)hh=0;
        if(mm>59)mm=0;
        if(ss>59)ss=0;
        if(set_day>7)set_day=0;
        if(dd>31)dd=0;
        if(bb>12)bb=0;
        if(yy>2030)yy=2000;
        if(RS.StartHH>23)RS.StartHH=0;
        if(RS.StartMM>59)RS.StartMM=0;
        if(RS.FinishHH>23)RS.FinishHH=0;
        if(RS.FinishMM>59)RS.FinishMM=0;
        rtc.setDOW(set_day);
    }        

    if(digitalRead (bt_down) == 0){                                      
            if (setAlarm<2 && setMode==1)hh=hh-1; 
            if (setAlarm<2 && setMode==2)mm=mm-1;
            if (setAlarm<2 && setMode==3)ss=ss-1;
            if (setAlarm<2 && setMode==4)set_day=set_day-1;
            if (setAlarm<2 && setMode==5)dd=dd-1;
            if (setAlarm<2 && setMode==6)bb=bb-1;
            if (setAlarm<2 && setMode==7)yy=yy-1;
            //Timer
            if (mode==0 && setMode==0 && setAlarm==1)T1.timer=0;
            if (mode==0 && setMode==0 && setAlarm==2)T2.timer=0;
            if (mode==0 && setMode==0 && setAlarm==3)T3.timer=0;
            if (mode==0 && setMode==0 && setAlarm==4)T4.timer=0;
            
            if (mode>0 && setMode==0 && setAlarm==1)RS.StartHH=RS.StartHH-1;
            if (mode>0 && setMode==0 && setAlarm==2)RS.StartMM=RS.StartMM-1;
            if (mode>0 && setMode==0 && setAlarm==3)RS.FinishHH=RS.FinishHH-1;
            if (mode>0 && setMode==0 && setAlarm==4)RS.FinishMM=RS.FinishMM-1;

        if(hh<0)hh=23;
        if(mm<0)mm=59;
        if(ss<0)ss=59;
        if(set_day<0)set_day=7;
        if(dd<0)dd=31;
        if(bb<0)bb=12;
        if(yy<0)yy=2030;
        if(RS.StartHH<0)RS.StartHH=23;
        if(RS.StartMM<0)RS.StartMM=59;
        if(RS.FinishHH<0)RS.FinishHH=23;
        if(RS.FinishMM<0)RS.FinishMM=59;
        rtc.setDOW(set_day);
    }
    
}

void setTimer (){
//Timer
    if (setMode == 0 && setAlarm >0 && mode>0){
        lcd.setCursor (0,0);
        lcd.print("Timer");
        lcd.print(mode);
        lcd.print(" On :");
        lcd.setCursor (11,0);
        lcd.print((RS.StartHH/10)%10);
        lcd.print(RS.StartHH % 10);
        lcd.print(":");
        lcd.print((RS.StartMM/10)%10);
        lcd.print(RS.StartMM % 10);

        lcd.setCursor (0,1);
        lcd.print("Timer");
        lcd.print(mode);
        lcd.print(" Off:");
        lcd.setCursor (11,1);
        lcd.print((RS.FinishHH/10)%10);
        lcd.print(RS.FinishHH % 10);
        lcd.print(":");
        lcd.print((RS.FinishMM/10)%10);
        lcd.print(RS.FinishMM % 10);
    }

    if (setMode == 0 && setAlarm >0 && mode==0){
        lcd.setCursor (0,0);
        lcd.print(" T1  T2  T3  T4 ");
        lcd.setCursor (0,1);
            if(T1.timer==1){lcd.print("  A");}
        else{lcd.print("  D");}
        
            if(T2.timer==1){lcd.print("   A");}
        else{lcd.print("   D");}
        
            if(T3.timer==1){lcd.print("   A");}
        else{lcd.print("   D");}
        
            if(T4.timer==1){lcd.print("   A");}
        else{lcd.print("   D");}
    }
}

void ReadEeprom() {
    T1.StartHH=EEPROM.read(11);T1.StartMM=EEPROM.read(12);T1.FinishHH=EEPROM.read(13);T1.FinishMM=EEPROM.read(14);
    T2.StartHH=EEPROM.read(21);T2.StartMM=EEPROM.read(22);T2.FinishHH=EEPROM.read(23);T2.FinishMM=EEPROM.read(24);
    T3.StartHH=EEPROM.read(31);T3.StartMM=EEPROM.read(32);T3.FinishHH=EEPROM.read(33);T3.FinishMM=EEPROM.read(34);
    T4.StartHH=EEPROM.read(41);T4.StartMM=EEPROM.read(42);T4.FinishHH=EEPROM.read(43);T4.FinishMM=EEPROM.read(44);

    if(mode==1){RS.StartHH=T1.StartHH, RS.StartMM=T1.StartMM, RS.FinishHH=T1.FinishHH,RS.FinishMM=T1.FinishMM;}
    if(mode==2){RS.StartHH=T2.StartHH, RS.StartMM=T2.StartMM, RS.FinishHH=T2.FinishHH,RS.FinishMM=T2.FinishMM;}
    if(mode==3){RS.StartHH=T3.StartHH, RS.StartMM=T3.StartMM, RS.FinishHH=T3.FinishHH,RS.FinishMM=T3.FinishMM;}
    if(mode==4){RS.StartHH=T4.StartHH, RS.StartMM=T4.StartMM, RS.FinishHH=T4.FinishHH,RS.FinishMM=T4.FinishMM;}

    T1.timer=EEPROM.read(1);
    T2.timer=EEPROM.read(2);
    T3.timer=EEPROM.read(3);
    T4.timer=EEPROM.read(4);
}

void WriteEeprom() {
    if(mode==1){EEPROM.write(11,RS.StartHH);EEPROM.write(12,RS.StartMM);EEPROM.write(13,RS.FinishHH);EEPROM.write(14,RS.FinishMM);}
    if(mode==2){EEPROM.write(21,RS.StartHH);EEPROM.write(22,RS.StartMM);EEPROM.write(23,RS.FinishHH);EEPROM.write(24,RS.FinishMM);}
    if(mode==3){EEPROM.write(31,RS.StartHH);EEPROM.write(32,RS.StartMM);EEPROM.write(33,RS.FinishHH);EEPROM.write(34,RS.FinishMM);}
    if(mode==4){EEPROM.write(41,RS.StartHH);EEPROM.write(42,RS.StartMM);EEPROM.write(43,RS.FinishHH);EEPROM.write(44,RS.FinishMM);}

    EEPROM.write(1,T1.timer);
    EEPROM.write(2,T2.timer);
    EEPROM.write(3,T3.timer);
    EEPROM.write(4,T4.timer);
}
