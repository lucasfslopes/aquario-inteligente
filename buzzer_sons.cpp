#include "buzzer_sons.h"

int buzzerErro(int seletor){

    switch(seletor){
        case 1:
            tone(buzzer, 2000);
            delay(50);
            noTone(buzzer);
            delay(100);
            tone(buzzer, 2000);
            delay(50);
            noTone(buzzer);
        break;
        case 2:
            tone(buzzer, 2000);
            delay(1000);
            noTone(buzzer);
            delay(500);
            tone(buzzer, 2000);
            delay(1000);
            noTone(buzzer); 
        break;
    }
    return 0;
}