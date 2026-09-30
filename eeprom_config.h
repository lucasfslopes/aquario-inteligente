#ifndef EEPROM_CONFIG_H
#define EEPROM_CONFIG_H

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

void configSave();

void configReset();

#endif