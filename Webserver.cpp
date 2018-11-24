//
// Webserver.cpp - Library for wrapping the ESP8266 Web Server
//                 also manages the SPIFFS files
//

#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <FS.h>

#include "Webserver.h"
#include "defaults.h"

ESP8266WebServer         server(8080);   // Set the HTTP Server port here


Webserver::Webserver() {
  SPIFFS.begin();
  Serial.println("File System Initialized");
}


void Webserver::begin( Config *config, LED *led ) {
  _config = config;  // Keep a reference to the config
  _led    = led;     // Keep a reference to the LED controller

  // See if we can find the version of the SPIFFS that we're running
  _spiffs_version = get_spiffs_version();

  _fw_check_interval = FW_CHECK_INTERVAL * 1000;
  _next_fw_check = millis() + _fw_check_interval;

  Serial.println( "[Webserver] HTTP INIT, hostname: " + String( _config->conf.hostname ) + "  port: " + String( _config->conf.http_server_port ) );
  Serial.println( "SPIFFS Version: " + _spiffs_version );
  

//  MDNS.begin( conf.hostname );

  // HTTP callbacks bound to class member functions
  server.on("/",          HTTP_GET,  std::bind(&Webserver::handleWebRequests, this));
  server.on("/display",   HTTP_GET,  std::bind(&Webserver::setDisplay, this));
  server.on("/reset",     HTTP_POST, std::bind(&Webserver::processConfigReset, this));
  server.on("/settings",  HTTP_POST, std::bind(&Webserver::processSettings, this));
  server.on("/webupdate", HTTP_POST, std::bind(&Webserver::runWebUpdate, this));

//  server.on("/api/settings/network", HTTP_OPTIONS, std::bind(&Webserver::sendCORS, this));
  server.on("/api/settings/network", HTTP_GET,  std::bind(&Webserver::getNetworkSettings, this));
  server.on("/api/settings/network", HTTP_POST, std::bind(&Webserver::setNetworkSettings, this));

  server.on("/api/led", HTTP_POST, std::bind(&Webserver::setLEDs, this));

//  server.on("/api/led/display", HTTP_OPTIONS, std::bind(&Webserver::sendCORS, this));
  server.on("/api/led/display", HTTP_GET, std::bind(&Webserver::getDisplay, this));
  server.on("/api/led/display", HTTP_POST, std::bind(&Webserver::setDisplay, this));

  server.on("/api/led/palettes", HTTP_GET, std::bind(&Webserver::getPalettes, this));
  server.on("/api/led/palette", HTTP_POST, std::bind(&Webserver::savePalette, this));

  server.onNotFound(std::bind( &Webserver::handleWebRequests, this));

  // Attach the OTA update service
  _httpUpdater.setup(&server, HTTP_OTA_UPDATE_PATH, HTTP_AUTH_USER, _config->conf.http_pw);
  
  server.begin();
}


void Webserver::loop() {
  // Handle any HTTP Requests
  server.handleClient();

  if (millis() > _next_fw_check) {   // Time to check for updated files
    _next_fw_check = millis() + _fw_check_interval;
    check_for_spiffs_update();
  }
}


void Webserver::check_for_fwupdate() {
  Serial.println("[Webserver] checking for Firmware updates");

  t_httpUpdate_return ret = ESPhttpUpdate.update(UPDATE_URL, INO_VERSION);
//  ret = ESPhttpUpdate.updateSpiffs(UPDATE_URL, INO_VERSION);

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("[Webserver] HTTP update failed.  Error (%d): %s\n", 
          ESPhttpUpdate.getLastError(),
          ESPhttpUpdate.getLastErrorString().c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("[Webserver] No remote update found.");
      break;

    case HTTP_UPDATE_OK:
      Serial.println("[Webserver] Remote Firmware update succeeded!");
      break;
  }

}


String Webserver::get_spiffs_version() {
  File versionFile = SPIFFS.open("/version.txt", "r");

  if (!versionFile) {
    Serial.println("[Webserver] Failed to open /version.txt, unknown SPIFFS version!");
    return String("");
  }

  if (versionFile.size() > 25) {
    Serial.println("[Webserver] /version.txt is too large!");
    return String("");
  }

  return versionFile.readString();
  
}

void Webserver::check_for_spiffs_update() {
  Serial.println("[Webserver] checking for SPIFFS updates");

  t_httpUpdate_return ret = ESPhttpUpdate.updateSpiffs(UPDATE_URL, String(INO_VERSION) + ":" + _spiffs_version);
//  ret = ESPhttpUpdate.updateSpiffs(UPDATE_URL, INO_VERSION);

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("[Webserver] HTTP SPIFFS update failed.  Error (%d): %s\n", 
          ESPhttpUpdate.getLastError(),
          ESPhttpUpdate.getLastErrorString().c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("[Webserver] No remote SPIFFS update found.");
      break;

    case HTTP_UPDATE_OK:
      Serial.println("[Webserver] Remote SPIFFS update succeeded!");
      break;
  }

}


void Webserver::handleWebRequests() {
  Serial.println( "[Webserver] handleWebRequests: " + server.uri() );

  if ( server.method() == HTTP_OPTIONS ) {
    sendCORS();
    return;
  }

  if ( authRequired() ) return;  // Page requires authentication

  if ( loadFromSpiffs( server.uri() ) ) return;
  
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send( 404, "text/plain", message );
  Serial.println( message );
}


// Respond to a web client request
void Webserver::httpReturn(uint16_t httpcode, String mimetype, String content) {
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(httpcode, mimetype, content);
}


// Handle loading a file from the local file system (SPIFFS)
bool Webserver::loadFromSpiffs( String path ){
  String dataType = "text/plain";
  if (path.endsWith("/")) path += "index.html";
 
  if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".html")) dataType = "text/html";
  else if (path.endsWith(".htm")) dataType = "text/html";
  else if (path.endsWith(".css")) dataType = "text/css";
  else if (path.endsWith(".js")) dataType = "application/javascript";
  else if (path.endsWith(".png")) dataType = "image/png";
  else if (path.endsWith(".gif")) dataType = "image/gif";
  else if (path.endsWith(".jpg")) dataType = "image/jpeg";
  else if (path.endsWith(".ico")) dataType = "image/x-icon";
  else if (path.endsWith(".xml")) dataType = "text/xml";
  else if (path.endsWith(".pdf")) dataType = "application/pdf";
  else if (path.endsWith(".zip")) dataType = "application/zip";

  Serial.println("[Webserver] loadFromSpiffs path:" + path + " dataType: " + dataType);
  
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (server.hasArg("download")) dataType = "application/octet-stream";
  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
    Serial.println("[Webserver] loadFromSpiffs != dataFile.size()" );
  }
 
  dataFile.close();
  return true;
}



bool Webserver::authRequired() {
  if ( !server.authenticate( HTTP_AUTH_USER, _config->conf.http_pw ) ) {
    server.requestAuthentication( DIGEST_AUTH, auth_realm, auth_fail_response );
    return true;
  }
  return false;
}


// OPTIONS - Send CORS headers
void Webserver::sendCORS() {
  server.sendHeader( "Access-Control-Allow-Origin", "*" );
  server.sendHeader( "Access-Control-Allow-Credentials", "false");
  server.sendHeader( "Access-Control-Allow-Headers", "x-requested-with,authorization,content-type" );
  server.sendHeader( "Access-Control-Allow-Methods", "GET,OPTIONS,POST" );

  server.send(204);
}

// POST /reset
void Webserver::processConfigReset() {
  if ( authRequired() ) return;  // Page requires authentication

  // Delay to give the user a chance to pull the plug
  for (int x=5; x>0; x--) {
    Serial.println("[RESET REQUEST] Clearing config in " + String(x) + "...Reset to stop");
    delay(1000);
  }

  // Write defaults to the config
  _config->resetConfig();
  
  httpReturn(200, "application/json", "{\"status\": \"ok\"}");

  // Restart
  ESP.restart();
}


// POST /settings
void Webserver::processSettings() {
  if ( authRequired() ) return;  // Page requires authentication

  // Update config with values from form
  if ( server.hasArg("location") )       _config->set( CONFIG_LOCATION,        server.arg("location") );
  if ( server.hasArg("http_pw") )        _config->set( CONFIG_HTTP_PW,         server.arg("http_pw") );

  _config->writeConfig();

  // Success to the client.
  httpReturn(200, "application/json", "{\"status\": \"ok\"}");
}


// GET /api/settings/network
// Return a JSON string of the network settings
void Webserver::getNetworkSettings() {
  if ( authRequired() ) return;  // Page requires authentication
    
  String jsonstr = _config->NetworkJSON( String(WiFi.macAddress()) );
  
  httpReturn(200, "application/json", jsonstr);
}


// POST /network
void Webserver::setNetworkSettings() {
  if ( authRequired() ) return;  // Page requires authentication

  if (server.args() < 1) {
    httpReturn(400, "text/html", "Missing Data");
    return;
  }

  // Update config with values from form
  if ( server.arg("ssid") )
    _config->set( CONFIG_SSID, server.arg("ssid") );
  if ( server.arg("hostname") )
    _config->set( CONFIG_HOSTNAME, server.arg("hostname") );

  // Only change the wifi password if one was passed
  if ( server.arg("wifi_pw").length() > 0 )
    _config->set( CONFIG_WIFI_PW, server.arg("wifi_pw") );

  // Success to the client.
  httpReturn( 200, "application/json", "{\"status\": \"ok\"}" );

  // Save the running config
  _config->writeConfig();

  // Wait a bit before disconnecting/reconnecting
  delay(2000);

  // Disconnect the wifi and re-run the setup routine
  WiFi.disconnect( true );
  setup();
}


// GET /display
void Webserver::getDisplay() {
  if ( authRequired() ) return;  // Page requires authentication
  
  httpReturn(200, "application/json", _led->getJSON());
}


// POST /led/display
void Webserver::setDisplay() {
    if ( authRequired() ) return;  // Page requires authentication

    _led->setDisplay(server.arg("mode").toInt(), server.arg("duration").toInt());

    // Success to the client.
    httpReturn( 200, "application/json", "{\"status\": \"ok\"}" );
}

// POST /led
void Webserver::setLEDs() {
    if ( authRequired() ) return;  // Page requires authentication

    _led->setDelay( server.arg("delay").toInt() );
    _led->setBrightness( server.arg( "brightness" ).toInt() );

    // Success to the client.
    httpReturn( 200, "application/json", "{\"status\": \"ok\"}" );
}


// GET /led/palettes
void Webserver::getPalettes() {
    Dir dir = SPIFFS.openDir( PALETTE_DIR );
    String jsonstr = "";
    
    while (dir.next()) {
        String fn = dir.fileName();
        fn = fn.substring( sizeof(PALETTE_DIR) );
        
        Serial.println( "found palette file: " + fn );
        jsonstr += ", \"" + fn + "\"";
    }

    if (jsonstr.length() > 1)
        jsonstr = jsonstr.substring(2);

    jsonstr = "{\"status\": \"ok\", \"palettes\": [ " + jsonstr + " ] }";
    
    httpReturn( 200, "application/json", "{\"status\": \"ok\", \"palettes\": " + jsonstr + " }" );
   
}

// POST /led/palette
// Stores a custom palette via SPIFFS
void Webserver::savePalette() {
    String palette_fn = String(PALETTE_DIR) + "/" + String( server.arg("name") );
    File f = SPIFFS.open( palette_fn, "w" );
    if (!f) {
        Serial.println( "[Webserver] savePalette: failed to open palette file - " + palette_fn );
        httpReturn( 500, "application/json", "{\"status\": \"ok\"}" );
        return;
    }

    Serial.println( "[Webserver] savePalette: saving to - " + palette_fn );
    for (uint8_t i=0; i < server.args(); i++) {
        if ( server.argName(i) == "palette" ) {
            Serial.println("[WebServer] savePalette: " + String(server.argName(i)) + " - " + String(server.arg(i)) );
            f.println( server.arg(i) );
        }
    }
    f.close();

    // Reload the custom palettes
    _led->loadCustomPalettes();
    
    // Success to the client.
    httpReturn( 200, "application/json", "{\"status\": \"ok\"}" );
}

void Webserver::runWebUpdate() {
}


