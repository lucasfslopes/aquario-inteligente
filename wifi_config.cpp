#include "wifi_config.h"

void wificonnect() {
  
    display.clearDisplay();  
    display.setTextColor(SH110X_WHITE);
    display.setTextSize(1); // scale text
    display.setCursor(0, 0);
    display.print("Conectando WI-FI...");
    display.display();

    wm.setConfigPortalTimeout(5); //Tempo em segundos que o wifi tenta se conectar antes de continuar o codigo

    bool res;
      res = wm.autoConnect(wifi_ssid , wifi_password); // password protected ap

    if(!res) {
        display.clearDisplay();  
        display.setTextColor(SH110X_WHITE);
        display.setTextSize(1); // scale text
        display.setCursor(5, 17);
        display.print("Nenhuma rede WI-FI");
        display.setCursor(5, 27);
        display.print("foi conectada.");
        display.display();
        delay(1000);
        display.setCursor(5, 0);
        display.print("Iniciano Sistema...");
        display.display();
        wm.setConfigPortalTimeout(30);
        estadoWiFi = 0;          
        delay(2000);
        
        // ESP.restart();
    } 
    else {
        display.clearDisplay();
        display.setTextColor(SH110X_WHITE);
        display.setTextSize(1); // scale text
        display.setCursor(40, 17);
        display.print("Conectado!");
        display.display();
        delay(1000);
        display.setCursor(5, 0);
        display.print("Iniciano Sistema...");
        display.display();
        estadoWiFi = 1;
        delay(1000);
        
    }
}

void configWiFi() {
  
        display.clearDisplay();  
        display.setTextColor(SH110X_WHITE);
        display.setTextSize(1); // scale text
        display.setCursor(50, 0);
        display.print("AP Criado!");
        display.setCursor(5, 40);
        display.print("SSID: " + String(wifi_ssid));
        display.setCursor(5, 55);
        display.print("Password: " + String(wifi_password));
        display.display();
        delay(1000);

  wm.setConfigPortalTimeout(120);
  bool res;
  
  res = wm.startConfigPortal(wifi_ssid , wifi_password); // password protected ap

    if(!res) {
        display.clearDisplay();  
        display.setTextColor(SH110X_WHITE);
        display.setTextSize(1); // scale text
        display.setCursor(5, 17);
        display.print("Nenhuma rede WI-FI");
        display.setCursor(5, 27);
        display.print("foi conectada.");
        display.display();
        delay(1000);
        display.setCursor(5, 0);
        display.print("Continuando...");
        display.display();
        estadoWiFi = 0;
        delay(2000);
        
        // ESP.restart();
    } 
    else {
        display.clearDisplay();  
        display.setTextColor(SH110X_WHITE);
        display.setTextSize(1); // scale text
        display.setCursor(40, 17);
        display.print("Conectado!");
        display.display();
        delay(1000);
        display.setCursor(40, 27);
        display.print("Pressione voltar");
        display.display();
        estadoWiFi = 1;
        delay(1000);
       
    }
}

void resetWiFi() {
    
    wm.resetSettings(); //Codigo para resetar o acesso ao wifi

}