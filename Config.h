//
// config.h - Library for handling storing and retrieving 
//            config settings from EEPROM
//

#ifndef Config_h
#define Config_h

#include "Arduino.h"

#define CONFIG_VERSION           1
#define EEPROM_SIZE              512
#define EEPROM_CONFIG_START      0

#define DEFAULT_HOSTNAME         "led-notifier"
#define DEFAULT_HTTP_PORT        8080
#define DEFAULT_SAMPLE_INTERVAL  60
#define DEFAULT_SSID             "SightUnseenFarm"
#define DEFAULT_WIFI_PW          "PASSWORD"        // Also the default AP password
#define DEFAULT_HTTP_PW          "admin"
#define DEFAULT_T_OFFSET         0

#define CONFIG_HOSTNAME        1
#define CONFIG_LOCATION        2
#define CONFIG_HTTP_PW         3
#define CONFIG_SSID            10
#define CONFIG_WIFI_PW         11

#define MAX_HOSTNAME  20
#define MAX_LOCATION  20
#define MAX_HTTP_PW   10
#define MAX_SSID      32
#define MAX_WIFI_PW   32


//
// EEPROM Configuration Structure
struct configuration {
  unsigned int version;

  // System Settings (and +1 for the \0)
  char hostname[ MAX_HOSTNAME+1 ];
  char location[ MAX_LOCATION+1 ];
  unsigned short http_server_port;
  char http_pw[ MAX_HTTP_PW+1 ];

  // Network Settings
  char ssid[ MAX_SSID+1 ];
  char wifi_pw[ MAX_WIFI_PW+1 ];
};


//
// Config Library Class
class Config
{
  public:
    Config();
    void readConfig();
    bool set( int key, String value );
    void writeConfig();
    void resetConfig();

    void reboot();

    String NetworkJSON(String macaddr);

    configuration conf;


  private:
    configuration _defaults = { CONFIG_VERSION, DEFAULT_HOSTNAME, "unknown", DEFAULT_HTTP_PORT, DEFAULT_HTTP_PW,
                                DEFAULT_SSID, DEFAULT_WIFI_PW,
                              };

};


#endif
