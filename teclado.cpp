#include "teclado.h"

char teclado() {
  
    /* Keypad button analog Value
    no button pressed 600
    menu      103
    voltar    480
    baixo     267
    cima       7
    */
    
    int val = analogRead(botao);

    if(val < 50){
        return 'C';
        }
    else if(val < 200){
        return 'M';
        }
    else if(val < 300){
        return 'B';
        }
    else if(val < 500){
        return 'V';
        }
    else{
        return 'N';
        }

    delay(10);
}