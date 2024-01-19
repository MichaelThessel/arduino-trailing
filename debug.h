//#define DEBUG
#ifdef DEBUG
#define LEDONX(x) { digitalWrite(LED_DEBUG, HIGH); delay(x); digitalWrite(LED_DEBUG, LOW); }
#define LEDON() { digitalWrite(LED_DEBUG, HIGH); }
#define LEDOFF() { digitalWrite(LED_DEBUG, LOW); }
#else
#define LEDONX(x)
#define LEDON(x)
#define LEDOFF(x)
#endif
