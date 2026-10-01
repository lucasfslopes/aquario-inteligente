#include "portas.h"
#include "global_lib.h"
#include "eeprom_config.h"
#include "display_config.h"
#include "wifi_config.h"
#include "webserver_config.h"
#include "bitmaps.h"
#include "ir_config.h"
#include "temp_config.h"
#include "buzzer_sons.h"
#include "telas.h"
#include "menu.h"
#include "teclado.h"
#include "msg_erros.h"

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