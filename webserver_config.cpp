#include "webserver_config.h"

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

ESP8266WebServer server(80);

const char* host = "aqua-smart-update";

const char* serverIndex = R"rawliteral(
    <!DOCTYPE html>
    <html lang="pt-br">
    <head>
    <meta charset="UTF-8">
    <title>AquaSmart Update</title>
    <style>
    body {
        margin: 0;
        min-height: 100vh;
        display: flex;
        align-items: center;
        justify-content: center;
        background: linear-gradient(135deg, #1e1e2f, #2a2a40);
        font-family: Arial, Helvetica, sans-serif;
        color: #fff;
    }

    .card {
        background: #111827;
        padding: 30px;
        border-radius: 12px;
        width: 90%;
        max-width: 360px;
        box-shadow: 0 10px 30px rgba(0,0,0,.4);
        text-align: center;
    }

    h1 {
        margin: 0 0 5px;
        font-size: 22px;
    }

    p {
        font-size: 13px;
        opacity: .7;
        margin-bottom: 20px;
    }

    .file {
        border: 2px dashed #4f46e5;
        padding: 12px;
        border-radius: 8px;
        cursor: pointer;
        margin-bottom: 15px;
        background: #1f2933;
    }

    input[type="file"] {
        display: none;
    }

    button {
        width: 100%;
        padding: 12px;
        margin-top: 20px;
        border: none;
        border-radius: 8px;
        background: #4f46e5;
        color: #fff;
        font-size: 16px;
        cursor: pointer;
    }

    button:hover {
        background: #4338ca;
    }

    .warn {
        margin-top: 10px;
        font-size: 12px;
        opacity: .6;
    }
    </style>
    </head>

    <body>
    <div class="card">
        <h1>Atualizar Firmware</h1>
        <p>AquaSmart</p>

        <form method="POST" action="/update" enctype="multipart/form-data">
        <label class="file">
            Selecionar arquivo (.bin)
            <input type="file" name="update" required>
        </label>

        <button type="submit">Enviar atualização</button>
        </form>

        <div class="warn">⚠️ Não desligue o dispositivo</div>
    </div>
    </body>
    </html>
    )rawliteral";

void beginUpdate(){

    MDNS.begin(host);
    server.on("/", HTTP_GET, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", serverIndex);
    });
    server.on(
      "/update", HTTP_POST, []() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
        ESP.restart();
      },
      []() {
        HTTPUpload& upload = server.upload();
        if (upload.status == UPLOAD_FILE_START) {
          Serial.setDebugOutput(true);
          WiFiUDP::stopAll();
          Serial.printf("Update: %s\n", upload.filename.c_str());
          Serial.printf("Comecei\n");
          updating();
          uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
          if (!Update.begin(maxSketchSpace)) {  // start with max available size
            Update.printError(Serial);
          }
        } else if (upload.status == UPLOAD_FILE_WRITE) {
          if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
            Update.printError(Serial);
          }
        } else if (upload.status == UPLOAD_FILE_END) {
          if (Update.end(true)) {  // true to set the size to the current progress
            Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
            Serial.printf("Terminei\n");
            display.setCursor(52, 57);
            display.print("100%");
            display.display();
          } else {
            Update.printError(Serial);
          }
          Serial.setDebugOutput(false);
        }
        yield();
      });
    server.begin();
    MDNS.addService("http", "tcp", 80);

    Serial.printf("Ready! Open http://%s.local in your browser\n", host);
    display.setCursor(0, 32);
    display.print("Open http://");
    display.print(host);
    display.print(".local");
    display.setCursor(0, 55);
    display.print("in your browser");
    display.display();
}

void updating(){
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(32, 0);
    display.print("Atualizando");

    display.drawBitmap(40, 11, update_icon, 48, 36, 1); //(coluna, linha, variavel do bitmap, largura, altura, cor)

    display.setCursor(27, 49);
    display.print("Nao desligue!");
    display.display();

}