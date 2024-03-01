boolean printDiagnostics = false;
#define NROWS 6
#define NCOLS 10
#define BUFFER_LEN 120

union CubeData
{
  struct
  {
    int16_t state;
    int16_t watchdog;
    int16_t updateInterval;
    int16_t publishPad;
    int16_t mouseState;
    int16_t mouseWeight;
    int16_t mouseSize;
    int16_t mouseStep;    
  };
  byte buffer[16];
};
CubeData cubeData;

union MousePadData
{
  struct
  {
    int16_t pad[NROWS][NCOLS];
  };
  byte buffer[BUFFER_LEN];
};
MousePadData mousePadData;

#include "BlinkyPicoWCube.h"
#include "MickeyMouse.h";

MickeyMouse mickeyMouse = MickeyMouse(&mousePadData, &cubeData, NROWS, NCOLS);


int commLEDPin = LED_BUILTIN;
int commLEDBright = 255; 
int resetButtonPin = 15;

unsigned long lastPublishTime;
unsigned long lastPadPublishTime;
unsigned long publishInterval = 2000;

void setupServerComm()
{
  // Optional setup to overide defaults
  if (printDiagnostics)
  {
    Serial.begin(115200);
    delay(10000);
  }
  BlinkyPicoWCube.setChattyCathy(printDiagnostics);
  BlinkyPicoWCube.setWifiTimeoutMs(20000);
  BlinkyPicoWCube.setWifiRetryMs(20000);
  BlinkyPicoWCube.setMqttRetryMs(3000);
  BlinkyPicoWCube.setResetTimeoutMs(10000);
  BlinkyPicoWCube.setHdwrWatchdogMs(8000);
  BlinkyPicoWCube.setBlMqttKeepAlive(8);
  BlinkyPicoWCube.setBlMqttSocketTimeout(4);
  BlinkyPicoWCube.setMqttLedFlashMs(10);
  BlinkyPicoWCube.setWirelesBlinkMs(100);
  BlinkyPicoWCube.setMaxNoMqttErrors(5);
  
  // Must be included
  BlinkyPicoWCube.init(commLEDPin, commLEDBright, resetButtonPin);
}

void setupCube()
{
  lastPublishTime = millis();
  lastPadPublishTime = lastPublishTime;
  cubeData.state = 1;
  cubeData.watchdog = 0;
  cubeData.updateInterval = 1000;
  cubeData.publishPad = 0;
  cubeData.mouseState = 0;
  cubeData.mouseWeight = 8192;
  cubeData.mouseSize = 100;
  cubeData.mouseStep = 10;
  mickeyMouse.zero();
  Serial.begin(115200);

}

void cubeLoop()
{
  unsigned long nowTime = millis();
  
  if ((nowTime - lastPublishTime) > publishInterval)
  {
    lastPublishTime = nowTime;
    cubeData.watchdog = cubeData.watchdog + 1;
    if (cubeData.watchdog > 32760) cubeData.watchdog= 0 ;
    BlinkyPicoWCube::publishToServer();
  }  
  
  if ((nowTime - lastPadPublishTime) > cubeData.updateInterval)
  {
    lastPadPublishTime = nowTime;
    if (cubeData.publishPad > 0)
    {
      switch(cubeData.mouseState)
      {
        case 0:
          mickeyMouse.zero();
          break;
        case 1:
          mickeyMouse.flat();
          break;
        case 2:
          mickeyMouse.randomWalk();
          break;
        case 3:
          mickeyMouse.ramp();
          break;
        default:
          break;
      }
      Serial.write(mousePadData.buffer, BUFFER_LEN);      
    }
  }  
}

void handleNewSettingFromServer(uint8_t address)
{
  switch(address)
  {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      mickeyMouse.zero();
      break;
    case 5:
      break;
    default:
      break;
  }
}
