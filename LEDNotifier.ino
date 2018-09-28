#include "Config.h"
#include "Network.h"
#include "Webserver.h"
#include "LED.h"
#include "defaults.h"

Config config;
Network net;
Webserver web;
LED led;



void setup() {
    delay( 3000 ); // power-up safety delay

    // Start serial
    Serial.begin(115200);
    Serial.println();
    
    Serial.println("-------------------------");
    Serial.println(" L E D   N O T I F I E R ");
    Serial.println("-------------------------");
    Serial.println();
    
    // Initialize Network/WiFi
    net.begin( &config );
    
    // Initialize File System and Web Server
    web.begin( &config, &led );

    // Initialize the LED controller
    led.begin( &config );
    
    delay(500);
    
}


void loop()
{
    net.loop();
    web.loop();
    led.loop();
}

