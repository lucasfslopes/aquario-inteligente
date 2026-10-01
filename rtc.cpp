#include "RTClib.h"

RTC_DS1307 rtc;

void ajustarHora(int *vetorPtr){

    int posicaoCursor = 1;
    int hrTemp = (*vetorPtr);
    int mnTemp = (*(vetorPtr + 1));
    int sgTemp = (*(vetorPtr + 2));
    String strHora;
    String strMin;
    String strSeg;
    int w = 26;
    int h = 18;
    int r = 4;

    while (!posicaoCursor <= 0) {
      
          (hrTemp >= 0 && hrTemp <=9) ? strHora = ("0" + String(hrTemp)) : strHora = String(hrTemp);
          (mnTemp >= 0 && mnTemp <=9) ? strMin = ("0" + String(mnTemp)) : strMin = String(mnTemp);
          (sgTemp >= 0 && sgTemp <=9) ? strSeg = ("0" + String(sgTemp)) : strSeg = String(sgTemp);

          display.clearDisplay();
          display.setTextSize(1);
          display.setTextColor(SH110X_WHITE);
          display.setCursor(0, 0);
          display.print("Ajuste de Hora");

          display.setTextSize(2);
          display.setCursor(15, 30);
          display.setTextColor(SH110X_WHITE);
          display.print("  :  :  " );
          display.setCursor(15, 30);
          display.print(strHora);
          display.setCursor(51, 30);
          display.print(strMin);
          display.setCursor(88, 30);
          display.print(strSeg);

          //posição do ponteiro de ajuste
          if (posicaoCursor == 1) {
 
              display.fillRoundRect(13, 28, w, h, r, SH110X_WHITE);
              display.setTextSize(2);
              display.setTextColor(SH110X_BLACK);
              display.setCursor(15, 30);
              display.print(strHora);
                        
          }
          else if (posicaoCursor == 2) {

              display.fillRoundRect(49, 28, w, h, r, SH110X_WHITE);
              display.setTextSize(2);
              display.setTextColor(SH110X_BLACK);
              display.setCursor(51, 30);
              display.print(strMin);
                      
          }
          else if (posicaoCursor == 3) {
 
              display.fillRoundRect(86, 28, w, h, r, SH110X_WHITE);
              display.setTextSize(2);
              display.setTextColor(SH110X_BLACK);
              display.setCursor(88, 30);
              display.print(strSeg);
                        
          }
          else if (posicaoCursor == 4) {
              //rtc.adjust(DateTime(2024, 1, 1, hora, minutos, segundos));
              *vetorPtr = hrTemp;
              *(vetorPtr + 1) = mnTemp;
              *(vetorPtr + 2) = sgTemp;
              posicaoCursor = 0;   
              delay(100);        

              display.clearDisplay();
              display.setTextSize(1);
              display.setTextColor(SH110X_WHITE);
              display.setCursor(15, 30);
              display.print("Hora Ajustada!");
              display.display();
              delay(500);
          }

          display.display();

          //Botoes

              estadoBotao = teclado();
              estadoControle = controleIR();
              
              if (estadoBotao == 'M' || estadoControle == 'M') {
                    posicaoCursor++;              
                  }

              if (estadoBotao == 'V' || estadoControle == 'V') {
                    posicaoCursor--;
                  }
              if (estadoBotao == 'C' || estadoControle == 'C') {
                    if (posicaoCursor == 1) {
                          if (hrTemp >= 23) {
                              hrTemp = 0;                      
                          }
                          else {
                              hrTemp++;
                          }
                    }
                    else if (posicaoCursor == 2) {
                          if (mnTemp >= 59) {
                              mnTemp = 0;
                          }
                          else {
                              mnTemp++;
                          }
                    }
                    else if (posicaoCursor == 3) {
                          if (sgTemp >= 59) {
                              sgTemp = 0;
                          }
                          else {
                              sgTemp++;
                          }
                    }       
                  }

              if (estadoBotao == 'B' || estadoControle == 'B') {
                    if (posicaoCursor == 1) {
                          if (hrTemp <= 0) {
                              hrTemp = 23;                      
                          }
                          else {
                              hrTemp--;
                          }
                    }
                    else if (posicaoCursor == 2) {
                          if (mnTemp <= 0) {
                              mnTemp = 59;
                          }
                          else {
                              mnTemp--;
                          }
                    }
                    else if (posicaoCursor == 3) {
                          if (sgTemp <= 0) {
                              sgTemp = 59;
                          }
                          else {
                              sgTemp--;
                          }
                    }
                }
              delay(1);
    }
}