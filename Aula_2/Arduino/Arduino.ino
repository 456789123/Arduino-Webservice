//Carrega a biblioteca LiquidCrystal
#include <LiquidCrystal.h>
//Carrega a biblioteca DHT11
#include "DHT.h"

#define RS (2)
#define EN (3)
#define D4 (4)
#define D5 (5)
#define D6 (6)
#define D7 (7)

#define DHTPIN A1
#define DHTTYPE DHT11 // DHT 11

#define LIMIT_16 (16)
#define LIMIT_32 (32)
#define SHOW_  ('\\')

//Tempo de intervalo
#define TEMPO   (100)
 
//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
DHT dht(DHTPIN, DHTTYPE);


// Variaveis globais
String texto = "";
boolean show_lcd = false;

void setup()
{
  Serial.begin(9600);
  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);
  dht.begin();

  // Teste para o início do Arduino
  lcd.setCursor(0, 0);
  lcd.print(" ARDUINO  ESTA ");
  lcd.setCursor(0, 1);
  lcd.print("       OK      ");
  delay(3000);
  lcd.clear( );
}
 
void loop()
{

  // A leitura da temperatura e umidade pode levar 250ms!
  // O atraso do sensor pode chegar a 2 segundos.
  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();

  if (isnan(temperatura) || isnan(umidade)) 
  {
    Serial.println("Failed to read from DHT");
  } 
  else
  {
    Serial.print("-humidit-");
    Serial.print(umidade);
    Serial.print("-temper-");
    Serial.println(temperatura);
  }

  delay(TEMPO);

}

void serialEvent( )
{
  if(Serial.available( ))
  {
    char caracter = Serial.read( );
    if( caracter == SHOW_ ) show_lcd = true; else
    if( LIMIT_32 > texto.length( ))
    {
      texto += caracter;
    } else texto = "";
  }

  if( show_lcd )
  {
    lcd.clear( );
    show_lcd = false;
    if( texto.length( ) < LIMIT_16 )
    {
      lcd.setCursor(0, 0);
      lcd.print(texto.substring(0, 15));
      texto = "";
    } else {
      lcd.setCursor(0, 0);
      lcd.print(texto.substring( 0, 15));
      lcd.setCursor(0, 1);
      lcd.print(texto.substring(15, 30));
      texto = "";
    }
  }
}
