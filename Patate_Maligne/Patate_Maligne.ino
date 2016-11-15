//****************************************************************************************
// Illutron take on Disney style capacitive touch sensor using only passives and Arduino
// Dzl 2012
//****************************************************************************************


//                              10n
// PIN 9 --[10k]-+-----10mH---+--||-- OBJECT
//               |            |
//              3.3k          |
//               |            V 1N4148 diode
//              GND           |
//                            |
//Analog 0 ---+------+--------+
//            |      |
//          100pf   1MOmhm
//            |      |
//           GND    GND



#define SET(x,y) (x |=(1<<y))				//-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))       		// |
#define CHK(x,y) (x & (1<<y))           		// |
#define TOG(x,y) (x^=(1<<y))            		//-+



#define N 160  //How many frequencies

float results[N];            //-Filtered result buffer
float freq[N];            //-Filtered result buffer
int sizeOfArray = N;

//Définition des ports de sortie des couleurs led

const int R=10; 
const int V=11; 
const int B=12;
   
   

void setup()
{
  
  
  TCCR1A=0b10000010;        //-Set up frequency generator
  TCCR1B=0b00011001;        //-+
  ICR1=110;
  OCR1A=55;

// Initialisation LED


// --- Déclaration des constantes des broches E/S numériques ---

const int ledRouge =10; // Constante pour la broche 3
const int ledVert =11; // Constante pour la broche 5
const int ledBleu =12; // Constante pour la broche 6

pinMode(ledRouge, OUTPUT);
pinMode(ledVert, OUTPUT);
pinMode(ledBleu, OUTPUT);

  pinMode(9,OUTPUT);        //-Signal generator pin
  pinMode(8,OUTPUT);        //-Sync (test) pin

  Serial.begin(115200);

  for(int i=0;i<N;i++)      //-Preset results
    results[i]=0;         //-+

}

void loop()
{
  unsigned int d;
  
  int counter = 0;
  for(unsigned int d=0;d<N;d++)
  {
    
    if(results[43] > results[83]){
      digitalWrite(B, 0);
      digitalWrite(R, 0);
      digitalWrite(V, 255);

     }else if(results[83] > results[43] && results[83] > results[140]){
      digitalWrite(V, 0);
      digitalWrite(R, 0);
      digitalWrite(B, 255);
     }else if(results[140] > results[43] && results[140] > results[83]){
      digitalWrite(B, 0);
      digitalWrite(V, 0);
      digitalWrite(R, 255);
     }
    
    int v=analogRead(0);    //-Read response signal
    CLR(TCCR1B,0);          //-Stop generator
    TCNT1=0;                //-Reload new frequency
    ICR1=d;                 // |
    OCR1A=d/2;              //-+
    SET(TCCR1B,0);          //-Restart generator

    results[d]=results[d]*0.5+(float)(v)*0.5; //Filter results
   
    freq[d] = d;

 //   plot(v,0);              //-Display
 //   plot(results[d],1);
  // delayMicroseconds(1);
  }


PlottArray(1,freq,results); 
 

  TOG(PORTB,0);            //-Toggle pin 8 after each sweep (good for scope)
}
   
