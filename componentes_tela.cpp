void seletor(int menuPosition, String arrow_text) {
    
    int arrow_pos = ((menuPosition - 1) % 5) + 1;
    int max_arrow_pos = 5;
    int w = 110;
    int h = 9;
    int r = 4;

    if (arrow_pos > max_arrow_pos) {
        arrow_pos = 1;
    }
    else if (arrow_pos < 1 ) {
        arrow_pos = 5;
    }
    else {
        if (arrow_pos == 1) {
            
            int x = 5;
            int y = 16;
          
            display.fillRoundRect(x, y, w, h, r, SH110X_WHITE);
            //display.fillRect(x, y, w, h, SH110X_WHITE);
            display.setTextSize(1); // Normal 1:1 pixel scale
            display.setTextColor(SH110X_BLACK);
            display.setCursor(15, 17);
            display.print(arrow_text);   
        }
        else if (arrow_pos == 2) {
          
          int x = 5;
          int y = 26; 
            
          display.fillRoundRect(x, y, w, h, r, SH110X_WHITE);
          //display.fillRect(x, y, w, h, SH110X_WHITE);
          display.setTextSize(1);      // Normal 1:1 pixel scale
          display.setTextColor(SH110X_BLACK);
          display.setCursor(15, 27);
          display.print(arrow_text);

        }
        else if (arrow_pos == 3) {
          
          int x = 5;
          int y = 36;

          display.fillRoundRect(x, y, w, h, r, SH110X_WHITE);
          //display.fillRect(x, y, w, h, SH110X_WHITE);
          display.setTextSize(1);      // Normal 1:1 pixel scale
          display.setTextColor(SH110X_BLACK);
          display.setCursor(15, 37);
          display.print(arrow_text);
        
        }
        else if (arrow_pos == 4) {
          
          int x = 5;
          int y = 46;
          
          display.fillRoundRect(x, y, w, h, r, SH110X_WHITE);
          //display.fillRect(x, y, w, h, SH110X_WHITE);
          display.setTextSize(1);      // Normal 1:1 pixel scale
          display.setTextColor(SH110X_BLACK);
          display.setCursor(15, 47);
          display.print(arrow_text);
        
        }        
        else {
          
          int x = 5;
          int y = 55;
          
          display.fillRoundRect(x, y, w, h, r, SH110X_WHITE);
          //display.fillRect(x, y, w, h, SH110X_WHITE);
          display.setTextSize(1);      // Normal 1:1 pixel scale
          display.setTextColor(SH110X_BLACK);
          display.setCursor(15, 56);
          display.print(arrow_text);
        
        }
    }
}

void barra_superior () {
  
    String strHora;
    String strMin;  
    String strSeg;

    tempoatual = millis();

    if (tempoatual - timerequest >= 1000) {
        DateTime now = rtc.now();
        hora[0] = now.hour(), DEC;
        hora[1] = now.minute(), DEC;
        hora[2] = now.second(), DEC;
        timerequest = tempoatual;        
    }

    (hora[0] >= 0 && hora[0] <=9) ? strHora = ("0" + String(hora[0])) : strHora = String(hora[0]);
    (hora[1] >= 0 && hora[1] <=9) ? strMin = ("0" + String(hora[1])) : strMin = String(hora[1]);
    (hora[2] >= 0 && hora[2] <=9) ? strSeg = ("0" + String(hora[2])) : strSeg = String(hora[2]);
            
    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.print(strHora + ":" + strMin + ":" + strSeg);
  
    if(estadoWiFi){display.drawBitmap(112, 0, wifi_icon, 16, 16, 1);}
}

void moldura() {
    display.drawRoundRect(0, 16, 128, 48, 3, SH110X_WHITE);
    //display.drawRect(0 , 16 , 128 , 16 , SSD1306_WHITE);
}

void barravertical(int position) {
    
    display.drawLine(125, 16, 125, 64, SH110X_WHITE);

    if (position == 1) {
      display.drawRoundRect(124, 18, 3, 8, 2, SH110X_WHITE);
    }
    else if (position == 2) {
      display.drawRoundRect(124, 24, 3, 8, 2, SH110X_WHITE);
    }
    else if (position == 3) {
      display.drawRoundRect(124, 31, 3, 8, 2, SH110X_WHITE);
    }
    else if (position == 4) {
      display.drawRoundRect(124, 36, 3, 8, 2, SH110X_WHITE);
    }
    else if (position == 5) {
      display.drawRoundRect(124, 42, 3, 8, 2, SH110X_WHITE);
    }
    else if (position == 6) {
      display.drawRoundRect(124, 48, 3, 8, 2, SH110X_WHITE);
    }
    else if (position == 7) {
      display.drawRoundRect(124, 54, 3, 8, 2, SH110X_WHITE);
    }

    display.display();
}

void animacaotransicao() {
      
    display.clearDisplay();

    for(int16_t i=0; i<display.height()/2-2; i+=2) {
        display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, SH110X_WHITE);
        display.display();
        delay(1);
    }
}