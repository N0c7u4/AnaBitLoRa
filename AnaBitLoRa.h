/**
* @Autor : St3v3n-4n4
* @Modificacion : 20-06-2023
* @Commit : Libreria "AnaBitLoRa" 
*           para el uso del modulo RHF76-052DM
*           con AnaBit
**/
#ifndef AnaBitLoRa_h
#define AnaBitLoRa_h
 
#include "Arduino.h"
#include <SoftwareSerial.h>

class AnaBitLoRa
{
    public:
        AnaBitLoRa(int F, int SF, int BW, int PL, int P, bool DEBUG);
        void start(int baud);
        void Modo(String modo);
        void PtPrx();
        String readString();
        void PtPtxSTR(String Mensage, int cont);
        bool available();
        String msgRX(String datos,String modo);
        void Frecuencia(int F);
    private:
        int _F;
        int _SF;
        int _BW;
        int _PL;
        int _Power;
        bool _debug;
        String _DLoRa;
        void escucha(int time);
};

#endif