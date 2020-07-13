#include <Ethernet.h> 
#include <EthernetUdp.h> 
#include <SPI.h> 
#include <TimerOne.h>

#define IN1  3
#define IN2  4
#define IN3  5
#define IN4  6
#define dioda 10

int Steps = 4096; 
int cstep = 0;
bool zastavica = false;
bool poslano = false;
int ldrPin = 9;
int c;
int d;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; 
IPAddress ip(192, 168, 1, 11); 
unsigned int localPort = 5000; 
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
String datReq; 
int packetSize; 
EthernetUDP Udp; 

void setup() {
  Timer1.initialize(500000);
  Timer1.attachInterrupt(procitajSenzor);
  pinMode(ldrPin, INPUT);
  pinMode(dioda, OUTPUT); 
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT); 
  
  Serial.begin(9600); 
  Ethernet.begin(mac, ip); 
  Udp.begin(localPort); 
  delay(150); 
}

void loop() {

  digitalWrite(dioda,HIGH);
  
  packetSize = Udp.parsePacket(); 
  
  if(packetSize>0){ 
    
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    String datReq(packetBuffer); 
  
    if (datReq == "Pokreni") { 
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());  
      String pokreni = "p";
      Udp.print(pokreni);
      Udp.endPacket();   
      zastavica = true;
    }
    
     if (datReq == "Zaustavi") { 
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort()); 
      String zaustavi = "z";
      Udp.print(zaustavi);
      Udp.endPacket(); 
      zastavica = false;
     }
  }
  
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
  
  if(zastavica == true){
      for(int x=0;x<Steps;x++)
      {
        step1();
        delay(1);
      }
      
      noInterrupts();
      procitajSenzor();
      interrupts();
  }
  
  if(zastavica == false){
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, LOW);
    }  
}

void procitajSenzor(){
  
   c=digitalRead(ldrPin);
   Serial.println(c);
   if(c==1){
      d++;
    }
    if(c==0){
      d=0;
      poslano = false;
    }
    if(poslano == false && zastavica == true){
      if(d>18){
         Udp.beginPacket(Udp.remoteIP(), Udp.remotePort()); 
          String kvar = "k";
          Udp.print(kvar);
          Udp.endPacket(); 
          poslano = true;
      }
    }
}

void step1()
{
  switch(cstep)
  {
   case 0:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
   break; 
   case 1:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, HIGH);
   break; 
   case 2:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
   break; 
   case 3:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
   break; 
   case 4:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
   case 5:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
     case 6:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
   case 7:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
   break; 
   default:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
  }
   cstep=cstep+1;
   if(cstep==8)
     {cstep=0;}
}
