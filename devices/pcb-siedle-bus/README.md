
[aib-150-01 Vendor page](https://www.siedle.de/de-de/home/service/linked-pages/produktkatalog/tuersprechanlagen-innen/siedle-basic/aib-150-01/)
[German Forum thread](https://www.mikrocontroller.net/topic/308271)

010 - 3 Bit, Start des Siedle In Home-Buspakets
6 Bit - Signal oder Befehl
5 Bit - Adresse des Zielgeräts
4 Bit - Adresse des Zielsegments
010 - 3 Bits, normalerweise sind sie immer so, aber sofort habe ich 
einen Befehl erhalten, der hier 110 hatte. Auf jeden Fall scheint "1" in 
der Mitte "1" zu sein.
5 Bit - Adresse des Quellgeräts
4 Bit - Adresse des Quellensegments
00 - 2 Bits, normalerweise sind sie 00, aber in einigen Fällen könnten 
es 10 sein. Ich persönlich glaube, dass dies wahrscheinlich irgendwie 
mit Broadcast-Nachrichten zusammenhängt, aber ich weiß es wirklich 
nicht.

#define DEV_ON_BUS 0b101000
#define LIGHT_ON 0b101100
#define SET_SEGMENT_ADDR 0b010000
#define IS_ADDR_OK 0b110011
#define ADDR_OK 0b100100
#define START_PROG 0b110000
#define PROGunknown 0b010011
#define STOPPROG 0b001000
#define CANCEL_SEG 0b011100
#define PROGMODE_DEV 0b111010
#define INCOMING_RING 0b110001
#define RING_ACCEPTED 0b001110
#define ONAIR 0b000001
#define ONAIR_OFF 0b100001
#define OPEN_DOOR 0b001100
#define CALLBACK 0b111001

44.9A.10.B0
44.9A.10.A0
40.8A.11.A0
46.0A.11.A0
46.0A.11.A0
42.9A.11.A0
42.9A.11.A0


44.A2.10.A0 Pequeno
44.FA.10.A0 Grande