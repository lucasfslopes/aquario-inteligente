#include "portas.h"
#include "eeprom_config.h"
#include "display_config.h"
#include "webserver_config.h"
#include "bitmaps.h"

//Bibliotecas

    #include <Wire.h>
    #include "RTClib.h"
    #include <OneWire.h>
    #include <DallasTemperature.h>
    #include <IRrecv.h>

//Variaveis globais

      
      int contrasteLCD = 1;
      int estado_de_tela = 1;
      bool estadoWiFi = false;
      int incrementoPWMLuz01 = 0;
      int incrementoPWMLuz02 = 0;

      //Tempo e Hora

          int hora[3] = {0, 0, 0};
          
          int duracaoAnimacao = 200; //Milisegundos
          int tempoTransicaoTelas = 4000; //Milisegundos
          int maxPenumbra = 120; //Minutos

          unsigned long tempoatual = 0;
          unsigned long tempoanterior = 0;
          unsigned long timerequest = 0;
          unsigned long ultimaAtualizacaoTemperatura = 0;
          unsigned long tempoUltimoAlarme = 0;
          unsigned long tempoAnimacaoAnterior = 0;
          unsigned long fadeAnteriorLuz01 = 0;
          unsigned long fadeAnteriorLuz02 = 0;

      //Textos globais

          const char *luzMode[] = {"MANUAL" , "AUTO"};
          const char *state[] = {"OFF" , "ON"};

      //Leitura Sensores
      
          char estadoBotao;
          char estadoControle;
          float tempAgua = 0;




//Modulo RTC Data e  Hora

      RTC_DS1307 rtc;



  //Receptor Infravermelho
  
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

  //Sensor Temperatura

      OneWire oneWire(ONE_WIRE_BUS); //Sensor Temperatura - Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
      DallasTemperature sensors(&oneWire); //Pass our oneWire reference to Dallas Temperature.



void setup() {

  pinMode(botao, INPUT);
  pinMode(fanpin, OUTPUT);
  pinMode(luz02Pin, OUTPUT);
  pinMode(luz01Pin, OUTPUT);
  pinMode(buzzer, OUTPUT);

  millis();
  EEPROM.begin(sizeof(structConfig));
  Serial.begin(9600);
  rtc.begin();  
  sensors.begin();
  receptor.enableIRIn(); // inicializa a recepção sinais do controle remoto
  display.begin(i2c_Address, true); // Address 0x3C default
  display.setRotation(2);

  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  //Imprime a Logo e a Versao

      display.clearDisplay();
      display.setContrast(contrasteLCD);
      display.drawBitmap( (display.width() - LOGO_WIDTH ) / 2, (display.height() - LOGO_HEIGHT) / 2, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();
      delay(1000);
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(90, 54);
      display.print(versaoFirmware);
      display.display();
      delay(2000);

      // --- Lendo os dados existentes (no boot) ---
      EEPROM.get(0, configData);

      animacaotransicao();
      delay(1000);

      wificonnect();
      tempoanterior = tempoatual;
  }

void loop() {
  
    tempoatual = millis();

    regrasSaidas();

    switch (estado_de_tela) {
        case 1:    
            tela1();

            /*if (tempoatual - tempoanterior >= tempoTransicaoTelas) {
              estado_de_tela = 2;
              tempoanterior = tempoatual;
            }*/
        break;
        case 2:
          
            tela2();

            /*if (tempoatual - tempoanterior >= tempoTransicaoTelas) {
              estado_de_tela = 3;
              tempoanterior = tempoatual;
            }*/
        break;
        case 3:

            tela3();

            /*if (tempoatual - tempoanterior >= tempoTransicaoTelas) {
              estado_de_tela = 1;
              tempoanterior = tempoatual;
            } */
        break;
        case 4:

            menuPrincipal();
          
            tempoanterior = tempoatual;
            estado_de_tela = 1;
        break;
    }
    
    //Botões
    
        estadoBotao = teclado();
        estadoControle = controleIR();

        (estadoBotao == 'M' || estadoControle == 'M') ? (estado_de_tela = 4) : (estado_de_tela = estado_de_tela);
            
        if (estadoBotao == 'C' || estadoControle == 'C') {
            tempoanterior = tempoatual;
            (estado_de_tela <= 1) ? estado_de_tela = 3 : estado_de_tela--;
        }
        if (estadoBotao == 'B' || estadoControle == 'B') {
            tempoanterior = tempoatual;
            (estado_de_tela >= 3) ? estado_de_tela = 1 : estado_de_tela++;
        }        
        delay(100);
  }

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

void seletor(int menuPosition, String arrow_text) {
    
    int arrow_pos = ((menuPosition - 1) % 5) + 1;
    int max_arrow_pos = 5;
    int w = 110;
    int h = 9;
    int r = 4;

    if (arrow_pos > max_arrow_pos) {
        arrow_pos = 1;
    }
    else if (arrow_pos < 1 ) {
        arrow_pos = 5;
    }
    else {
        if (arrow_pos == 1) {
            
            int x = 5;
            int y = 16;
          
            display.fillRoundRect(x, y, w, h, r, SH110X_WHITE);
            //display.fillRect(x, y, w, h, SH110X_WHITE);
            display.setTextSize(1); // Normal 1:1 pixel scale
            display.setTextColor(SH110X_BLACK);
            display.setCursor(15, 17);
            display.print(arrow_text);   
        }
        else if (arrow_pos == 2) {
          
          int x = 5;
          int y = 26; 
            
          display.fillRoundRect(x, y, w, h, r, SH110X_WHITE);
          //display.fillRect(x, y, w, h, SH110X_WHITE);
          display.setTextSize(1);      // Normal 1:1 pixel scale
          display.setTextColor(SH110X_BLACK);
          display.setCursor(15, 27);
          display.print(arrow_text);

        }
        else if (arrow_pos == 3) {
          
          int x = 5;
          int y = 36;

          display.fillRoundRect(x, y, w, h, r, SH110X_WHITE);
          //display.fillRect(x, y, w, h, SH110X_WHITE);
          display.setTextSize(1);      // Normal 1:1 pixel scale
          display.setTextColor(SH110X_BLACK);
          display.setCursor(15, 37);
          display.print(arrow_text);
        
        }
        else if (arrow_pos == 4) {
          
          int x = 5;
          int y = 46;
          
          display.fillRoundRect(x, y, w, h, r, SH110X_WHITE);
          //display.fillRect(x, y, w, h, SH110X_WHITE);
          display.setTextSize(1);      // Normal 1:1 pixel scale
          display.setTextColor(SH110X_BLACK);
          display.setCursor(15, 47);
          display.print(arrow_text);
        
        }        
        else {
          
          int x = 5;
          int y = 55;
          
          display.fillRoundRect(x, y, w, h, r, SH110X_WHITE);
          //display.fillRect(x, y, w, h, SH110X_WHITE);
          display.setTextSize(1);      // Normal 1:1 pixel scale
          display.setTextColor(SH110X_BLACK);
          display.setCursor(15, 56);
          display.print(arrow_text);
        
        }
    }
  }

void barra_superior () {
  
    String strHora;
    String strMin;  
    String strSeg;

    tempoatual = millis();

    if (tempoatual - timerequest >= 1000) {
        DateTime now = rtc.now();
        hora[0] = now.hour(), DEC;
        hora[1] = now.minute(), DEC;
        hora[2] = now.second(), DEC;
        timerequest = tempoatual;        
    }

    (hora[0] >= 0 && hora[0] <=9) ? strHora = ("0" + String(hora[0])) : strHora = String(hora[0]);
    (hora[1] >= 0 && hora[1] <=9) ? strMin = ("0" + String(hora[1])) : strMin = String(hora[1]);
    (hora[2] >= 0 && hora[2] <=9) ? strSeg = ("0" + String(hora[2])) : strSeg = String(hora[2]);
            
    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.print(strHora + ":" + strMin + ":" + strSeg);
  
    if(estadoWiFi){display.drawBitmap(112, 0, wifi_icon, 16, 16, 1);}

 }
 
void moldura() {
  display.drawRoundRect(0, 16, 128, 48, 3, SH110X_WHITE);
  //display.drawRect(0 , 16 , 128 , 16 , SSD1306_WHITE);
 }
void barravertical(int position) {
    
    display.drawLine(125, 16, 125, 64, SH110X_WHITE);

    if (position == 1) {
      display.drawRoundRect(124, 18, 3, 8, 2, SH110X_WHITE);
    }
    else if (position == 2) {
      display.drawRoundRect(124, 24, 3, 8, 2, SH110X_WHITE);
    }
    else if (position == 3) {
      display.drawRoundRect(124, 31, 3, 8, 2, SH110X_WHITE);
    }
    else if (position == 4) {
      display.drawRoundRect(124, 36, 3, 8, 2, SH110X_WHITE);
    }
    else if (position == 5) {
      display.drawRoundRect(124, 42, 3, 8, 2, SH110X_WHITE);
    }
    else if (position == 6) {
      display.drawRoundRect(124, 48, 3, 8, 2, SH110X_WHITE);
    }
    else if (position == 7) {
      display.drawRoundRect(124, 54, 3, 8, 2, SH110X_WHITE);
    }

    display.display();
 }

void animacaotransicao() {
      
    display.clearDisplay();

    for(int16_t i=0; i<display.height()/2-2; i+=2) {
        display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, SH110X_WHITE);
        display.display();
        delay(1);
    }
 }

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


void regrasSaidas(){

    int horaAtual = hora[0] * 3600 + hora[1] * 60 + hora[2];
    int horaOn;
    int horaOff;
    bool dentroIntervalo;

    //Controla o alarme de temperatura
        if(configData.alarme && tempAgua < configData.tempMin){
            if((tempoatual - tempoUltimoAlarme) > (configData.intervaloAlarme * 60000)){
                msgErro(3);
                buzzerErro(2);
                tempoUltimoAlarme = tempoatual;
            }
        }
        else if(configData.alarme && tempAgua > configData.tempMax){
            if((tempoatual - tempoUltimoAlarme) > (configData.intervaloAlarme * 60000)){
                msgErro(2);
                buzzerErro(2);
                tempoUltimoAlarme = tempoatual;
            }
        }

    //Controla a saida de Cooler
        (configData.PWMFan > 0) ? analogWrite(fanpin, configData.PWMFan) : analogWrite(fanpin, 0);

    //Controla a saida de luz 01
        if(configData.luz01Estado && !configData.luz01mode){ //Se modo estiver manual
            analogWrite(luz01Pin, configData.PWMLuz01);
        }
        else if(configData.luz01Estado && configData.luz01mode){ //Se modo estiver automatico
            
            horaOn  = configData.timeOnLuz01[0] * 3600 + configData.timeOnLuz01[1] * 60 + configData.timeOnLuz01[2];
            horaOff = configData.timeOffLuz01[0] * 3600 + configData.timeOffLuz01[1] * 60 + configData.timeOffLuz01[2];

            //Intervalo normal / Intervalo normal
            (horaOn <= horaOff) ? dentroIntervalo = (horaAtual >= horaOn && horaAtual <= horaOff) : dentroIntervalo = (horaAtual >= horaOn || horaAtual <= horaOff);

            if(dentroIntervalo){
                if (millis() - fadeAnteriorLuz01 >= 10){
                    fadeAnteriorLuz01 = millis();
                        
                    if(incrementoPWMLuz01 <= configData.PWMLuz01){
                        analogWrite(luz01Pin, incrementoPWMLuz01);
                        incrementoPWMLuz01++;
                    }
                    else{
                        analogWrite(luz01Pin, configData.PWMLuz01);
                    }                       
                }
            }
            else{
                if (millis() - fadeAnteriorLuz01 >= 10){
                    fadeAnteriorLuz01 = millis();
                        
                    if(incrementoPWMLuz01 >= 0){
                        analogWrite(luz01Pin, incrementoPWMLuz01);
                        incrementoPWMLuz01--;
                    }
                    else{
                        analogWrite(luz01Pin, 0);
                        incrementoPWMLuz01 = 0;
                    }                       
                }
            }
        }
        else{
                analogWrite(luz01Pin, 0);
        }

    //Controla a saida de luz 02
        if(configData.luz02Estado && !configData.luz02mode){ //Se o modo estiver manual
            analogWrite(luz02Pin, configData.PWMLuz02);
        }
        else if(configData.luz02Estado && configData.luz02mode){ //Se o modo estiver automatico
            
            horaOn  = configData.timeOnLuz02[0] * 3600 + configData.timeOnLuz02[1] * 60 + configData.timeOnLuz02[2];
            horaOff = configData.timeOffLuz02[0] * 3600 + configData.timeOffLuz02[1] * 60 + configData.timeOffLuz02[2];

            //Intervalo normal / Intervalo normal
            (horaOn <= horaOff) ? dentroIntervalo = (horaAtual >= horaOn && horaAtual <= horaOff) : dentroIntervalo = (horaAtual >= horaOn || horaAtual <= horaOff);

            if(dentroIntervalo){
                  if (millis() - fadeAnteriorLuz02 >= 10) {
                      fadeAnteriorLuz02 = millis();
                          
                      if(incrementoPWMLuz02 <= configData.PWMLuz02){
                          analogWrite(luz02Pin, incrementoPWMLuz02);
                          incrementoPWMLuz02++;
                      }
                      else{
                          analogWrite(luz02Pin, configData.PWMLuz02);
                      }                       
                  }
            }
            else{
                if (millis() - fadeAnteriorLuz02 >= 10){
                    fadeAnteriorLuz02 = millis();
                        
                    if(incrementoPWMLuz02 >= 0){
                        analogWrite(luz02Pin, incrementoPWMLuz02);
                        incrementoPWMLuz02--;
                    }
                    else{
                        analogWrite(luz02Pin, 0);
                        incrementoPWMLuz02 = 0;
                    }                       
                }
            }
        }
        else{
                analogWrite(luz02Pin, 0);
        }
  }