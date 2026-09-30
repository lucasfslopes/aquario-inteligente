#ifndef WEBSERVER_CONFIG_H
#define WEBSERVER_CONFIG_H

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* host = "aqua-smart-update";

ESP8266WebServer server(80);

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

    void beginUpdate();

    void updating();

#endif