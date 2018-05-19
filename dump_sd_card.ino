#include <SD.h>
#include "base64.h"


Sd2Card card;


void setup()
{
    Serial.begin(115200);
    pinMode(10, OUTPUT);

    if (!card.init(SPI_HALF_SPEED, 10)) {
        Serial.print("Error code: 0x");
        Serial.println(card.errorCode(), HEX);
        Serial.print("Status code: 0x");
        Serial.println(card.errorData(), HEX);
        Serial.println("");

        // Several of my cards reported errors here but then worked fine, so
        // let's keep going. If the card is unreadable, it will report 0
        // blocks of capacity below.
    }

    Serial.print("Card type: 0x");
    Serial.println(card.type(), HEX);

    long blocks = card.cardSize();

    Serial.print("512B blocks: ");
    Serial.println(blocks);

    Serial.println("");
    Serial.println("begin");

    uint8_t data[512];
    char encoded[344];
    char *p;

    for (long blockNumber = 0; blockNumber < blocks; blockNumber++) {
        if (!card.readBlock(blockNumber, data)) {
            Serial.print("Error reading block ");
            Serial.println(blockNumber);
        } else {
            // The Uno doesn't have enough RAM to base64 encode the entire
            // 512B data block, so split it in two.
            p = (char *) data;

            while (p < data + 512) {
                Base64encode(encoded, p, 256);
                Serial.println(encoded);
                p += 256;
            }
        }
    }

    Serial.println("done");
}


void loop()
{
    // nothing happens after setup finishes.
}
