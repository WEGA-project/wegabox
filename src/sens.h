#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 23
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);