//Carrega a biblioteca LiquidCrystal
#include <LiquidCrystal.h>

#define RS (2)
#define EN (3)
#define D4 (4)
#define D5 (5)
#define D6 (6)
#define D7 (7)

#define LIMIT_16 (16)
#define LIMIT_32 (32)
#define SHOW_  ('\\')
 
//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
 
void setup()
{
  Serial.begin(9600);
  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);

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
  static String texto = "";
  static boolean show_lcd = false;

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
      lcd.print(texto.substring(0, 16));
    } else {
      lcd.setCursor(0, 0);
      lcd.print(texto.substring( 0, 16));
      lcd.setCursor(0, 1);
      lcd.print(texto.substring(16, 32));
    }
    texto = "";
  }

}
