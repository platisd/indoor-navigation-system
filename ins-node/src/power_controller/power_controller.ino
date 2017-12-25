#include <stdint.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

enum WatchDogTimeout {
  WDT_16ms = 0,
  WDT_32ms,
  WDT_64ms,
  WDT_128ms,
  WDT_250ms,
  WDT_500ms,
  WDT_1sec,
  WDT_2sec,
  WDT_4sec,
  WDT_8sec
};

enum PowerState {
  // The wifi module is off, while power controller is
  // in deep sleep and can be woken up by a watchdog timer.
  SLEEP_WIFI_OFF,
  // The wifi module is on, while power controller is in
  // deep sleep and can be woken up by a watchdog timer
  // or an external interrupt.
  SLEEP_WIFI_ON,
  // The power controller warns the Wifi module it is time
  // to shutdown and then it goes to deep sleep for a
  // limited period. The Wifi module can ask to postpone
  // shutdown if it is not ready yet.
  PREPARE_SHUTDOWN
};

const uint8_t TX_PIN = PB0;
const uint8_t RX_PIN = PB1;
const uint8_t WIFI_MODULE_PIN = PB3; // Controls the power to the Wifi module

WatchDogTimeout currentTimeout = WDT_8sec; // Current watchdog timeout

unsigned long timeWeHaveSlept = 0; // The total amount of time we have been asleep
volatile bool watchDogBarked = false; // Flag to determine the source of a wake up
bool watchDogEnabled = false;

const unsigned long WIFI_OFF_SLEEP_DURATION = 16000UL; // TO-DO: Change this to something larger
const unsigned long WIFI_ON_SLEEP_DURATION = 16000UL; // TO-DO: Change this to something larger
const unsigned long WIFI_PREPARE_SHUTDOWN_DURATION = 10000UL;

volatile bool signalFromWifiModule = false; // Flag to determine if WiFi module has signaled us

PowerState currentState = SLEEP_WIFI_OFF; // Current operational state

/**
   Change interrupt routine (e.g. when WiFi module signals us)
*/
ISR (PCINT0_vect) {
  // Only consider interrupts from the defined source and during the proper state(s)
  if ((currentState == SLEEP_WIFI_ON) && (digitalRead(RX_PIN) == HIGH)) {
    signalFromWifiModule = true;
  }
}

/**
  Watchdog interrupt routine to be triggered when watchdog times out.
*/
ISR(WDT_vect) {
  watchDogBarked = true;
  wdt_disable(); // Disable watchdog so it does not get automatically retriggered
}

/**
    Sets up watchdog to be triggered (once) after the specified time
    @param wdt  the watchdog timeout duration
*/
void triggerWatchDogIn(WatchDogTimeout wdt) {
  // Adopted from InsideGadgets (www.insidegadgets.com)
  byte timeoutVal = wdt & 7;
  if (wdt > 7) {
    timeoutVal |= (1 << 5);
  }
  timeoutVal |= (1 << WDCE);

  MCUSR &= ~(1 << WDRF);
  WDTCR |= (1 << WDCE) | (1 << WDE); // Start timed sequence
  WDTCR = timeoutVal;
  WDTCR |= _BV(WDIE);
  wdt_reset();  // Pat the dog
  currentTimeout = wdt;
}

/**
   Sets the MCU into a deep sleep state until a change interrupt is triggered
*/
void goToSleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  ADCSRA = 0; // Turn off ADC
  power_all_disable(); // Power off ADC, Timer 0 and 1, serial interface
  sleep_enable();
  sleep_cpu(); // Sleep here and wait for an interrupt or watchdog bark
  sleep_disable();
  power_all_enable(); // Power everything back on
}

/**
   Sets up pin change interrupt for RX
*/
void setupRXInterrupt() {
  PCMSK |= bit (PCINT1); // Want pin D1 (PB1)
  GIFR |= bit (PCIF); // Clear any outstanding interrupts
  GIMSK |= bit (PCIE); // Enable pin change interrupts
}

/**
   A utility method to derive a watchdog timeout's duration
   @return the amount of milliseconds corresponding to a watchdog timeout
*/
unsigned long getTimeoutDuration() {
  return 1 << (currentTimeout + 4);
}

/**
    Puts the MCU to deep sleep and uses a watchdog timeout to wake it up after
    the currently set timeout. Monitors how long it has remained asleep since the
    last time it signaled a wake up.
    @param  totalSleepDuration how long to sleep in milliseconds
    @return                    whether it has remained asleep enough
*/
bool stayAsleepFor(const unsigned long totalSleepDuration) {
  // If watchdog is not enabled, then we either just arrived from a different state
  // or from a watchdog bark. Therefore we need to set another watchdog timer.
  if (!watchDogEnabled) {
    // If watchdog is not enabled and we are in this state
    // then we should set it up.
    triggerWatchDogIn(currentTimeout);
    watchDogEnabled = false;
  }
  // Go to deep sleep until an interrupt or watchdog bark occurs
  goToSleep();
  // We might have gone out of sleep due to an interrupt so make sure that we
  // know what we are doing depending on who interrupted our sleep.
  if (watchDogBarked) {
    watchDogBarked = false;
    watchDogEnabled = false;
    timeWeHaveSlept += getTimeoutDuration(); // Log how much time we've been in this state
    // Check if it is time to signal a state change
    if (timeWeHaveSlept >= totalSleepDuration) {
      timeWeHaveSlept = 0;
      return true;
    }
  }
  return false;
}

/**
   Notify the WiFi module that it is time to shut down
*/
void notifyWifiShutdown() {
  // Send a short pulse
  pinMode(TX_PIN, OUTPUT);
  digitalWrite(TX_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(TX_PIN, LOW);
  pinMode(TX_PIN, INPUT);
}

void setup() {
  // Setup I/O pins
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT); // Set it as an input to avoid strange parasitic voltages
  pinMode(WIFI_MODULE_PIN, OUTPUT);

  // Initialize the pin states
  digitalWrite(WIFI_MODULE_PIN, LOW);

  setupRXInterrupt();
}

void loop() {
  switch (currentState) {
    case SLEEP_WIFI_OFF:
      {
        bool timeToChangeState = stayAsleepFor(WIFI_OFF_SLEEP_DURATION);
        if (timeToChangeState) {
          currentState = SLEEP_WIFI_ON;
          digitalWrite(WIFI_MODULE_PIN, HIGH); // Turn WiFi module on
        }
        break;
      }
    case SLEEP_WIFI_ON:
      {
        bool timeToChangeState = stayAsleepFor(WIFI_ON_SLEEP_DURATION);
        if (timeToChangeState || signalFromWifiModule) {
          currentState = PREPARE_SHUTDOWN;
          notifyWifiShutdown();

          signalFromWifiModule = false;
          timeWeHaveSlept = 0; // Reset this in case sleep was prematurely halted by an interrupt
        }
        break;
      }
    case PREPARE_SHUTDOWN:
      {
        bool timeToChangeState = stayAsleepFor(WIFI_PREPARE_SHUTDOWN_DURATION);
        // Since the timeout and the interrupt have different results we must
        // guard the sensitive area.
        noInterrupts();
        if (timeToChangeState) {
          currentState = SLEEP_WIFI_OFF;
          digitalWrite(WIFI_MODULE_PIN, LOW); // Turn WiFi module off
        } else if (signalFromWifiModule) {
          currentState = SLEEP_WIFI_ON; // Give it more time before turnign off
          timeWeHaveSlept = 0; // Reset since sleep was halted by an interrupt
        }
        signalFromWifiModule = false;
        interrupts();
        break;
      }
    default:
      break;
  }
}
