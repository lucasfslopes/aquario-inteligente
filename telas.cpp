#include "telas.h"

void tela1() {

  tempoatual = millis();
  
  if (tempoatual - ultimaAtualizacaoTemperatura >= 2000) {
    sensors.requestTemperatures(); // Send the command to get temperatures
    tempAgua = sensors.getTempCByIndex(0);
    ultimaAtualizacaoTemperatura = tempoatual;
  }

  display.clearDisplay(); 
  moldura();
  barra_superior();

  display.setTextColor(SH110X_WHITE);
  display.drawBitmap(11, 36, temp_icon, 10, 24, 1); //(coluna, linha, variavel do bitmap, largura, altura, cor "acho")
  display.setTextSize(1); // scale text
  display.setCursor(6, 19);
  display.println("TEMPERATURA DA AGUA");
  display.setTextSize(2); // scale text
  display.setCursor(40, 40);
  display.println(String(tempAgua, 1) + (char)247 + "C"); 
  display.display();      // Show
  }

void tela2() {

  display.clearDisplay();
  moldura();
  barra_superior();

  display.setTextColor(SH110X_WHITE);
  display.drawBitmap(2, 36, alimento_icon, 33, 25, 1); //(coluna, linha, variavel do bitmap, largura, altura, cor "acho")
  display.setTextSize(1); // scale text
  display.setCursor(5, 19);
  display.println("PROXIMA ALIMENTACAO:");
  display.setTextSize(2); // scale text
  display.setCursor(40, 40);
  display.println("20:30");
  display.display();      // Show
  }

void tela3() {
  
  display.clearDisplay();
  moldura();
  barra_superior ();
 
  //Ventoinha

    display.setTextSize(1); // scale text
    display.setCursor(40, 17);
    display.println("COOLER:");
    display.drawRoundRect(40, 26, 65, 13, 3, SH110X_WHITE); 
    display.setCursor(44, 29);

      if(configData.PWMFan == 0){
        display.println("DESLIGADO");
        display.drawBitmap(7, 18, fan_icon, 21, 21, 1); //(coluna, linha, variavel do bitmap, largura, altura, cor)
      } else{
        display.println("LIGADO");
        
          if(tempoatual - tempoAnimacaoAnterior >= duracaoAnimacao) {
              display.drawBitmap(7, 18, fananima1_icon, 21, 21, 1); //(coluna, linha, variavel do bitmap, largura, altura, cor)
              
              if(tempoatual - tempoAnimacaoAnterior > (duracaoAnimacao * 2)) {
                  tempoAnimacaoAnterior = tempoatual;
              }
          }
          else{
              display.drawBitmap(7, 18, fananima2_icon, 21, 21, 1); //(coluna, linha, variavel do bitmap, largura, altura, cor)
          }
      }

  display.setTextSize(1); // scale text
  display.setCursor(40, 42);
  display.println("LUZ LED:");
  display.drawRoundRect(40, 51, 65, 13, 3, SH110X_WHITE);
  display.setCursor(44, 54);
  
  if(configData.PWMLuz01 == 0 && configData.PWMLuz02 == 0){
    display.println("DESLIGADO");
    display.drawBitmap(8, 41, luz_icon, 21, 21, 1); //(coluna, linha, variavel do bitmap, largura, altura, cor)
  } else{
    display.println("LIGADO");
    display.drawBitmap(8, 41, luz_icon, 21, 21, 1); //(coluna, linha, variavel do bitmap, largura, altura, cor)
  }

  display.display(); // Show 
}