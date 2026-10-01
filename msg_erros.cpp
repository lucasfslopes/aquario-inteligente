#include "msg_erros.h"

int msgErro(int seletor){

    display.fillRect(0, 0, 128, 15, SH110X_WHITE);
    
    switch(seletor){
        case 2:            
            display.setTextSize(1); // Normal 1:1 pixel scale
            display.setTextColor(SH110X_BLACK);
            display.setCursor(0, 3);
            display.print("Temperatura Alta!");  
        break;
        case 3:
            display.setTextSize(1); // Normal 1:1 pixel scale
            display.setTextColor(SH110X_BLACK);
            display.setCursor(0, 3);
            display.print("Temperatura baixa!");  
        break;
    }
    display.display();
    return 0;
}