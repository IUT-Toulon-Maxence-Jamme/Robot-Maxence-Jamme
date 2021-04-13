/* 
 * File:   CB_RX1.h
 * Author: GEII Robot
 *
 * Created on 13 avril 2021, 16:13
 */

#ifndef CB_RX1_H
#define	CB_RX1_H
void CB_RX1_Add(unsigned char value);
unsigned char CB_RX1_Get(void);
unsigned char CB_RX1_IsDataAvailable(void);
int CB_RX1_GetRemainingSize(void);
int CB_RX1_GetDataSize(void);

#endif	/* CB_RX1_H */

