#ifndef POWER_CONTROLLER_UTILS_H
#define POWER_CONTROLLER_UTILS_H

// ATtiny85 pinout according to https://github.com/SpenceKonde/ATTinyCore

enum ATTINY_PIN
{
    PB0,
    PB1,
    PB2,
    PB3,
    PB4,
    PCINT1,
};

// Stub several registers of the ATtiny
unsigned int PCMSK = 0;
unsigned int GIFR = 0;
unsigned int PCIF = 0;
unsigned int GIMSK = 0;
unsigned int PCIE = 0;

unsigned int bit(unsigned int toBit) {
    return toBit;
}

unsigned int _BV(unsigned int toBit) {
    return toBit;
}

void interrupts(){}

void noInterrupts(){}

#endif
