#include <LiquidCrystal.h>

#include <IRremote.h>


LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

unsigned long com = 0;
int RECV_PIN = 13;
IRrecv irrecv(RECV_PIN);
decode_results results;
int colonne = 0;
int ligne = 0;
int ON = 1;
int BLINK = 1;
int AUTO = 1;



void setup()
{

  irrecv.enableIRIn(); // Start the receiver
  lcd.begin(16, 2);
  lcd.print("Hello in to LCD");
  lcd.setCursor(0, 1);
  lcd.print("VERSION 0.9");
  Serial.begin(9600);

}

void loop(){
  if (irrecv.decode(&results)) {
    com = results.value;
    Serial.println(com);
    irrecv.resume(); // Receive the next value
  } 
  else {

        
    lcd.setCursor(colonne, ligne);

    if(colonne > 15){
      colonne = 0;
      ligne = ligne + 1;
    }
    if(colonne < 0){
      colonne = 0;
    }
    if(ligne > 1){
      ligne = 0;
    }
    if(ligne < 0){
      ligne = 1;
    }
    if(com == 16753245){
      if (ON == 0){
        lcd.display();
        ON = 1;
      } 
      else {
        lcd.noDisplay();
        ON = 0;
      }
    }

    if(com == 16720605){
      ligne = 0;
      colonne = 0;
    }


    if(com == 16750695){
      if (BLINK == 0){
        lcd.blink();
        BLINK = 1;
      } 
      else {
        lcd.noBlink();
        BLINK = 0;
      }
    }

    if(com == 16769565){
      lcd.clear();
      ligne = 0;
      colonne = 0;
    }

    if(com == 16712445){
      colonne = colonne - 1;
    }

    if(com == 16761405){
      colonne = colonne + 1;
    }

    if(com == 16769055){
      ligne = ligne + 1;
    }

    if(com == 16736925){
      if (AUTO == 0){
        AUTO = 1;
      } 
      else {
        AUTO = 0;
      }
    }


    if(com == 16738455){
      lcd.print("0");
      if(AUTO == 1){
        colonne = colonne + 1;
      }
    }


    if(com == 16724175){
      lcd.print("1");
      if(AUTO == 1){
        colonne = colonne + 1;
      }
    }


    if(com == 16718055){
      lcd.print("2");
      if(AUTO == 1){
        colonne = colonne + 1;
      }
    }


    if(com == 16743045){
      lcd.print("3");
      if(AUTO == 1){
        colonne = colonne + 1;
      }
    }


    if(com == 16716015){
      lcd.print("4");
      if(AUTO == 1){
        colonne = colonne + 1;
      }
    }


    if(com == 16726215){
      lcd.print("5");
      if(AUTO == 1){
        colonne = colonne + 1;
      }
    }


    if(com == 16734885){
      lcd.print("6");
      if(AUTO == 1){
        colonne = colonne + 1;
      }
    }


    if(com == 16728765){
      lcd.print("7");
      if(AUTO == 1){
        colonne = colonne + 1;
      }
    }


    if(com == 16730805){
      lcd.print("8");
      if(AUTO == 1){
        colonne = colonne + 1;
      }
    }


    if(com == 16732845){
      lcd.print("9");
      if(AUTO == 1){
        colonne = colonne + 1;
      }
    }

    if(com == 16754775){
      lcd.print(" ");
      if(AUTO == 1){
        colonne = colonne - 1;
      }
    }

    if(com == 16748655){
      lcd.print(" ");
      if(AUTO == 1){
        colonne = colonne + 1;
      }
    }




    com = 0;



  }
}





