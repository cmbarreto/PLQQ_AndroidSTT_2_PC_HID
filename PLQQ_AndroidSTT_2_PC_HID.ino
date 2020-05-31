#include<SoftwareSerial.h>
#include <Keyboard.h>

//SoftwareSerial RC(1, 0);// bluetooth TX,RX
char RCD;
String printed;
String word1 = "";

//
// Control keys
//
bool CTRL_Requested = false;
bool ALT_Requested = false;
bool WIN_Requested = false;
bool SHIFT_Requested = false;
bool KEY_Requested = false;


void setup() {
  // put your setup code here, to run once:
  //RC
  Serial1.begin(9600);
  Serial.begin(9600);
  Keyboard.begin();
  pinMode(4,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available() > 0) {
      RCD = Serial1.read();//recieve value from bluetooth
//    Keyboard.print(RCD);//print char as recieved byte by byte
      if( ReadWord(RCD)) //armo palabras
      {   
         WriteString1(word1);
         word1 = "";
      }
    printed +=RCD;
  }
  if(digitalRead(4)==0){delay(1000);Serial.println(printed);}
}
/*if you want to record the words that been printed.. you should use string
 */

//
// Read Words.
// Lee una palabra completa separada por espacions antes de procesarla
// esto es para poder procesar comandos como control alt shift etc,,
//
bool ReadWord( char Char2Write)
{
  if( Char2Write == ' ' ) {
    return true;
  }
  else {
    word1 += Char2Write;
    return false;
  }
}

void WriteString1(String word2print) 
{
  if( SubstituteShift(word2print) ) return;
  if( SubstituteCtrl(word2print) ) return;
  if( SubstituteAlt(word2print) ) return;
  if( SubstituteWin(word2print) ) return;
  //if( SubstituteShift(word2print) ) return;
  if( SubstituteKey(word2print) ) return;
  if( CTRL_Requested) Keyboard.press( KEY_RIGHT_CTRL);
  if( ALT_Requested) Keyboard.press( KEY_RIGHT_ALT);
  if( WIN_Requested) Keyboard.press( KEY_LEFT_GUI);
  if( SHIFT_Requested) Keyboard.press( KEY_RIGHT_SHIFT);
  if( KEY_Requested) word2print = ReplaceKey( word2print);
  //Carateres especiales Spanish
  word2print.replace("T", "[");  //reemplazo tildes
  word2print.replace("N", ";");  //reemplazo enies
  Keyboard.print( word2print);  
  if( !KEY_Requested) Keyboard.print(' ');
  if( SHIFT_Requested) Keyboard.release( KEY_RIGHT_SHIFT);
  if( WIN_Requested) Keyboard.release( KEY_LEFT_GUI);
  if( ALT_Requested) Keyboard.release( KEY_RIGHT_ALT);
  if( CTRL_Requested) Keyboard.release( KEY_RIGHT_CTRL);
  CTRL_Requested = false;
  ALT_Requested = false;
  WIN_Requested = false;
  SHIFT_Requested = false;
  KEY_Requested = false;

}

//
// Sets Shift key
//
bool SubstituteShift( String code)
{
  code.trim();
  if( String("shift") != code) return false;
  SHIFT_Requested = true;
  return true;
}

//
// Sets Control key
//
bool SubstituteCtrl( String code)
{
  code.trim();
  if( String("control") != code) return false;
  CTRL_Requested = true;
  return true;
}

//
// Sets Alt key
//
bool SubstituteAlt( String code)
{
  code.trim();
  if( String("alt") != code) return false;
  //if( code.indexOf("alt", 0) == -1) return false;
  ALT_Requested = true;
  return true;
}

//
// Sets Win key
//
bool SubstituteWin( String code)
{
  code.trim();
  if( String("win") != code) return false;
  WIN_Requested = true;
  return true;
}


//
// Sets Special key
//
bool SubstituteKey( String code)
{
  code.trim();
  if( String("tecla") != code) return false;
  KEY_Requested = true;
  return true;
}

//
// Replace received Special key
//
String  ReplaceKey( String code)
{
  code.trim();
  //Caracteres de conrol
  code.replace("derecha", String((char)KEY_RIGHT_ARROW));  //reemplazo right
  code.replace("fin", String((char)KEY_END));  //reemplazo end
  code.replace("backspace", String((char)KEY_BACKSPACE));  //reemplazo bkspc
  code.replace("izquierda", String((char)KEY_LEFT_ARROW));  //reemplazo left
  code.replace("inicio", String((char)KEY_HOME));  //reemplazo home
  code.replace("arriba", String((char)KEY_UP_ARROW));  //reemplazo up
  code.replace("up", String((char)KEY_PAGE_UP));  //reemplazo page up
  code.replace("down", String((char)KEY_PAGE_DOWN));  //reemplazo page down
  code.replace("abajo", String((char)KEY_DOWN_ARROW));  //reemplazo down
  code.replace("escape", String((char)KEY_ESC));  //reemplazo esc
//  code.replace("tvl", String((char)KEY_RIGHT_CTRL));  //reemplazo NADA its modifier
//  code.replace("tvl", String((char)KEY_RIGHT_ALT));  //reemplazo  NADA its modifier
  code.replace("borrar", String((char)KEY_DELETE));  //reemplazo delete 
  code.replace("insertar", String((char)KEY_INSERT));  //reemplazo insert
  code.replace("tabulador", String((char)KEY_TAB));  //reemplazo tabulador
  code.replace("enter", String((char)KEY_RETURN)); //+ String((char)KEY_BACKSPACE));  //reemplazo enter
//  code.replace("tvl", String((char)KEY_RIGHT_SHIFT));  //reemplazo  NADA its modifier
  //Simbolos con el teclado Spanish
  code.replace("parentesis", ")");  //reemplazo left parentesis
  code.replace("parentesis", "(");  //reemplazo rigth parentesis
  code.replace("techo", "^");  //reemplazo 
  code.replace("corchete", "[");  //reemplazo left corch
  code.replace("corchete", "]");  //reemplazo  right corch
  code.replace("arroba", "@");  //reemplazo arroba
  code.replace("igual", "=");  //reemplazo equal
  code.replace("backslash", "\\");  //reemplazo backslash
  //OJOO FALTAcode.replace("lprt", "`");  //reemplazo 
  code.replace("menos", "-");  //reemplazo dash
  code.replace("comillas", "\"");  //reemplazo comillas
  code.replace("coma", ",");  //reemplazo  coma
  code.replace("punto", ".");  //reemplazo point
  code.replace("pregunta", "?");  //reemplazo rigth ask
  code.replace("dividido", "/");  //reemplazo divide
  code.replace("ampersand", "&");  //reemplazo ampersand
  code.replace("llave", "{");  //reemplazo left key
  code.replace("llave", "}");  //reemplazo right key
  code.replace("sharp", "#");  //reemplazo sharp
  code.replace("mas", "+");  //reemplazo plus
  code.replace("pipe", "|");  //reemplazo pipe
  code.replace("nioqui", "~");  //reemplazo nioqui
  code.replace("infraguion", "_");  //reemplazo sub guion
  //code.replace("lprt", "\'");  //reemplazo tilde
  code.replace("pc", ";");  //reemplazo punto y coma
  code.replace("dp", ":");  //reemplazo dos puntos
  code.replace("admiracion", "!");  //reemplazo right admiracion
  code.replace("porcentaje", "%");  //reemplazo percent
  code.replace("menor", "<");  //reemplazo minor
  code.replace("asterisco", "*");  //reemplazo aster
  code.replace("mayor", ">");  //reemplazo major
  code.replace("espacio", " ");  //reemplazo espacio
  code.replace("pesos", "$");  //reemplazo pesos
  return code;
}


