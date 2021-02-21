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

#define LED_1 (A2)
#define LED_2 (A3)
#define LED_3 (A4)
#define LED_4 (A5)

#define PWM_PINO (9)

#define DHTPIN A1
#define DHTTYPE DHT11 // DHT 11

#define LIMIT_16 (16)
#define LIMIT_32 (32)
#define SHOW_  ('\\')

#define FUNCIONALIDADE_1 ("01")
#define FUNCIONALIDADE_2 ("02")
#define FUNCIONALIDADE_3 ("03")

//Tempo de intervalo
#define TEMPO   (10)
 
//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
DHT dht(DHTPIN, DHTTYPE);


// Variaveis globais
String texto = "";
String texto_receive = "";
boolean abilit_func = false;
int pwm_valor = 0;


static void texto_lcd( )
{
  lcd.clear( );

  if( texto.length( ) < LIMIT_16 )
  {
    lcd.setCursor(0, 0);
    lcd.print(texto.substring( 0, 16 ));
  } else {
    lcd.setCursor(0, 0);
    lcd.print(texto.substring( 0, 16 ));
    lcd.setCursor(0, 1);
    lcd.print(texto.substring( 16, 32 ));
  }

}

static void leds_on_off( )
{
  int i = int(texto.charAt(0));
  if( i == 48 || i == 49 ) digitalWrite(LED_1, i - 48 );

  i = int(texto.charAt(1));
  if( i == 48 || i == 49 ) digitalWrite(LED_2, i - 48 );

  i = int(texto.charAt(2));
  if( i == 48 || i == 49 ) digitalWrite(LED_3, i - 48 );

  i = int(texto.charAt(3));
  if( i == 48 || i == 49 ) digitalWrite(LED_4, i - 48 );

}

static void pwm_controle( )
{
  String numero_pwm_str = texto.substring( 0, 3 );
  pwm_valor = numero_pwm_str.toInt( );
}

void setup()
{
  Serial.begin(9600);
  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);
  dht.begin();

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);

  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, HIGH);
  digitalWrite(LED_3, HIGH);
  digitalWrite(LED_4, HIGH);

  lcd.setCursor(0, 0);
  lcd.print(" ARDUINO  ESTA ");
  lcd.setCursor(0, 1);
  lcd.print("       OK      ");
  delay(3000);
  lcd.clear( );

  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  digitalWrite(LED_3, LOW);
  digitalWrite(LED_4, LOW);

}
 
void loop()
{
  // A leitura da temperatura e umidade pode levar 250ms!
  // O atraso do sensor pode chegar a 2 segundos.
  int umidade = dht.readHumidity();
  int temperatura = dht.readTemperature();

  analogWrite( PWM_PINO, pwm_valor );

  if (isnan(temperatura) || isnan(umidade)) 
  {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("-humidit-");
    Serial.print(umidade);
    Serial.print("-temper-");
    Serial.print(temperatura);
    Serial.print("-");
  }

  delay(TEMPO);

}

void serialEvent( )
{
  if(Serial.available( ))
  {
    char caracter = Serial.read( );
    if( caracter == SHOW_ ) abilit_func = true;
    else texto_receive += caracter;
  }

  if( abilit_func )
  {
    abilit_func = false;

    String funcionalidade = "";

    if( texto_receive.charAt( 0 ) == '0')
    {
      funcionalidade.concat(texto_receive.charAt( 0 ));
      funcionalidade.concat(texto_receive.charAt( 1 ));

      for( int i = 2; i < texto_receive.length( ); i++ )
      {
        texto.concat(texto_receive.charAt( i ));
      }
    } else {
      funcionalidade.concat(texto_receive.charAt( 1 ));
      funcionalidade.concat(texto_receive.charAt( 2 ));

      for( int i = 3; i < texto_receive.length( ); i++ )
      {
        texto.concat(texto_receive.charAt( i ));
      }
    }

    texto_receive = "";

    if( funcionalidade == FUNCIONALIDADE_1 ) texto_lcd( );
    if( funcionalidade == FUNCIONALIDADE_2 ) leds_on_off( );
    if( funcionalidade == FUNCIONALIDADE_3 ) pwm_controle( );

    texto = "";
  }
}
