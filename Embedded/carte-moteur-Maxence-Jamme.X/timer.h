#ifndef TIMER_H
#define	TIMER_H
#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1
void InitTimer23(void);
void InitTimer1(void);
void SetFreqTimer1(float freq);
void InitTimer4(void);
void SetFreqTimer4(float freq);
extern unsigned long timestamp ;
#endif	/* TIMER_H */