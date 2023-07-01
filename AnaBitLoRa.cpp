/**
* @Autor : St3v3n-4n4
* @Modificacion : 20-06-2023
* @Commit : Libreria "AnaBitLoRa" 
*           para el uso del modulo RHF76-052DM
*           con AnaBit
**/

#include "Arduino.h"
#include <SoftwareSerial.h>
#include "AnaBitLoRa.h"

// pines de la comunicacion UART virtual para LoRa
SoftwareSerial SLora(32,33);//RX,TX

AnaBitLoRa::AnaBitLoRa(int F, int SF, int BW, int PL, int P,bool DEBUG)
{
    _F = F;
    _SF = SF;
    _BW = BW;
    _PL = PL;
    _Power = P;
    _debug = DEBUG;
}

void AnaBitLoRa::Frecuencia(int F)
{
    _F = F;
}

void AnaBitLoRa::escucha(int time)
{
    delay(time);
    while(SLora.available())
    {
        if(_debug)
        {
            Serial.println(SLora.readString());
        }
        else
        {
            SLora.readString();
        }
    }
    
}

void AnaBitLoRa::start(int baud)
{
    // configuracion de velocidad del puerto serial del modulo de LoRa
    SLora.begin(baud);
    // preparacion del modulo para su configuracion y uso
    SLora.println("AT");
    escucha(10);
    SLora.println("AT+RESET");
    escucha(10);
    SLora.println("AT");
    escucha(10);
}

void AnaBitLoRa::Modo(String modo)
{
    // Configuracion del modo de uso del modulo
    SLora.println("AT+Mode="+modo);
    escucha(100);
}

void AnaBitLoRa::PtPrx()
{
    // se configura el radio para la recepcion de datos
    SLora.println("AT+TEST=RFCFG,"+String(_F)+","+String(_SF)+","+String(_BW)+","+String(_PL)+","+String(_PL)+","+String(_Power));
    escucha(100);
    // Activacion para recibir datos
    SLora.println("AT+TEST=RXLRPKT");
    escucha(100);
}

String AnaBitLoRa::readString()
{
    // Recepcion de datos enviados por el modulo
    _DLoRa = SLora.readString();
    return _DLoRa;
}

String AnaBitLoRa::msgRX(String datos,String modo)
{
    if(datos.indexOf("+TEST: RX ")>=0)
    {
        int index = datos.lastIndexOf(' ');
        int length = datos.length();

        String RXHEX = datos.substring(index,length);

        String HEXmsg = "";
        for (int i = 0; i < RXHEX.length(); i ++) 
        {
            if(RXHEX[i]!=' ' && RXHEX[i]!='"')
            {
                HEXmsg = HEXmsg + RXHEX[i];
            }
        }

        if(modo == "HEX")
        {
            return HEXmsg;
        }

        String ASCIImsg = "";
        for (int i = 0; i < HEXmsg.length(); i += 2) 
        {
            String hexByte = HEXmsg.substring(i, i + 2);
            char asciiChar = strtol(hexByte.c_str(), NULL, 16);
            ASCIImsg += asciiChar;
        }
        if(modo == "ASCII")
        {
            return ASCIImsg;
        }
        
    }
    return "";
}

bool AnaBitLoRa::available()
{
    if(SLora.available())
    {
        return true;
    }
    else
    {
        return false;
    }
}



void AnaBitLoRa::PtPtxSTR(String Mensage, int cont)
{
    // se configura el radio para la transmision de datos
    SLora.println("AT+TEST=RFCFG,"+String(_F)+","+String(_SF)+","+String(_BW)+","+String(_PL)+","+String(_PL)+","+String(_Power));
    escucha(100);
    for (int i = 0; i < cont; i ++)
    {
        // Envio de mensajes tipo cadena
        SLora.println("AT+TEST=TXLRSTR,"+Mensage);
        escucha(100);
    } 
    
}