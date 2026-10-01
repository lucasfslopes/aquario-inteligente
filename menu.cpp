#include "menu.h"

void menuPrincipal() {

    const char *menuOptions[] = {"vazio" , "ALIMENTACAO" , "ILUMINACAO" , "TEMPERATURA" , "RESFRIAMENTO" , "AJUSTES DE HORA" , "WI-FI CONFIG" , "SALVAR CONFIG" , "RESET CONFIG" , "UPDATE" , "SOBRE"};
    int menuPosition = 1;
    int maxMenuPosition = 10;

    animacaotransicao();

    while (estadoBotao!= 'V' && estadoControle!= 'V') {
        
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(40, 0);
        display.print("Menu");
        
        if (menuPosition >= 1 && menuPosition <= 5) {
            display.setTextSize(1);
            display.setCursor(15, 17);
            display.print(menuOptions[1]);
            display.setCursor(15, 27);
            display.print(menuOptions[2]);
            display.setCursor(15, 37);
            display.print(menuOptions[3]);
            display.setCursor(15, 47);
            display.print(menuOptions[4]);
            display.setCursor(15, 56);
            display.print(menuOptions[5]);
        }        
        else if (menuPosition >= 6 && menuPosition <= 10) {
            display.setTextSize(1);
            display.setCursor(15, 17);
            display.print(menuOptions[6]);
            display.setCursor(15, 27);
            display.print(menuOptions[7]);
            display.setCursor(15, 37);
            display.print(menuOptions[8]);
            display.setCursor(15, 47);
            display.print(menuOptions[9]);
            display.setCursor(15, 56);
            display.print(menuOptions[10]);
        }

        seletor(menuPosition, menuOptions[menuPosition]);
        barravertical(menuPosition);
        display.display();
        
        //Botoes
        
            estadoBotao = teclado();
            estadoControle = controleIR();

            if (estadoBotao == 'M' || estadoControle == 'M') {

                switch (menuPosition) {
                    case 1:
                    menu01();
                    break;
                    case 2:
                    menu02();
                    break;
                    case 3:
                    menu03();
                    break;
                    case 4:
                    menu04();
                    break;
                    case 5:
                    menu05();
                    break;
                    case 6:
                    menu06();
                    break;
                    case 7:
                    menu07();
                    break;
                    case 8:
                    menu08();
                    break;
                    case 9:
                    menu09();
                    break;
                    case 10:
                    menu10();
                    break;
                }
            }

            if (estadoBotao == 'B' || estadoControle == 'B') {
                
                if (menuPosition >= maxMenuPosition) {
                    menuPosition = maxMenuPosition;
                }
                else {
                    menuPosition++;             
                }
            }

            if (estadoBotao == 'C' || estadoControle == 'C') {
                
                if (menuPosition <= 1) {
                    menuPosition = 1;
                }
                else {
                    menuPosition--;                 
                }
            }

            delay(100);
    }

    animacaotransicao();
}

void menu01() {
    
    const char *menuOptions[] = {"vazio" , "QUANTIDADE" , "FEED 1" , "FEED 2" , "TIME 1" , "TIME 2"};
    int menuPosition = 1;
    int maxMenuPosition = 5;
    int value = 0;
    
    while (estadoBotao!= 'V' && estadoControle!= 'V') {
      
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        seletor(menuPosition, "");
        display.setTextColor(SH110X_WHITE); display.setCursor(5, 0); display.print("ALIMENTACAO");

        display.setTextColor(menuPosition == 1 ? SH110X_BLACK : SH110X_WHITE);
        display.setCursor(10, 17); display.print(menuOptions[1]); display.setCursor(78, 17); display.print(String(configData.quantidadeFeed, 2) + "g");
        display.setTextColor(menuPosition == 2 ? SH110X_BLACK : SH110X_WHITE);
        display.setCursor(10, 27); display.print(menuOptions[2]); display.setCursor(78, 27); display.print(state[configData.feed01]);
        display.setTextColor(menuPosition == 3 ? SH110X_BLACK : SH110X_WHITE);
        display.setCursor(10, 37); display.print(menuOptions[3]); display.setCursor(78, 37); display.print(state[configData.feed02]);
        display.setTextColor(menuPosition == 4 ? SH110X_BLACK : SH110X_WHITE);
        display.setCursor(10, 47); display.print(menuOptions[4]); display.setCursor(78, 47); display.print(((configData.timeFeed01[0] <= 9) ? "0" + String(configData.timeFeed01[0]) : String(configData.timeFeed01[0])) + ":" + ((configData.timeFeed01[1] <= 9) ? "0" + String(configData.timeFeed01[1]) : String(configData.timeFeed01[1])));
        display.setTextColor(menuPosition == 5 ? SH110X_BLACK : SH110X_WHITE);
        display.setCursor(10, 56); display.print(menuOptions[5]); display.setCursor(78, 56); display.print(((configData.timeFeed02[0] <= 9) ? "0" + String(configData.timeFeed02[0]) : String(configData.timeFeed02[0])) + ":" + ((configData.timeFeed02[1] <= 9) ? "0" + String(configData.timeFeed02[1]) : String(configData.timeFeed02[1])));
        display.display();

        estadoBotao = teclado();
        estadoControle = controleIR();

        if (estadoBotao == 'M' || estadoControle == 'M') {
            switch (menuPosition) {
                case 1: //Sub Menu 
                    
                     while(estadoBotao!= 'V' && estadoControle!= 'V') {
                        
                        display.clearDisplay();
                        display.setTextSize(1);
                        display.setTextColor(SH110X_WHITE);
                        display.setCursor(0, 0);
                        display.print("Quantidade Feed");
                        display.setTextSize(2);
                        display.setCursor(42, 32);
                        display.print(String(configData.quantidadeFeed, 2) + "g");
                        moldura();
                        display.display();
                        
                        //Botoes  

                            estadoBotao = teclado();
                            estadoControle = controleIR();
                          
                            if (estadoBotao == 'B' || estadoControle == 'B') {
                                (configData.quantidadeFeed <= 0) ? configData.quantidadeFeed = 0 : configData.quantidadeFeed-=0.1;
                            }

                            else if (estadoBotao == 'C' || estadoControle == 'C') {
                                (configData.quantidadeFeed >= 5) ? (configData.quantidadeFeed = 5, buzzerErro(1)) : configData.quantidadeFeed+=0.1;
                            }
                            delay(10);

                    }
                    estadoBotao = teclado();
                    estadoControle = controleIR(); 
                break;
                case 2: //Sub Menu 
                    configData.feed01 = !configData.feed01;
                    delay(100);
                break;
                case 3: //Sub Menu 
                    configData.feed02 = !configData.feed02;
                    delay(100);
                break;
                case 4:

                    ajustarHora(configData.timeFeed01);
                break;
                case 5:

                    ajustarHora(configData.timeFeed02);
                break;
            }
        }

        //Botoes MENU 01

            if (estadoBotao == 'B' || estadoControle == 'B') {
                (menuPosition >= maxMenuPosition) ? menuPosition = maxMenuPosition : menuPosition++;
            }

            if (estadoBotao == 'C' || estadoControle == 'C') {
                (menuPosition <= 1) ? menuPosition = 1 : menuPosition--;
            }
    }
    estadoBotao = teclado();
    estadoControle = controleIR(); 
  }

void menu02() {
    
    const char *menuOptions[] = {"vazio" , "ON/OFF" , "DIMMER" , "MODO" , "AUTO ON" , "AUTO OFF", "ON/OFF" , "DIMMER" , "MODO" , "AUTO ON" , "AUTO OFF"};
    int menuPosition = 1;
    int maxMenuPosition = 10;
    int value = 0;
    
    while (estadoBotao!= 'V' && estadoControle!= 'V') {
      
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        seletor(menuPosition, "");

        if (menuPosition >= 1 && menuPosition <= 5) {

            display.setTextColor(SH110X_WHITE); display.setCursor(5, 0); display.print("Iluminacao: LUZ 1");
            display.setTextColor(menuPosition == 1 ? SH110X_BLACK : SH110X_WHITE);
            display.setCursor(10, 17); display.print(menuOptions[1]); display.setCursor(78, 17); display.print(state[configData.luz01Estado]);
            display.setTextColor(menuPosition == 2 ? SH110X_BLACK : SH110X_WHITE);
            display.setCursor(10, 27); display.print(menuOptions[2]); display.setCursor(78, 27); display.print(String(int(configData.PWMLuz01 / 2.55)) + "%");
            display.setTextColor(menuPosition == 3 ? SH110X_BLACK : SH110X_WHITE);
            display.setCursor(10, 37); display.print(menuOptions[3]); display.setCursor(78, 37); display.print(luzMode[configData.luz01mode]);
            display.setTextColor(menuPosition == 4 ? SH110X_BLACK : SH110X_WHITE);
            display.setCursor(10, 47); display.print(menuOptions[4]); display.setCursor(78, 47); display.print(((configData.timeOnLuz01[0] <= 9) ? "0" + String(configData.timeOnLuz01[0]) : String(configData.timeOnLuz01[0])) + ":" + ((configData.timeOnLuz01[1] <= 9) ? "0" + String(configData.timeOnLuz01[1]) : String(configData.timeOnLuz01[1])));
            display.setTextColor(menuPosition == 5 ? SH110X_BLACK : SH110X_WHITE);
            display.setCursor(10, 56); display.print(menuOptions[5]); display.setCursor(78, 56); display.print(((configData.timeOffLuz01[0] <= 9) ? "0" + String(configData.timeOffLuz01[0]) : String(configData.timeOffLuz01[0])) + ":" + ((configData.timeOffLuz01[1] <= 9) ? "0" + String(configData.timeOffLuz01[1]) : String(configData.timeOffLuz01[1])));

        }        
        else if (menuPosition >= 6 && menuPosition <= 10) {
            
            display.setTextColor(SH110X_WHITE); display.setCursor(5, 0); display.print("Iluminacao: LUZ 2");
            display.setTextColor(menuPosition == 6 ? SH110X_BLACK : SH110X_WHITE);
            display.setCursor(10, 17); display.print(menuOptions[6]); display.setCursor(78, 17); display.print(state[configData.luz02Estado]);
            display.setTextColor(menuPosition == 7 ? SH110X_BLACK : SH110X_WHITE);
            display.setCursor(10, 27); display.print(menuOptions[7]); display.setCursor(78, 27); display.print(String(int(configData.PWMLuz02 / 2.55)) + "%");
            display.setTextColor(menuPosition == 8 ? SH110X_BLACK : SH110X_WHITE);
            display.setCursor(10, 37); display.print(menuOptions[8]); display.setCursor(78, 37); display.print(luzMode[configData.luz02mode]);
            display.setTextColor(menuPosition == 9 ? SH110X_BLACK : SH110X_WHITE);
            display.setCursor(10, 47); display.print(menuOptions[9]); display.setCursor(78, 47); display.print(((configData.timeOnLuz02[0] <= 9) ? "0" + String(configData.timeOnLuz02[0]) : String(configData.timeOnLuz02[0])) + ":" + ((configData.timeOnLuz02[1] <= 9) ? "0" + String(configData.timeOnLuz02[1]) : String(configData.timeOnLuz02[1])));
            display.setTextColor(menuPosition == 10 ? SH110X_BLACK : SH110X_WHITE);
            display.setCursor(10, 56); display.print(menuOptions[10]); display.setCursor(78, 56); display.print(((configData.timeOffLuz02[0] <= 9) ? "0" + String(configData.timeOffLuz02[0]) : String(configData.timeOffLuz02[0])) + ":" + ((configData.timeOffLuz02[1] <= 9) ? "0" + String(configData.timeOffLuz02[1]) : String(configData.timeOffLuz02[1])));

        }

        display.display();

        estadoBotao = teclado();
        estadoControle = controleIR();

        if (estadoBotao == 'M' || estadoControle == 'M') {
            switch (menuPosition) {
                case 1: //Sub Menu LUZ 01 ON/OFF

                    configData.luz01Estado = !configData.luz01Estado;
                    delay(100);      
                break;
                case 2: //Sub Menu LUZ 01 DIMMER
                    
                    value = configData.PWMLuz01 / 2.55;

                    while(estadoBotao!= 'V' && estadoControle!= 'V') {
                        
                        display.clearDisplay();
                        display.setTextSize(1);
                        display.setTextColor(SH110X_WHITE);
                        display.setCursor(0, 0);
                        display.print("LUZ 01 DIMMER");
                        display.setTextSize(2);
                        display.setCursor(42, 32);
                        display.print(String(value) + "%");
                        moldura();
                        display.display();
                        
                        //Botoes  

                            estadoBotao = teclado();
                            estadoControle = controleIR();
                          
                            if (estadoBotao == 'B' || estadoControle == 'B') {
                                (value <= 0) ? value = 0 : value-=5;
                            }

                            else if (estadoBotao == 'C' || estadoControle == 'C') {
                                (value >= 100) ? (value = 100, buzzerErro(1)) : value+=5;
                            }

                        configData.PWMLuz01 = (value * 2.55);

                        regrasSaidas();

                        delay(1);
                    }
                    estadoBotao = teclado();
                    estadoControle = controleIR();                    
                break;
                case 3: //Sub Menu LUZ 01 MODO
                    
                    configData.luz01mode = !configData.luz01mode;
                    delay(100);

                break;
                case 4: //Sub Menu LUZ 01 ON
                    
                    ajustarHora(configData.timeOnLuz01);
                break;
                case 5: //Sub Menu LUZ 01 OFF
                    
                    ajustarHora(configData.timeOffLuz01);            
                break;
                case 6: //Sub Menu LUZ 02 ON/OFF

                    configData.luz02Estado = !configData.luz02Estado;
                    delay(100);      
                break;
                case 7: //Sub Menu LUZ 02 DIMMER

                    value = configData.PWMLuz02 / 2.55;
                    
                    while(estadoBotao!= 'V' && estadoControle!= 'V') {
                        
                        display.clearDisplay();
                        display.setTextSize(2);
                        display.setTextColor(SH110X_WHITE);
                        display.setCursor(0, 0);
                        display.print("Luz Red");
                        display.setCursor(42, 32);
                        display.print(String(value) + "%");
                        moldura();
                        display.display();
                        
                        //Botoes
                          
                            estadoBotao = teclado();
                            estadoControle = controleIR();
                          
                            if (estadoBotao == 'B' || estadoControle == 'B') {
                                (value <= 0) ? value = 0 : value-=5;
                            }

                            else if (estadoBotao == 'C' || estadoControle == 'C') {
                                (value >= 100) ? (value = 100, buzzerErro(1)) : value+=5;
                            }

                        configData.PWMLuz02 = (value * 2.55);

                        regrasSaidas();

                        delay(1);
                    }
                    
                break;
                case 8: //Sub Menu LUZ 02 MODO
                    
                    configData.luz02mode = !configData.luz02mode;
                    delay(100);   
                break;
                case 9: //Sub Menu LUZ 02 ON
                    
                    ajustarHora(configData.timeOnLuz02);           
                break;
                case 10: //Sub Menu LUZ 02 OFF
                    
                    ajustarHora(configData.timeOffLuz02);            
                break;
                case 11: //Sub Menu LUZ 02 PENUMBRA
                    while (estadoBotao!= 'V' && estadoControle!= 'V') {
                        
                        display.clearDisplay();
                        display.setTextSize(1);
                        display.setTextColor(SH110X_WHITE);
                        display.setCursor(0, 0);
                        display.print("LUZ 02 PENUMBRA");
                        display.setTextSize(2);
                        display.setCursor(42, 32);
                        display.print(String(configData.luz02Penumbra) + "min");
                        moldura();
                        display.display();
                        
                        //Botoes  

                            estadoBotao = teclado();
                            estadoControle = controleIR();

                            if (estadoBotao == 'B' || estadoControle == 'B') {
                                (configData.luz02Penumbra <= 0) ? configData.luz02Penumbra = 0 : configData.luz02Penumbra-=5;
                            }

                            else if (estadoBotao == 'C' || estadoControle == 'C') {
                                (configData.luz02Penumbra >= maxPenumbra) ? (configData.luz02Penumbra = maxPenumbra, buzzerErro(1)) : configData.luz02Penumbra+=5;
                            }
                    }
                    estadoBotao = teclado();
                    estadoControle = controleIR();             
                break;
                
            }
        }
        
        //Botoes MENU 02

            if (estadoBotao == 'B' || estadoControle == 'B') {
                (menuPosition >= maxMenuPosition) ? menuPosition = maxMenuPosition : menuPosition++;
            }

            if (estadoBotao == 'C' || estadoControle == 'C') {
                (menuPosition <= 1) ? menuPosition = 1 : menuPosition--;
            }
    }
    estadoBotao = teclado();
    estadoControle = controleIR();
  }

void menu03() {
    
    const char *menuOptions[] = {"vazio" , "ALARME" , "INTERVALO" , "TEMP MIN" , "TEMP MAX"};
    int menuPosition = 1;
    int maxMenuPosition = 4;
    
    while (estadoBotao!= 'V' && estadoControle!= 'V') {

        display.clearDisplay();
        display.setTextColor(SH110X_WHITE);
        display.setTextSize(1); // scale text
        display.setCursor(0, 0);
        display.println("Temperatura  da Agua");
        seletor(menuPosition, "");

        display.setTextColor(menuPosition == 1 ? SH110X_BLACK : SH110X_WHITE);
        display.setCursor(10, 17); display.print(menuOptions[1]); display.setCursor(78, 17); display.print(state[configData.alarme]);
        display.setTextColor(menuPosition == 2 ? SH110X_BLACK : SH110X_WHITE);
        display.setCursor(10, 27); display.print(menuOptions[2]); display.setCursor(78, 27); display.print(String(configData.intervaloAlarme) + "min");
        display.setTextColor(menuPosition == 3 ? SH110X_BLACK : SH110X_WHITE);
        display.setCursor(10, 37); display.print(menuOptions[3]); display.setCursor(78, 37); display.print(String(configData.tempMin, 0) + (char)247 + "C");
        display.setTextColor(menuPosition == 4 ? SH110X_BLACK : SH110X_WHITE);
        display.setCursor(10, 47); display.print(menuOptions[4]); display.setCursor(78, 47); display.print(String(configData.tempMax, 0) + (char)247 + "C");
        display.display();

        estadoBotao = teclado();
        estadoControle = controleIR();

        if (estadoBotao == 'M' || estadoControle == 'M') {
            switch (menuPosition) {
                case 1: //Sub Menu 01

                    configData.alarme = !configData.alarme;
                    delay(100);
                break;
                case 2: //Sub Menu 02

                    while (estadoBotao!= 'V' && estadoControle!= 'V') {
                        
                        display.clearDisplay();
                        display.setTextSize(1);
                        display.setTextColor(SH110X_WHITE);
                        display.setCursor(0, 0);
                        display.print("Intervalo Alarme");
                        display.setTextSize(2);
                        display.setCursor(42, 32);
                        display.print(String(configData.intervaloAlarme) + "min");
                        moldura();
                        display.display();
                        
                        //Botoes  

                            estadoBotao = teclado();
                            estadoControle = controleIR();
                          
                            if (estadoBotao == 'B' || estadoControle == 'B') {
                                (configData.intervaloAlarme <= 0) ? configData.intervaloAlarme = 0 : configData.intervaloAlarme-=1;
                            }

                            else if (estadoBotao == 'C' || estadoControle == 'C') {
                                (configData.intervaloAlarme >= maxPenumbra) ? (configData.intervaloAlarme = maxPenumbra, buzzerErro(1)) : configData.intervaloAlarme+=1;
                            }
                    }
                    estadoBotao = teclado();
                    estadoControle = controleIR(); 
                break;
                case 3: //Sub Menu 03

                    while(estadoBotao!= 'V' && estadoControle!= 'V') {
                            
                            display.clearDisplay();
                            display.setTextSize(1);
                            display.setTextColor(SH110X_WHITE);
                            display.setCursor(0, 0);
                            display.print("Temperatura Min");
                            display.setTextSize(2);
                            display.setCursor(42, 32);
                            display.print(String(configData.tempMin, 0) + (char)247 + "C");
                            moldura();
                            display.display();
                            
                            //Botoes  

                                estadoBotao = teclado();
                                estadoControle = controleIR();
                              
                                if (estadoBotao == 'B' || estadoControle == 'B') {
                                    (configData.tempMin <= -10) ? configData.tempMin = -10 : configData.tempMin-=1;
                                }

                                else if (estadoBotao == 'C' || estadoControle == 'C') {
                                    (configData.tempMin >= 50) ? (configData.tempMin = 50, buzzerErro(1)) : configData.tempMin+=1;
                                }

                        }
                        estadoBotao = teclado();
                        estadoControle = controleIR(); 
                break;
                case 4: //Sub Menu 04

                    while(estadoBotao!= 'V' && estadoControle!= 'V') {
                            
                            display.clearDisplay();
                            display.setTextSize(1);
                            display.setTextColor(SH110X_WHITE);
                            display.setCursor(0, 0);
                            display.print("Temperatura Max");
                            display.setTextSize(2);
                            display.setCursor(42, 32);
                            display.print(String(configData.tempMax, 0) + (char)247 + "C");
                            moldura();
                            display.display();
                            
                            //Botoes  

                                estadoBotao = teclado();
                                estadoControle = controleIR();
                              
                                if (estadoBotao == 'B' || estadoControle == 'B') {
                                    (configData.tempMax <= -10) ? configData.tempMax = -10 : configData.tempMax-=1;
                                }

                                else if (estadoBotao == 'C' || estadoControle == 'C') {
                                    (configData.tempMax >= 50) ? (configData.tempMax = 50, buzzerErro(1)) : configData.tempMax+=1;
                                }

                        }
                        estadoBotao = teclado();
                        estadoControle = controleIR();
                break;
            }
        }

        //Botoes MENU 03

            if (estadoBotao == 'B' || estadoControle == 'B') {
                (menuPosition >= maxMenuPosition) ? menuPosition = maxMenuPosition : menuPosition++;
            }

            if (estadoBotao == 'C' || estadoControle == 'C') {
                (menuPosition <= 1) ? menuPosition = 1 : menuPosition--;
            }
    }
  }

void menu04() {
    int value = 0;

    while (estadoBotao!= 'V' && estadoControle!= 'V') {

    //Cooler

    //ON/OFF
    //+ 2ºc
    //Temp Max
    //-2
    //-4


    configData.PWMFan = (value * 2.55);
    analogWrite(fanpin, configData.PWMFan);

    display.clearDisplay();
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(SH110X_WHITE); // Draw white text
    display.setCursor(0, 0);
    display.print("Resfriamento");
    display.setCursor(40, 45);
    display.setTextSize(2);
    display.println(String(value) + "%");
    display.setCursor(2, 17);
    display.setTextSize(1);
    display.println("PWMFan: " + String(configData.PWMFan));
    display.display();


   //Botoes  

        estadoBotao = teclado();
        estadoControle = controleIR();
      
            if (estadoBotao == 'B' || estadoControle == 'B') {
                if (value <= 0) {
                  value = 0;
                }
                else {
                  value-=5;
                }
            }

            if (estadoBotao == 'C' || estadoControle == 'C') {
                if (value >= 100) {
                value = 100;
                buzzerErro(1);
                }
                else {
                  value+=5;
                }
            }    
    }

  }

void menu05() {

    ajustarHora(hora);
    rtc.adjust(DateTime(2026, 1, 1, hora[0], hora[1], hora[2]));

  }

void menu06() {
      
      const char *menuOptions[] = {"vazio" , "CONECTAR WI-FI" , "CONFIG WI-FI" , "RESET WI-FI" , "INFO SSID"};
      int menuPosition = 1;
      int maxMenuPosition = 4;

      while (estadoBotao!= 'V' && estadoControle!= 'V') {
          
          display.clearDisplay();
          display.setTextSize(2);      // Normal 1:1 pixel scale
          display.setTextColor(SH110X_WHITE); // Draw white text
          display.setCursor(35, 0);
          display.print("WI-FI");

          display.setTextSize(1);
          display.setCursor(15, 17);
          display.print(menuOptions[1]);
          display.setCursor(15, 27);
          display.print(menuOptions[2]);
          display.setCursor(15, 37);
          display.print(menuOptions[3]);
          display.setCursor(15, 47);
          display.print(menuOptions[4]);

          seletor(menuPosition, menuOptions[menuPosition]);

          display.display();

          estadoBotao = teclado();
          estadoControle = controleIR();

          //Sub menu 06
              if (estadoBotao == 'M' || estadoControle == 'M') {
                    switch (menuPosition) {
                        case 1:
                                display.clearDisplay();
                                display.setTextSize(1);
                                display.setCursor(0, 16);
                                display.print("Em desenvolvimento.");
                                delay(1000);                                            
                        break;
                        case 2:
                            configWiFi();
                        break;
                        case 3:
                            display.clearDisplay();
                            display.setTextSize(1);      // Normal 1:1 pixel scale
                            display.setTextColor(SH110X_WHITE); // Draw white text
                            display.setCursor(0, 16);
                            display.print("Resetando WI-FI...");
                            display.display();
                            resetWiFi();
                            delay(2000);              
                        break;
                        case 4:
                            while (teclado()!= 'V' && controleIR()!= 'V') {
                                  display.clearDisplay();
                                  display.setTextSize(1);      // Normal 1:1 pixel scale
                                  display.setTextColor(SH110X_WHITE); // Draw white text
                                  display.setCursor(0, 16);
                                  display.print("SSID:  " + String(wifi_ssid));
                                  display.setCursor(0, 32);
                                  display.print("Password:  " + String(wifi_password));
                                  display.display();
                            }             
                        break;
                        
                    }
              }

          //Botoes
              
              if (estadoBotao == 'B' || estadoControle == 'B') {
                  if (menuPosition >= maxMenuPosition) {
                    menuPosition = maxMenuPosition;
                    display.display();
                  }
                  else {
                    menuPosition++;
                    display.display();
                  }
              }

              if (estadoBotao == 'C' || estadoControle == 'C') {
                  if (menuPosition <= 1) {
                  menuPosition = 1;
                  display.display();
                  }
                  else {
                    menuPosition--;
                    display.display();
                  }
              }        
      }

 }

void menu07(){

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(21, 10);
    display.print("Salvando...");
    display.display();
    delay(1000);
    
    configSave();

    display.setCursor(21, 27);
    display.print("Config salva!");
    display.display();
    delay(1000);
  }

void menu08(){

    while (estadoBotao!= 'V' && estadoControle!= 'V') {
    
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(21, 10);
        display.print("Tem certeza?");
        display.setCursor(0, 40);
        display.print("Pressione OK para");
        display.setCursor(0, 48);
        display.print("confirmar.");
        display.display();
        delay(1000);
            
        estadoBotao = teclado();
        estadoControle = controleIR();

        if (estadoBotao == 'M' || estadoControle == 'M') {
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(SH110X_WHITE);
            display.setCursor(21, 10);
            display.print("Resetando...");
            display.display();
            delay(1000);
            
            configReset();

            display.setCursor(21, 27);
            display.print("Concluido!");
            display.display();
            delay(1000);
            break;
        }
    }
  }

void menu09(){

    display.clearDisplay();
    display.setTextSize(2);      // Normal 1:1 pixel scale
    display.setTextColor(SH110X_WHITE); // Draw white text
    display.setCursor(0, 0);
    display.print("Update");
    display.display();

    bool update = true;

    while(update){

        wm.setConfigPortalTimeout(10); //Tempo em segundos que o wifi tenta se conectar antes de continuar o codigo

        bool res;
        res = wm.autoConnect(wifi_ssid , wifi_password); // password protected ap

        if(!res) {
            display.setTextColor(SH110X_WHITE);
            display.setTextSize(1); // scale text
            display.setCursor(5, 17);
            display.print("Nenhuma rede WI-FI");
            display.setCursor(5, 27);
            display.print("foi conectada!");
            display.display();
            delay(2000);
            wm.setConfigPortalTimeout(30);
            update = false;            
            //ESP.restart();
        } 
        else {
            
            display.clearDisplay();
            display.setTextSize(2);      // Normal 1:1 pixel scale
            display.setTextColor(SH110X_WHITE); // Draw white text
            display.setCursor(0, 0);
            display.print("Update");
            display.setTextColor(SH110X_WHITE);
            display.setTextSize(1); // scale text
            display.setCursor(0, 18);
            display.print("Ready!");
            display.display();
            delay(1000);

            beginUpdate();

            while(update){
                server.handleClient();
                MDNS.update();
            }            
        }
    }
  }
void menu10() {
    
    tone(buzzer, 659); delay(300); // Mi
    tone(buzzer, 659); delay(300); // Mi
    tone(buzzer, 698); delay(300); // Fá
    tone(buzzer, 784); delay(300); // Sol

    tone(buzzer, 784); delay(300); // Sol
    tone(buzzer, 698); delay(300); // Fá
    tone(buzzer, 659); delay(300); // Mi
    tone(buzzer, 587); delay(300); // Ré

    tone(buzzer, 523); delay(300); // Dó
    tone(buzzer, 523); delay(300); // Dó
    tone(buzzer, 587); delay(300); // Ré
    tone(buzzer, 659); delay(400); // Mi
    noTone(buzzer);

    while (teclado()!= 'V' && controleIR()!= 'V') {
      display.clearDisplay();
      display.setTextSize(2);      // Normal 1:1 pixel scale
      display.setTextColor(SH110X_WHITE); // Draw white text
      display.setCursor(21, 0);
    display.print("Sobre");
    display.display();
    }
  }
