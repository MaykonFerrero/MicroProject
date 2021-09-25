/*
  TTL TESTADOR DE CI

 Use para testar e identificar CIs
 
 o botao esquero é para dar start
 O botão direito é para selecionar

Equipe:
Adriely
Maykon
Nayra
Thaiane

Matéria: Microcontroladores
Professor: Walfredo Lucena
 */

#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

#define ledGreen A1
#define ledRed A0


const int selecionar = 6;
const int start  = 5;
int selecionarState = 0;
int prevSelectState = 0;
int startState = 0;
int prevStartState = 0;

const int pins[6] = {4, 3, 2, A2, A3, A4}; // PINOS DE TESTE

//variável usada para seleção
int chipSelect = 0;

const int highCHIP = 6;//número máximo de opçoes de seleção

//Nomes do CIs
unsigned char state[] = {LOW, HIGH};


struct CHIPS {
  const char* name;//nome do CI
  int pinOUT[6]; // indica quem é entrada e quem é saída 0 para saída e 1 para entrada
  int cOutput[4]; // tabala verdade das portas 

};

CHIPS chip[7] = {
  {"74HC00", {1, 1, 0, 1, 1, 1}, {1, 1, 1, 0}}, //NAND
  {"74HC02", {0, 1, 1, 1, 1, 1}, {1, 0, 0, 0}}, //NOR
  {"74HC04", {1, 0, 1, 1, 1, 1}, {1, 1, 0, 0}}, //NOT
  {"74HC08", {1, 1, 0, 1, 1, 1}, {0, 0, 0, 1}}, //AND
  {"74HC32", {1, 1, 0, 1, 1, 1}, {0, 1, 1, 1}}, //OR
  {"74HC86", {1, 1, 0, 1, 1, 1}, {0, 1, 1, 0}}, //XOR
  {"74HC74", {1, 1, 1, 1, 0, 0}, {0, 0,1 , 1}}, //FF D  
};


void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Qual CI?");
  lcd.setCursor(0, 1);
  lcd.print(chip[chipSelect].name);
  
  lcd.setCursor(0, 1);
  pinMode(selecionar, INPUT);
  pinMode(start, INPUT);
  for (int i = 0; i < 6; i++) {
    pinMode(pins[i], INPUT);
  }
    pinMode(ledRed, OUTPUT);
    pinMode(ledGreen,OUTPUT);
  
    Serial.begin(9600);
  
}

void loop() {
  digitalWrite(ledGreen,1);// ACIONA O LED VERDE INDICANDO QUE O TESTADOR ESTÁ PRONTO PARA TESTE
  digitalWrite(ledRed,0);
  
  selecionarState = digitalRead(selecionar);
  startState = digitalRead(start);

  if (selecionarState != prevSelectState)
  {
    if (selecionarState == LOW)
    {
      chipSelect++;
      if (chipSelect > highCHIP)
      {
        chipSelect = 0;
      }
      lcd.setCursor(0, 1);
      lcd.print(chip[chipSelect].name);

    }
  }
  if (startState != prevStartState)
  {
    if (startState == LOW)
    {
      lcd.clear();
      lcd.print("Check: ");
      lcd.print(chip[chipSelect].name);
      
      
      checkIC();//CHAMA A FUNÇÃO QUE TESTA O CI

    }
  }
  prevSelectState = selecionarState;
  prevStartState = startState;
}

void checkIC()
{

  digitalWrite(ledGreen,0);
  digitalWrite(ledRed,1);//ACIONA O LED VERDE INDICANDO QUE O TESTADOR ESTÁ EM USO

  
  int count = 0;
  int val = 0;
  bool quality = true;
  int out[2] = {};
  int in = 0;

  for (int i = 0; i < 6; i++) {                     
    if (chip[chipSelect].pinOUT[i] == 0) {
      pinMode(chip[chipSelect].pinOUT[i], INPUT);
      in = i;
    } else {
      pinMode(chip[chipSelect].pinOUT[i], OUTPUT);
      out[count] = i;
      count++;
    }
  }
  count = 0;

   lcd.setCursor(0, 1);

   if(chipSelect ==6){

     
    Serial.println(chipSelect);
    digitalWrite(pins[0], HIGH);
   
    digitalWrite(pins[3], HIGH);
    
  lcd.print('.');  
  digitalWrite(pins[2], HIGH);
  val =digitalRead(pins[4]);
  Serial.println(val);
  delay(1000);
  if ( val != chip[chipSelect].cOutput[count]) {
        quality = false;
      }
      
      
      count++;
  lcd.print('.');
  digitalWrite(pins[2], LOW);
  val =digitalRead(pins[4]);
  Serial.println(val);
  delay(1000);
   if ( val != chip[chipSelect].cOutput[count]) {
        quality = false;
      }
     
      
      count++; 
  lcd.print('.');
  digitalWrite(pins[2], HIGH);
  val =digitalRead(pins[4]);
  Serial.println(val);
  delay(1000);
   if ( val != chip[chipSelect].cOutput[count]) {
        quality = false;
      }
    
      
      count++; 
  lcd.print('.');
  digitalWrite(pins[2], LOW);
  val =digitalRead(pins[4]);
  Serial.println(val);
  delay(1000);
   if ( val != chip[chipSelect].cOutput[count]) {
        quality = false;
      }
     
     
      count++; 
  lcd.print('.');
    
  }

  
  
 else if(chipSelect!=6){
  
  // 00, 01, 10, 11
  for (int m = 0; m < 2; m++)
  {
    for (int n = 0; n < 2; n++)
    {
      lcd.print('.');
      
      delay(100);
      digitalWrite(pins[out[0]], state[m]);
      digitalWrite(pins[out[1]], state[n]); 
      val = digitalRead(pins[in]);
    
      if ( val != chip[chipSelect].cOutput[count]) {
        quality = false;
      }
      //se não igual
      if (!quality) break;
      count++;
    }
  }
}

  lcd.clear();
  if (quality)
  {
    lcd.print("Passou no teste");
  } else if(!quality){
    lcd.print("Falhou no teste");
  }
  lcd.setCursor(0, 1);
  lcd.print("Clique em prox");
  while (digitalRead(selecionar) != HIGH) {};
  delay(200);
  lcd.clear();
  lcd.print("Proximo CI?");
  for (int i = 0; i < 3; i++) {
    pinMode(pins[i], INPUT);
}

}

