#include <xc.h>
#include "UART_Protocol.h"
#include "CB_TX1.h"
#include "main.h"
#include "IO.h"

unsigned char UartCalculateChecksum(int msgFunction,int msgPayloadLength, unsigned char * msgPayload){
//Fonction prenant entr�e la trame et sa longueur pour calculer le checksum
    unsigned char Checksum = 0;
    Checksum ^= (0xFE);
    Checksum ^= (msgFunction >> 8);
    Checksum ^= (msgFunction >> 0);
    Checksum ^= (msgPayloadLength >> 8);
    Checksum ^= (msgPayloadLength >> 0);
    int i = 0;
    for (i = 0; i < msgPayloadLength; i++)
    {
        Checksum ^= msgPayload[i];
    }            
    return Checksum;
}

void UartEncodeAndSendMessage(int msgFunction,int msgPayloadLength, unsigned char* msgPayload){
//Fonction d?encodage et d?envoi d?un message
    unsigned char Checksum = 0;            
    unsigned char trame[msgPayloadLength+6];
    int pos = 0;
    trame[pos++] = (0xFE);
    trame[pos++] = (msgFunction >> 8);
    trame[pos++] = (msgFunction >> 0);
    trame[pos++] = (msgPayloadLength >> 8);
    trame[pos++] = (msgPayloadLength >> 0);
    int i = 0;
    for (i = 0; i < msgPayloadLength; i++)
    {
        trame[pos++] =  msgPayload[i];
    }
    Checksum = UartCalculateChecksum(msgFunction, msgPayloadLength, msgPayload);

    trame[pos++] = (Checksum);
    SendMessage(trame, msgPayloadLength+6);
}

int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;
int rcvState = 0;
unsigned char calculatedChecksum = 0;
void UartDecodeMessage(unsigned char c){
//Fonction prenant en entr�e un octet et servant � reconstituer les trames
    switch (rcvState)
    {
        case StateReceptionWaiting:
            if(c == 0xFE)
            {
                rcvState = StateReceptionFunctionMSB;
                msgDecodedPayloadLength = 0;
                msgDecodedFunction = 0;
                msgDecodedPayloadIndex = 0;
            }
            break;
        case StateReceptionFunctionMSB:
            msgDecodedFunction = (c<<8);
            rcvState = StateReceptionFunctionLSB;
            break;
        case StateReceptionFunctionLSB:
            msgDecodedFunction += c;
            rcvState = StateReceptionPayloadLengthMSB;
            break;
        case StateReceptionPayloadLengthMSB:
            msgDecodedPayloadLength = (c<<8);
            rcvState = StateReceptionPayloadLengthLSB;
            break;
        case StateReceptionPayloadLengthLSB:
            msgDecodedPayloadLength += c;
            if (msgDecodedPayloadLength > 1500)
            {
                rcvState = StateReceptionWaiting;
            }
            rcvState = StateReceptionPayload;
            break;
        case StateReceptionPayload:
            msgDecodedPayload[msgDecodedPayloadIndex] = c;
            msgDecodedPayloadIndex++;
            if(msgDecodedPayloadIndex == msgDecodedPayloadLength)
            {
                rcvState = StateReceptionCheckSum;
            }                
        break;
        case StateReceptionCheckSum:            
            calculatedChecksum = c;
            if (calculatedChecksum == c){ //isok�
                //TextBoxReception.Text += "youpi\n";
                //ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                //SendMessage( (unsigned char *) "1234567" , 7 );
                //UartEncodeAndSendMessage(msgDecodedFunction,msgDecodedPayloadLength,msgDecodedPayload);
                UartProcessDecodedMessage(msgDecodedFunction,msgDecodedPayloadLength,msgDecodedPayload);
            }
            else
            {       //pas OK
                //TextBoxReceptionText += "snif";
                SendMessage( (unsigned char *) "7654321" , 7 ) ;
            }
            rcvState = StateReceptionWaiting;
            break;
        default:
            rcvState = StateReceptionWaiting;
        break;
    }
}

void UartProcessDecodedMessage(unsigned char msgFunction,unsigned char msgpayloadLength, unsigned char* msgPayload){
//Fonction appel�e apr�s le d�codage pour ex�cuter l?action
//correspondant au message re�u

    switch (msgFunction){
        case SET_ROBOT_STATE:
            SetRobotState(msgPayload[0]);
        break;
        case SET_ROBOT_MANUAL_CONTROL:
            SetRobotAutoControlState(msgPayload[0]);
        break;
        case Function_Led:
            if(msgPayload[0]==0x49 && msgPayload[1]==0x31){
                LED_ORANGE = 1;
            }                
            if(msgPayload[0]==0x4F && msgPayload[1]==0x31){
                LED_ORANGE = 0;
            }
            if(msgPayload[0]==0x49 && msgPayload[1]==0x32){
                LED_BLANCHE = 1;
            }                
            if(msgPayload[0]==0x4F && msgPayload[1]==0x32){
                LED_BLANCHE = 0;
            }
            if(msgPayload[0]==0x49 && msgPayload[1]==0x33){
                LED_BLEUE = 1;
            }                
            if(msgPayload[0]==0x4F && msgPayload[1]==0x33){
                LED_BLEUE = 0;
            }
        break;
        default:
        break;    
    }
}
void SetRobotState (unsigned char c){
    
}

void SetRobotAutoControlState (unsigned char c){
    
}
//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/