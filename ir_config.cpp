#include "ir_config.h"
#include <IRrecv.h>

//Instancia Receptor Infravermelho

    IRrecv receptor(irSensorPin);                 // cria o objeto receptor
    decode_results resultado;                     // declara a variável resultado

//Botoes controle IR

    #define tk1 0xFFA25D
    #define tk2 0xFF629D
    #define tk3 0xFFE21D
    #define tk4 0xFF22DD
    #define tk5 0xFF02FD
    #define tk6 0xFFC23D
    #define tk7 0xFFE01F
    #define tk8 0xFFA857
    #define tk9 0xFF906F
    #define tk_ast 0xFF6897
    #define tk0 0xFF9867
    #define tk_hash 0xFFB04F
    #define tk_up 0xFF18E7
    #define tk_left 0xFF10EF
    #define tk_ok 0xFF38C7
    #define tk_right 0xFF5AA5
    #define tk_down 0xFF4AB5

char controleIR() {

    if (receptor.decode(&resultado))   {      // se exite algum código recebido
    
        switch (resultado.value) {
            case (tk1):
            Serial.println("Tecla 1");
            receptor.resume();
            return '1';
            break;
            case (tk2): 
            Serial.println("Tecla 2");
            receptor.resume();
            return '2';
            break; 
            case (tk3): 
            Serial.println("Tecla 3");
            receptor.resume();
            return '3';
            break;
            case (tk4): 
            Serial.println("Tecla 4");
            receptor.resume();
            return '4';
            break;
            case (tk5): 
            Serial.println("Tecla 5");
            receptor.resume();
            return '5';
            break;
            case (tk6): 
            Serial.println("Tecla 6");
            receptor.resume();
            return '6';
            break; 
            case (tk7): 
            Serial.println("Tecla 7");
            receptor.resume();
            return '7';
            break;
            case (tk8): 
            Serial.println("Tecla 8");
            receptor.resume();
            return '8';
            break;
            case (tk9): 
            Serial.println("Tecla 9");
            receptor.resume();
            return '9';
            break;
            case (tk0): 
            Serial.println("Tecla 0");
            receptor.resume();
            return '0';
            break; 
            case (tk_ast): 
            Serial.println("Tecla *");
            receptor.resume();
            return '*';
            break;
            case (tk_hash): 
            Serial.println("Tecla #");
            receptor.resume();
            return '#';
            break;
            case (tk_up): 
            Serial.println("Cima");
            receptor.resume();
            return 'C';
            break;
            case (tk_left): 
            Serial.println("Esquerda");
            receptor.resume();
            return 'V';
            break; 
            case (tk_right): 
            Serial.println("Direita");
            receptor.resume();
            return 'M';
            break;
            case (tk_ok): 
            Serial.println("OK");
            receptor.resume();
            return 'M';
            break;
            case (tk_down): 
            Serial.println("Baixo");
            receptor.resume();
            return 'B';
            break;
            default:
            receptor.resume();
            break;             
        }
    }
    return 0;
}