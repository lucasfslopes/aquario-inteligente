#include <OneWire.h>
#include <DallasTemperature.h>

//Sensor Temperatura

    OneWire oneWire(ONE_WIRE_BUS); //Sensor Temperatura - Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
    DallasTemperature sensors(&oneWire); //Pass our oneWire reference to Dallas Temperature.