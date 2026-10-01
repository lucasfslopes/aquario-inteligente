#include "eeprom_config.h"
#include <EEPROM.h>

//Variaveis salvas na EEPROM

struct structConfig {

    bool feed01;
    bool feed02;
    bool luz01Estado;
    bool luz02Estado;
    bool luz01mode;
    bool luz02mode;
    int timeOnLuz01[3];
    int timeOffLuz01[3];
    int timeOnLuz02[3];
    int timeOffLuz02[3];
    int timeFeed01[3];
    int timeFeed02[3];
    int luz01Penumbra;
    int luz02Penumbra;
    float quantidadeFeed;
    float PWMFan;
    float PWMLuz01;
    float PWMLuz02;
    bool alarme;
    int intervaloAlarme;
    float tempMin;
    float tempMax;
};

structConfig configData;

void configSave(){

    (!configData.luz01mode) ? configData.PWMLuz01 = 0 : 0;
    (!configData.luz02mode) ? configData.PWMLuz02 = 0 : 0;

    // Grava a estrutura inteira
    EEPROM.put(0, configData);
    // Commit é obrigatório para salvar na flash
    EEPROM.commit();
  }

void configReset(){
    
    configData.feed01 = 0;
    configData.feed02 = 0;
    configData.luz01mode = 0;
    configData.luz02mode = 0;
    configData.timeOnLuz01[0] = 0;
    configData.timeOnLuz01[1] = 0;
    configData.timeOnLuz01[2] = 0;
    configData.timeOffLuz01[0] = 0;
    configData.timeOffLuz01[1] = 0;
    configData.timeOffLuz01[2] = 0;
    configData.timeOnLuz02[0] = 0;
    configData.timeOnLuz02[1] = 0;
    configData.timeOnLuz02[2] = 0;
    configData.timeOffLuz02[0] = 0;
    configData.timeOffLuz02[1] = 0;
    configData.timeOffLuz02[2] = 0;
    configData.timeFeed01[0] = 0;
    configData.timeFeed01[1] = 0;
    configData.timeFeed01[2] = 0;
    configData.timeFeed02[0] = 0;
    configData.timeFeed02[1] = 0;
    configData.timeFeed02[2] = 0;
    configData.luz01Penumbra = 0;
    configData.luz02Penumbra = 0;
    configData.quantidadeFeed = 0;
    configData.PWMFan = 0;
    configData.PWMLuz01 = 0;
    configData.PWMLuz02 = 0;
    configData.alarme = 0;
    configData.intervaloAlarme = 0;
    configData.tempMin = 0;
    configData.tempMax = 0;

    configSave();
  }