
#include "pcd8544.hpp"

#define fontbyte(x) pgm_read_byte(&_cfont.font[x])
#define bitmapbyte(x) pgm_read_byte(&bitmap[x])


#define POS_LEFT 0
#define POS_RIGHT 9999
#define POS_CENTER 9998

const uint8_t SmallFont[] PROGMEM = {
    0x06, 0x08, 0x20, 0x5f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // sp
    0x00, 0x00, 0x00, 0x2f, 0x00, 0x00,   // !
    0x00, 0x00, 0x07, 0x00, 0x07, 0x00,   // "
    0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14,   // #
    0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12,   // $
    0x00, 0x23, 0x13, 0x08, 0x64, 0x62,   // %
    0x00, 0x36, 0x49, 0x55, 0x22, 0x50,   // &
    0x00, 0x00, 0x05, 0x03, 0x00, 0x00,   // '
    0x00, 0x00, 0x1c, 0x22, 0x41, 0x00,   // (
    0x00, 0x00, 0x41, 0x22, 0x1c, 0x00,   // )
    0x00, 0x14, 0x08, 0x3E, 0x08, 0x14,   // *
    0x00, 0x08, 0x08, 0x3E, 0x08, 0x08,   // +
    0x00, 0x00, 0x00, 0xA0, 0x60, 0x00,   // ,
    0x00, 0x08, 0x08, 0x08, 0x08, 0x08,   // -
    0x00, 0x00, 0x60, 0x60, 0x00, 0x00,   // .
    0x00, 0x20, 0x10, 0x08, 0x04, 0x02,   // /

    0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E,   // 0
    0x00, 0x00, 0x42, 0x7F, 0x40, 0x00,   // 1
    0x00, 0x42, 0x61, 0x51, 0x49, 0x46,   // 2
    0x00, 0x21, 0x41, 0x45, 0x4B, 0x31,   // 3
    0x00, 0x18, 0x14, 0x12, 0x7F, 0x10,   // 4
    0x00, 0x27, 0x45, 0x45, 0x45, 0x39,   // 5
    0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30,   // 6
    0x00, 0x01, 0x71, 0x09, 0x05, 0x03,   // 7
    0x00, 0x36, 0x49, 0x49, 0x49, 0x36,   // 8
    0x00, 0x06, 0x49, 0x49, 0x29, 0x1E,   // 9
    0x00, 0x00, 0x36, 0x36, 0x00, 0x00,   // :
    0x00, 0x00, 0x56, 0x36, 0x00, 0x00,   // ;
    0x00, 0x08, 0x14, 0x22, 0x41, 0x00,   // <
    0x00, 0x14, 0x14, 0x14, 0x14, 0x14,   // =
    0x00, 0x00, 0x41, 0x22, 0x14, 0x08,   // >
    0x00, 0x02, 0x01, 0x51, 0x09, 0x06,   // ?

    0x00, 0x32, 0x49, 0x59, 0x51, 0x3E,   // @
    0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C,   // A
    0x00, 0x7F, 0x49, 0x49, 0x49, 0x36,   // B
    0x00, 0x3E, 0x41, 0x41, 0x41, 0x22,   // C
    0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C,   // D
    0x00, 0x7F, 0x49, 0x49, 0x49, 0x41,   // E
    0x00, 0x7F, 0x09, 0x09, 0x09, 0x01,   // F
    0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A,   // G
    0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F,   // H
    0x00, 0x00, 0x41, 0x7F, 0x41, 0x00,   // I
    0x00, 0x20, 0x40, 0x41, 0x3F, 0x01,   // J
    0x00, 0x7F, 0x08, 0x14, 0x22, 0x41,   // K
    0x00, 0x7F, 0x40, 0x40, 0x40, 0x40,   // L
    0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F,   // M
    0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F,   // N
    0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E,   // O

    0x00, 0x7F, 0x09, 0x09, 0x09, 0x06,   // P
    0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E,   // Q
    0x00, 0x7F, 0x09, 0x19, 0x29, 0x46,   // R
    0x00, 0x46, 0x49, 0x49, 0x49, 0x31,   // S
    0x00, 0x01, 0x01, 0x7F, 0x01, 0x01,   // T
    0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F,   // U
    0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F,   // V
    0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F,   // W
    0x00, 0x63, 0x14, 0x08, 0x14, 0x63,   // X
    0x00, 0x07, 0x08, 0x70, 0x08, 0x07,   // Y
    0x00, 0x61, 0x51, 0x49, 0x45, 0x43,   // Z
    0x00, 0x00, 0x7F, 0x41, 0x41, 0x00,   // [
    0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,   // Backslash (Checker pattern)
    0x00, 0x00, 0x41, 0x41, 0x7F, 0x00,   // ]
    0x00, 0x04, 0x02, 0x01, 0x02, 0x04,   // ^
    0x00, 0x40, 0x40, 0x40, 0x40, 0x40,   // _

    0x00, 0x00, 0x03, 0x05, 0x00, 0x00,   // `
    0x00, 0x20, 0x54, 0x54, 0x54, 0x78,   // a
    0x00, 0x7F, 0x48, 0x44, 0x44, 0x38,   // b
    0x00, 0x38, 0x44, 0x44, 0x44, 0x20,   // c
    0x00, 0x38, 0x44, 0x44, 0x48, 0x7F,   // d
    0x00, 0x38, 0x54, 0x54, 0x54, 0x18,   // e
    0x00, 0x08, 0x7E, 0x09, 0x01, 0x02,   // f
    0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C,   // g
    0x00, 0x7F, 0x08, 0x04, 0x04, 0x78,   // h
    0x00, 0x00, 0x44, 0x7D, 0x40, 0x00,   // i
    0x00, 0x40, 0x80, 0x84, 0x7D, 0x00,   // j
    0x00, 0x7F, 0x10, 0x28, 0x44, 0x00,   // k
    0x00, 0x00, 0x41, 0x7F, 0x40, 0x00,   // l
    0x00, 0x7C, 0x04, 0x18, 0x04, 0x78,   // m
    0x00, 0x7C, 0x08, 0x04, 0x04, 0x78,   // n
    0x00, 0x38, 0x44, 0x44, 0x44, 0x38,   // o

    0x00, 0xFC, 0x24, 0x24, 0x24, 0x18,   // p
    0x00, 0x18, 0x24, 0x24, 0x18, 0xFC,   // q
    0x00, 0x7C, 0x08, 0x04, 0x04, 0x08,   // r
    0x00, 0x48, 0x54, 0x54, 0x54, 0x20,   // s
    0x00, 0x04, 0x3F, 0x44, 0x40, 0x20,   // t
    0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C,   // u
    0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C,   // v
    0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C,   // w
    0x00, 0x44, 0x28, 0x10, 0x28, 0x44,   // x
    0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C,   // y
    0x00, 0x44, 0x64, 0x54, 0x4C, 0x44,   // z
    0x00, 0x00, 0x10, 0x7C, 0x82, 0x00,   // {
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00,   // |
    0x00, 0x00, 0x82, 0x7C, 0x10, 0x00,   // }
    0x00, 0x00, 0x06, 0x09, 0x09, 0x06    // ~ (Degrees)
};

const uint8_t TinyFont[] PROGMEM = {
    0x04, 0x06, 0x20, 0x5f,
    0x00, 0x00, 0x00, 0x03, 0xa0, 0x00, 0xc0, 0x0c, 0x00, 0xf9, 0x4f, 0x80, 0x6b, 0xeb, 0x00, 0x98, 0x8c, 0x80, 0x52, 0xa5, 0x80, 0x03, 0x00, 0x00,  // Space, !"#$%&'
    0x01, 0xc8, 0x80, 0x89, 0xc0, 0x00, 0x50, 0x85, 0x00, 0x21, 0xc2, 0x00, 0x08, 0x40, 0x00, 0x20, 0x82, 0x00, 0x00, 0x20, 0x00, 0x18, 0x8c, 0x00,  // ()*+,-./
    0xfa, 0x2f, 0x80, 0x4b, 0xe0, 0x80, 0x5a, 0x66, 0x80, 0x8a, 0xa5, 0x00, 0xe0, 0x8f, 0x80, 0xea, 0xab, 0x00, 0x72, 0xa9, 0x00, 0x9a, 0x8c, 0x00,  // 01234567
    0xfa, 0xaf, 0x80, 0x4a, 0xa7, 0x00, 0x01, 0x40, 0x00, 0x09, 0x40, 0x00, 0x21, 0x48, 0x80, 0x51, 0x45, 0x00, 0x89, 0x42, 0x00, 0x42, 0x66, 0x00,  // 89:;<=>?
    0x72, 0xa6, 0x80, 0x7a, 0x87, 0x80, 0xfa, 0xa5, 0x00, 0x72, 0x25, 0x00, 0xfa, 0x27, 0x00, 0xfa, 0xa8, 0x80, 0xfa, 0x88, 0x00, 0x72, 0x2b, 0x00,  // @ABCDEFG
    0xf8, 0x8f, 0x80, 0x8b, 0xe8, 0x80, 0x8b, 0xe8, 0x00, 0xf8, 0x8d, 0x80, 0xf8, 0x20, 0x80, 0xf9, 0x0f, 0x80, 0xf9, 0xcf, 0x80, 0x72, 0x27, 0x00,  // HIJKLMNO
    0xfa, 0x84, 0x00, 0x72, 0x27, 0x40, 0xfa, 0x85, 0x80, 0x4a, 0xa9, 0x00, 0x83, 0xe8, 0x00, 0xf0, 0x2f, 0x00, 0xe0, 0x6e, 0x00, 0xf0, 0xef, 0x00,  // PQRSTUVW
    0xd8, 0x8d, 0x80, 0xc0, 0xec, 0x00, 0x9a, 0xac, 0x80, 0x03, 0xe8, 0x80, 0xc0, 0x81, 0x80, 0x8b, 0xe0, 0x00, 0x42, 0x04, 0x00, 0x08, 0x20, 0x80,  // XYZ[\]^_
    0x02, 0x04, 0x00, 0x31, 0x23, 0x80, 0xf9, 0x23, 0x00, 0x31, 0x24, 0x80, 0x31, 0x2f, 0x80, 0x31, 0x62, 0x80, 0x23, 0xea, 0x00, 0x25, 0x53, 0x80,  // `abcdefg
    0xf9, 0x03, 0x80, 0x02, 0xe0, 0x00, 0x06, 0xe0, 0x00, 0xf8, 0x42, 0x80, 0x03, 0xe0, 0x00, 0x79, 0x87, 0x80, 0x39, 0x03, 0x80, 0x31, 0x23, 0x00,  // hijklmno
    0x7d, 0x23, 0x00, 0x31, 0x27, 0xc0, 0x78, 0x84, 0x00, 0x29, 0x40, 0x00, 0x43, 0xe4, 0x00, 0x70, 0x27, 0x00, 0x60, 0x66, 0x00, 0x70, 0x67, 0x00,  // pqrstuvw
    0x48, 0xc4, 0x80, 0x74, 0x57, 0x80, 0x59, 0xe6, 0x80, 0x23, 0xe8, 0x80, 0x03, 0x60, 0x00, 0x8b, 0xe2, 0x00, 0x61, 0x0c, 0x00                     // zyx{|}~
};

static inline uint8_t uabs(cuint8_t a, cuint8_t b)
{
    return (a > b) ? (a - b) : (b - a);
}

cuint8_t constMaxPosX                   = 83;
cuint8_t constMaxPosY                   = 48;

// PCD8544 Command set
// -------------------
// General commands
#define PCD8544_POWERDOWN			0x04
#define PCD8544_ENTRYMODE			0x02
#define PCD8544_EXTENDEDINSTRUCTION	0x01
#define PCD8544_DISPLAYBLANK		0x00
#define PCD8544_DISPLAYNORMAL		0x04
#define PCD8544_DISPLAYALLON		0x01
#define PCD8544_DISPLAYINVERTED		0x05
// Normal instruction set
#define PCD8544_FUNCTIONSET			0x20
#define PCD8544_DISPLAYCONTROL		0x08
#define PCD8544_SETYADDR			0x40
#define PCD8544_SETXADDR			0x80
// Extended instruction set
#define PCD8544_SETTEMP				0x04
#define PCD8544_SETBIAS				0x10
#define PCD8544_SETVOP				0x80
// Display presets
#define LCD_BIAS					0x03	// Range: 0-7 (0x00-0x07)
#define LCD_TEMP					0x02	// Range: 0-3 (0x00-0x03)


Pcd8544::Pcd8544(void)
{
    // Resets data members
    this->_sclkPinOut                   = nullptr;
    this->_sclkPinDir                   = nullptr;
    this->_sclkPinBit                   = 0xFF;
    this->_mosiPinOut                   = nullptr;
    this->_mosiPinDir                   = nullptr;
    this->_mosiPinBit                   = 0xFF;
    this->_dataControlPinOut            = nullptr;
    this->_dataControlPinDir            = nullptr;
    this->_dataControlPinBit            = 0xFF;
    this->_resetPinOut                  = nullptr;
    this->_resetPinDir                  = nullptr;
    this->_resetPinBit                  = 0xFF;
    this->_chipSelectPinOut             = nullptr;
    this->_chipSelectPinDir             = nullptr;
    this->_chipSelectPinBit             = 0xFF;
    this->_isSleepModeOn            = false;
    this->_contrastValue         = 0;
    this->_isInitialized    = false;
    this->_isPortsSet       = false;

    // Returns successfully
    this->_lastError = Error::NONE;
    return;
}

Pcd8544::~Pcd8544(void)
{
    // Returns successfully
    return;
}

bool_t Pcd8544::clearScreen(void)
{
    // Clear data buffer
    for(uint16_t i = 0; i < 504; i++) {
        this->_dataBuffer[i] = 0;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t Pcd8544::drawLine(uint8_t xPos1_p, uint8_t yPos1_p, uint8_t xPos2_p, uint8_t yPos2_p, const Color color_p)
{
    // Check for errors
    if((xPos1_p > constMaxPosX) || (xPos2_p > constMaxPosX)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_VALUE_INVALID;
        return false;
    }
    if((yPos1_p > constMaxPosY) || (yPos2_p > constMaxPosY)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_VALUE_INVALID;
        return false;
    }

    // If Coord_1 == Coord_2, then draws pixels
    if((xPos1_p == xPos2_p) && (yPos1_p == yPos2_p)) {  // Draws pixel
        if(!this->drawPixel(xPos1_p, yPos1_p, color_p)) {
            // Returns error
            return false;
        }
    }

    // Checks which line to draw
    if(xPos1_p == xPos2_p) {            // Draws vertical line
        if(!this->_drawLineVertical(xPos1_p, getMinimumOf(yPos1_p, yPos2_p), uabs(yPos2_p, yPos1_p) + 1, color_p)) {
            // Returns error
            return false;
        }
    } else if(yPos1_p == yPos2_p) {     // Draws horizontal line
        if(!this->_drawLineHorizontal(getMinimumOf(xPos1_p, xPos2_p), yPos1_p, uabs(xPos2_p, xPos1_p) + 1, color_p)) {
            // Returns error
            return false;
        }
    } else {                            // Draws diagonal line
        if(!this->_drawLineDiagonal(xPos1_p, yPos1_p, xPos2_p, yPos2_p, color_p)) {
            // Returns error
            return false;
        }
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t Pcd8544::drawPixel(cuint8_t xPos_p, cuint8_t yPos_p, const Color color_p)
{
    // Local variables
    uint16_t auxBufferIndex = 0;
    uint8_t auxBitIndex = 0;

    // Check for errors
    if((xPos_p > constMaxPosX) || (yPos_p > constMaxPosY)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_VALUE_INVALID;
        return false;
    }

    // Evaluate pixel location
    auxBufferIndex = ((uint16_t)(yPos_p / 8) * 84) + (uint16_t)xPos_p;
    auxBitIndex = yPos_p % 8;

    // Updates in buffer
    if(color_p == Color::BLACK) {
        setBit(this->_dataBuffer[auxBufferIndex], auxBitIndex);
    } else {
        clrBit(this->_dataBuffer[auxBufferIndex], auxBitIndex);
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t Pcd8544::drawRectangle(cuint8_t xPos1_p, cuint8_t yPos1_p, cuint8_t xPos2_p, cuint8_t yPos2_p,
        const Color color_p)
{
    // Order coordinates so x0<=xN and y0<=yN
    cuint8_t x0 = getMinimumOf(xPos1_p, xPos2_p);
    cuint8_t xN = getMaximumOf(xPos1_p, xPos2_p);
    cuint8_t y0 = getMinimumOf(yPos1_p, yPos2_p);
    cuint8_t yN = getMaximumOf(yPos1_p, yPos2_p);

    // Draw border
    this->_drawLineHorizontal(x0, y0, xN - x0 + 1, color_p);
    this->_drawLineVertical(xN, y0, yN - y0 + 1, color_p);
    this->_drawLineHorizontal(x0, yN, xN - x0 + 1, color_p);
    this->_drawLineVertical(x0, y0, yN - y0 + 1, color_p);

    this->_lastError = Error::NONE;
    return true;
}

bool_t Pcd8544::drawRectangleFill(cuint8_t xPos1_p, cuint8_t yPos1_p, cuint8_t xPos2_p, cuint8_t yPos2_p,
        const Color color_p)
{
    // Order coordinates so x0<=xN and y0<=yN
    cuint8_t x0 = getMinimumOf(xPos1_p, xPos2_p);
    cuint8_t xN = getMaximumOf(xPos1_p, xPos2_p);
    cuint8_t y0 = getMinimumOf(yPos1_p, yPos2_p);
    cuint8_t yN = getMaximumOf(yPos1_p, yPos2_p);

    // Draw each line
    for(uint8_t y = y0; y <= yN; y++) {
        this->_drawLineHorizontal(x0, y, xN - x0 + 1, color_p);
    }

    this->_lastError = Error::NONE;
    return true;
}

bool_t Pcd8544::fillScreen(void)
{
    // Clear data buffer
    for(uint16_t i = 0; i < 504; i++) {
        this->_dataBuffer[i] = 0xFF;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

Error Pcd8544::getLastError(void)
{
    // Returns error
    return this->_lastError;
}

bool_t Pcd8544::init(cuint8_t contrastValue_p)
{
    // Resets data members
    this->_isInitialized = true;

    // Checks for errors
    if(!this->_isPortsSet) {            // Ports not set
        // Returns error
        this->_lastError = Error::GPIO_PORT_NOT_SET;
        return false;
    }
    if(contrastValue_p > 0x7F) {        // Invalid contrast value
        // Returns error
        this->_lastError = Error::ARGUMENT_VALUE_INVALID;
        return false;
    }

    // Configures ports
    setBit(*(this->_sclkPinOut), this->_sclkPinBit);
    setBit(*(this->_sclkPinDir), this->_sclkPinBit);
    setBit(*(this->_mosiPinOut), this->_mosiPinBit);
    setBit(*(this->_mosiPinDir), this->_mosiPinBit);
    setBit(*(this->_dataControlPinDir), this->_dataControlPinBit);
    setBit(*(this->_resetPinOut), this->_resetPinBit);
    setBit(*(this->_resetPinDir), this->_resetPinBit);
    setBit(*(this->_chipSelectPinOut), this->_chipSelectPinBit);
    setBit(*(this->_chipSelectPinDir), this->_chipSelectPinBit);

    // Reset LCD
    setBit(*(this->_dataControlPinOut), this->_dataControlPinBit);
    setBit(*(this->_mosiPinOut), this->_mosiPinBit);
    setBit(*(this->_sclkPinOut), this->_sclkPinBit);
    setBit(*(this->_chipSelectPinOut), this->_chipSelectPinBit);
    clrBit(*(this->_resetPinOut), this->_resetPinBit);
    delayMs(10);
    setBit(*(this->_resetPinOut), this->_resetPinBit);

    // Initialization procedure
    if(!this->_write(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, Pcd8544::Mode::COMMAND)) {
        // Returns error
        return false;
    }
    if(!this->_write(PCD8544_SETVOP | contrastValue_p, Pcd8544::Mode::COMMAND)) {
        // Returns error
        return false;
    }
    if(!this->_write(PCD8544_SETTEMP | LCD_TEMP, Pcd8544::Mode::COMMAND)) {
        // Returns error
        return false;
    }
    if(!this->_write(PCD8544_SETBIAS | LCD_BIAS, Pcd8544::Mode::COMMAND)) {
        // Returns error
        return false;
    }
    if(!this->_write(PCD8544_FUNCTIONSET, Pcd8544::Mode::COMMAND)) {
        // Returns error
        return false;
    }
    if(!this->_write(PCD8544_SETYADDR, Pcd8544::Mode::COMMAND)) {
        // Returns error
        return false;
    }
    if(!this->_write(PCD8544_SETXADDR, Pcd8544::Mode::COMMAND)) {
        // Returns error
        return false;
    }
    if(!this->_write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL, Pcd8544::Mode::COMMAND)) {
        // Returns error
        return false;
    }

    if(!this->clearScreen()) {
        // Returns error
        return false;
    }
    if(!this->renderScreen()) {
        // Returns error
        return false;
    }

    // Updates data members
    this->_cfont.font = 0;
    this->_isSleepModeOn = false;
    this->_contrastValue = contrastValue_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t Pcd8544::print(char_t *st, int x, int y)
{
    int stl;

    stl = strlen(st);
    if(x == POS_RIGHT) {
        x = 84 - (stl * _cfont.x_size);
    }
    if(x == POS_CENTER) {
        x = (84 - (stl * _cfont.x_size)) / 2;
    }

    for(int cnt = 0; cnt < stl; cnt++) {
        _printChar(*st++, x + (cnt * (_cfont.x_size)), y);
    }
    return true;
}

bool_t Pcd8544::renderScreen(void)
{
    // Checks for errors
    if(!this->_isInitialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        return false;
    }
    if(this->_isSleepModeOn) {
        // Returns error
        this->_lastError = Error::UNKNOWN;
        return false;
    }

    // Sets position (0, 0)
    if(!this->_write(PCD8544_SETYADDR, Pcd8544::Mode::COMMAND)) {
        // Returns error
        return false;
    }
    if(!this->_write(PCD8544_SETXADDR, Pcd8544::Mode::COMMAND)) {
        // Returns error
        return false;
    }

    // Updates entire screen
    for(uint16_t i = 0; i < 504; i++) {
        if(!this->_write(this->_dataBuffer[i], Pcd8544::Mode::DATA)) {
            // Returns error
            return false;
        }
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t Pcd8544::setContrast(cuint8_t contrastValue_p)
{
    // Checks for errors
    if(!this->_isInitialized) {         // Not initialized
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        return false;
    }
    if(contrastValue_p > 0x7F) {        // Invalid contrast value
        // Returns error
        this->_lastError = Error::ARGUMENT_VALUE_INVALID;
        return false;
    }

    // Sends command
    if(!this->_write(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, Pcd8544::Mode::COMMAND)) {
        // Returns error
        return false;
    }
    if(!this->_write(PCD8544_SETVOP | contrastValue_p, Pcd8544::Mode::COMMAND)) {
        // Returns error
        return false;
    }
    if(!this->_write(PCD8544_FUNCTIONSET, Pcd8544::Mode::COMMAND)) {
        // Returns error
        return false;
    }

    // Updates data members
    this->_contrastValue = contrastValue_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t Pcd8544::setFont(uint8_t *font_p)
{
    this->_cfont.font = font_p;
    this->_cfont.x_size = fontbyte(0);
    this->_cfont.y_size = fontbyte(1);
    this->_cfont.offset = fontbyte(2);
    this->_cfont.numchars = fontbyte(3);
    this->_cfont.inverted = 0;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t Pcd8544::setInvertedMode(cbool_t isTurnedOn_p)
{
    // Local variables
    uint8_t auxCommand = PCD8544_DISPLAYCONTROL;

    // Checks for errors - not initialized
    if(!this->_isInitialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        return false;
    }

    // Sends command
    auxCommand |= ((isTurnedOn_p) ? (PCD8544_DISPLAYINVERTED) : (PCD8544_DISPLAYNORMAL));
    if(!this->_write(auxCommand, Pcd8544::Mode::COMMAND)) {
        // Returns error
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t Pcd8544::setPort(ioRegAddress_t controlPort_p, ioPinIndex_t chipSelectPinIndex_p, ioPinIndex_t clockPinIndex_p,
        ioPinIndex_t dataPinIndex_p, ioPinIndex_t dataCommandPinIndex_p, ioPinIndex_t resetPinIndex_p)
{
    // Resets data members
    this->_isInitialized = false;
    this->_isPortsSet = false;

    // Checks for erros
    if(!isGpioAddressValid(controlPort_p)) {
        // Returns error
        this->_lastError = Error::GPIO_PORT_INVALID;
        return false;
    }
    if((!isGpioPinIndexValid(chipSelectPinIndex_p)) ||
            (!isGpioPinIndexValid(clockPinIndex_p)) ||
            (!isGpioPinIndexValid(dataPinIndex_p)) ||
            (!isGpioPinIndexValid(dataCommandPinIndex_p)) ||
            (!isGpioPinIndexValid(resetPinIndex_p))
    ) {
        // Returns error
        this->_lastError = Error::GPIO_PIN_INDEX_INVALID;
        return false;
    }

    // Updates data members
    this->_sclkPinOut               = getGpioPortAddress(controlPort_p);
    this->_sclkPinDir               = getGpioDdrAddress(controlPort_p);
    this->_mosiPinOut               = getGpioPortAddress(controlPort_p);
    this->_mosiPinDir               = getGpioDdrAddress(controlPort_p);
    this->_dataControlPinOut        = getGpioPortAddress(controlPort_p);
    this->_dataControlPinDir        = getGpioDdrAddress(controlPort_p);
    this->_resetPinOut              = getGpioPortAddress(controlPort_p);
    this->_resetPinDir              = getGpioDdrAddress(controlPort_p);
    this->_chipSelectPinOut         = getGpioPortAddress(controlPort_p);
    this->_chipSelectPinDir         = getGpioDdrAddress(controlPort_p);
    this->_sclkPinBit               = clockPinIndex_p;
    this->_mosiPinBit               = dataPinIndex_p;
    this->_dataControlPinBit        = dataCommandPinIndex_p;
    this->_resetPinBit              = resetPinIndex_p;
    this->_chipSelectPinBit         = chipSelectPinIndex_p;
    this->_isPortsSet               = true;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t Pcd8544::setSleepMode(cbool_t isTurnedOn_p)
{
    // Checks for errors - not initialized
    if(!this->_isInitialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        return false;
    }

    // Sends command
    if(isTurnedOn_p) {
        if(!this->_write(PCD8544_SETYADDR, Pcd8544::Mode::COMMAND)) {
            // Returns error
            return false;
        }
        if(!this->_write(PCD8544_SETXADDR, Pcd8544::Mode::COMMAND)) {
            // Returns error
            return false;
        }
        for(uint16_t i = 0; i < 504; i++) {
            if(!this->_write(0, Pcd8544::Mode::DATA)) {
                // Returns error
                return false;
            }
        }
        if(!this->_write(PCD8544_FUNCTIONSET | PCD8544_POWERDOWN, Pcd8544::Mode::COMMAND)) {
            // Returns error
            return false;
        }

    } else {
        if(!this->_write(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, Pcd8544::Mode::COMMAND)) {
            // Returns error
            return false;
        }
        if(!this->_write(PCD8544_SETVOP | this->_contrastValue, Pcd8544::Mode::COMMAND)) {
            // Returns error
            return false;
        }
        if(!this->_write(PCD8544_SETTEMP | LCD_TEMP, Pcd8544::Mode::COMMAND)) {
            // Returns error
            return false;
        }
        if(!this->_write(PCD8544_SETBIAS | LCD_BIAS, Pcd8544::Mode::COMMAND)) {
            // Returns error
            return false;
        }
        if(!this->_write(PCD8544_FUNCTIONSET, Pcd8544::Mode::COMMAND)) {
            // Returns error
            return false;
        }
        if(!this->_write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL, Pcd8544::Mode::COMMAND)) {
            // Returns error
            return false;
        }
        if(!this->renderScreen()) {
            // Returns error
            return false;
        }
    }

    // Updates data members
    this->_isSleepModeOn = isTurnedOn_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}



bool_t Pcd8544::_drawLineDiagonal(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, Color color_p)
{
    int16_t dx = (x1 > x0 ? x1 - x0 : x0 - x1);
    int16_t sx = (x0 < x1 ?  1 : -1);
    int16_t dy = -(y1 > y0 ? y1 - y0 : y0 - y1);
    int16_t sy = (y0 < y1 ?  1 : -1);
    int16_t err = dx + dy;  // erro inicial = dx + dy

    while(true) {
        drawPixel(x0, y0, color_p);

        if(x0 == x1 && y0 == y1) {
            break;
        }
        int16_t e2 = 2 * err;
        if(e2 >= dy) {
            err += dy;   // mover em x
            x0  += sx;
        }
        if(e2 <= dx) {
            err += dx;   // mover em y
            y0  += sy;
        }
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t Pcd8544::_drawLineHorizontal(cuint8_t xPos_p, cuint8_t yPos_p, cuint8_t length_p, const Color color_p)
{
    // Local variables
    uint16_t auxBufferIndex = 0;
    uint8_t auxBitIndex = 0;

    // Evaluate first pixel location
    auxBufferIndex = ((uint16_t)(yPos_p / 8) * 84) + (uint16_t)xPos_p;
    auxBitIndex = yPos_p % 8;

    // Updates in buffer
    for(uint8_t i = 0; i < length_p; i++) {
        if(color_p == Color::BLACK) {
            setBit(this->_dataBuffer[auxBufferIndex + i], auxBitIndex);
        } else {
            clrBit(this->_dataBuffer[auxBufferIndex + i], auxBitIndex);
        }
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

bool_t Pcd8544::_drawLineVertical(cuint8_t xPos_p, cuint8_t yPos_p, cuint8_t length_p, const Color color_p)
{
    uint16_t bufferIndex = (yPos_p / 8) * 84 + xPos_p;
    uint8_t  bitOffset   = yPos_p % 8;
    int16_t  remaining   = length_p;

    while(remaining > 0) {
        int8_t chunk = getMinimumOf((uint8_t)remaining, (uint8_t)(8 - bitOffset));
        uint8_t mask = ((1u << chunk) - 1) << bitOffset;

        if(color_p == Color::BLACK) {
            this->_dataBuffer[bufferIndex] |=  mask;
        } else {
            this->_dataBuffer[bufferIndex] &= ~mask;
        }

        remaining   -= chunk;
        bufferIndex += 84;
        bitOffset    = 0;
    }

    this->_lastError = Error::NONE;
    return true;
}

bool_t Pcd8544::_printChar(uint8_t c, uint8_t x, uint8_t y)
{
    if((_cfont.y_size % 8) == 0) {
        int font_idx = ((c - _cfont.offset) * (_cfont.x_size * (_cfont.y_size / 8))) + 4;
        for(int rowcnt = 0; rowcnt < (_cfont.y_size / 8); rowcnt++) {
            for(int cnt = 0; cnt < _cfont.x_size; cnt++) {
                for(int b = 0; b < 8; b++)
                    if((fontbyte(font_idx + cnt + (rowcnt * _cfont.x_size)) & (1 << b)) != 0)
                        if(_cfont.inverted == 0) {
                            drawPixel(x + cnt, y + (rowcnt * 8) + b, Color::BLACK);
                        } else {
                            drawPixel(x + cnt, y + (rowcnt * 8) + b, Color::WHITE);
                        } else if(_cfont.inverted == 0) {
                        drawPixel(x + cnt, y + (rowcnt * 8) + b, Color::WHITE);
                    } else {
                        drawPixel(x + cnt, y + (rowcnt * 8) + b, Color::BLACK);
                    }
            }
        }
    } else {
        int font_idx = ((c - _cfont.offset) * ((_cfont.x_size * _cfont.y_size / 8))) + 4;
        int cbyte = fontbyte(font_idx);
        int cbit = 7;
        for(int cx = 0; cx < _cfont.x_size; cx++) {
            for(int cy = 0; cy < _cfont.y_size; cy++) {
                if((cbyte & (1 << cbit)) != 0)
                    if(_cfont.inverted == 0) {
                        drawPixel(x + cx, y + cy, Color::BLACK);
                    } else {
                        drawPixel(x + cx, y + cy, Color::WHITE);
                    } else if(_cfont.inverted == 0) {
                    drawPixel(x + cx, y + cy, Color::WHITE);
                } else {
                    drawPixel(x + cx, y + cy, Color::BLACK);
                }
                cbit--;
                if(cbit < 0) {
                    cbit = 7;
                    font_idx++;
                    cbyte = fontbyte(font_idx);
                }
            }
        }
    }
    return true;
}

// Helper integer square-root (val ≤ 0x3FFF)
// Fonte: algoritmo “bitwise”
// Retorna floor(sqrt(val))
static inline uint8_t isqrt(uint16_t val)
{
    uint16_t res = 0;
    uint16_t bit = 1u << 14;            // o maior bit par ≤ 0x4000
    // Ajusta 'bit' para a maior potência de 4 ≤ val
    while(bit > val) {
        bit >>= 2;
    }
    while(bit) {
        if(val >= res + bit) {
            val   -= res + bit;
            res   = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return (uint8_t)res;
}

/**
 * @brief Draw the outline of a rectangle with rounded corners.
 *
 * @param xPos1_p  X of first corner
 * @param yPos1_p  Y of first corner
 * @param xPos2_p  X of opposite corner
 * @param yPos2_p  Y of opposite corner
 * @param radius_p Corner radius in pixels
 * @param color_p  Pixel color (BLACK/WHITE)
 */
bool_t Pcd8544::drawRoundedRectangle(uint8_t xPos1_p, uint8_t yPos1_p, uint8_t xPos2_p, uint8_t yPos2_p,
        uint8_t radius_p, const Color color_p)
{
    // Sort corners
    if(xPos2_p < xPos1_p) {
        uint8_t t = xPos1_p;
        xPos1_p = xPos2_p;
        xPos2_p = t;
    }
    if(yPos2_p < yPos1_p) {
        uint8_t t = yPos1_p;
        yPos1_p = yPos2_p;
        yPos2_p = t;
    }

    // Clamp radius
    uint8_t w = xPos2_p - xPos1_p + 1;
    uint8_t h = yPos2_p - yPos1_p + 1;
    if(radius_p > (w >> 1)) {
        radius_p = w >> 1;
    }
    if(radius_p > (h >> 1)) {
        radius_p = h >> 1;
    }

    // Draw straight edges (excluding corners)
    this->_drawLineHorizontal(xPos1_p + radius_p, yPos1_p, w - 2 * radius_p, color_p);
    this->_drawLineHorizontal(xPos1_p + radius_p, yPos2_p, w - 2 * radius_p, color_p);
    this->_drawLineVertical(xPos2_p, yPos1_p + radius_p, h - 2 * radius_p, color_p);
    this->_drawLineVertical(xPos1_p, yPos1_p + radius_p, h - 2 * radius_p, color_p);

    // Draw corner arcs (quarter‐circles)
    int16_t cx0 = xPos1_p + radius_p, cy0 = yPos1_p + radius_p; // TL
    int16_t cx1 = xPos2_p - radius_p, cy1 = yPos1_p + radius_p; // TR
    int16_t cx2 = xPos2_p - radius_p, cy2 = yPos2_p - radius_p; // BR
    int16_t cx3 = xPos1_p + radius_p, cy3 = yPos2_p - radius_p; // BL

    int16_t x = 0, y = radius_p;
    int16_t d = 1 - radius_p;

    while(x <= y) {
        // top-left
        this->drawPixel(cx0 - x, cy0 - y, color_p);
        this->drawPixel(cx0 - y, cy0 - x, color_p);
        // top-right
        this->drawPixel(cx1 + x, cy1 - y, color_p);
        this->drawPixel(cx1 + y, cy1 - x, color_p);
        // bottom-right
        this->drawPixel(cx2 + x, cy2 + y, color_p);
        this->drawPixel(cx2 + y, cy2 + x, color_p);
        // bottom-left
        this->drawPixel(cx3 - x, cy3 + y, color_p);
        this->drawPixel(cx3 - y, cy3 + x, color_p);

        if(d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }

    this->_lastError = Error::NONE;
    return true;
}

/**
 * @brief Draw a filled rectangle with rounded corners.
 *
 * @param xPos1_p  X of first corner
 * @param yPos1_p  Y of first corner
 * @param xPos2_p  X of opposite corner
 * @param yPos2_p  Y of opposite corner
 * @param radius_p Corner radius in pixels
 * @param color_p  Pixel color (BLACK/WHITE)
 */
bool_t Pcd8544::drawRoundedRectangleFill(uint8_t xPos1_p, uint8_t yPos1_p, uint8_t xPos2_p, uint8_t yPos2_p,
        uint8_t radius_p, const Color color_p)
{
    // Sort corners
    if(xPos2_p < xPos1_p) {
        uint8_t t = xPos1_p;
        xPos1_p = xPos2_p;
        xPos2_p = t;
    }
    if(yPos2_p < yPos1_p) {
        uint8_t t = yPos1_p;
        yPos1_p = yPos2_p;
        yPos2_p = t;
    }

    // Clamp radius
    uint8_t w = xPos2_p - xPos1_p + 1;
    uint8_t h = yPos2_p - yPos1_p + 1;
    if(radius_p > (w >> 1)) {
        radius_p = w >> 1;
    }
    if(radius_p > (h >> 1)) {
        radius_p = h >> 1;
    }

    int16_t cy0 = yPos1_p + radius_p;
    int16_t cy1 = yPos2_p - radius_p;
    int16_t cx0 = xPos1_p + radius_p;
    int16_t cx1 = xPos2_p - radius_p;
    int16_t r2  = (int16_t)radius_p * radius_p;

    for(int16_t y = yPos1_p; y <= yPos2_p; y++) {
        int16_t xs, xe;
        if(y < cy0) {
            int16_t dy = cy0 - y;
            int16_t dx = isqrt(r2 - dy * dy);
            xs = cx0 - dx;
            xe = cx1 + dx;
        } else if(y > cy1) {
            int16_t dy = y - cy1;
            int16_t dx = isqrt(r2 - dy * dy);
            xs = cx0 - dx;
            xe = cx1 + dx;
        } else {
            xs = xPos1_p;
            xe = xPos2_p;
        }
        this->_drawLineHorizontal((uint8_t)xs, (uint8_t)y, (uint8_t)(xe - xs + 1), color_p);
    }

    this->_lastError = Error::NONE;
    return true;
}

bool_t Pcd8544::_write(cuint8_t data_p, const Mode mode_p)
{
    // Local variables
    uint8_t auxByte = data_p;

    // Checks for errors - not initialized
    if(!this->_isInitialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        return false;
    }

    // Selects device
    clrBit(*(this->_chipSelectPinOut), this->_chipSelectPinBit);

    // Sets command/data mode
    if(mode_p == Pcd8544::Mode::COMMAND) {
        clrBit(*(this->_dataControlPinOut), this->_dataControlPinBit);
    } else {
        setBit(*(this->_dataControlPinOut), this->_dataControlPinBit);
    }

    // Sends data (bit-bang communication)
    for(uint8_t i = 0; i < 8; i++) {
        if(isBitSet(auxByte, 7)) {              // Evaluates last bit
            setBit(*(this->_mosiPinOut), this->_mosiPinBit);    // Sets data line
        } else {
            clrBit(*(this->_mosiPinOut), this->_mosiPinBit);    // Clears data line
        }
        auxByte <<= 1;                                          // Shifts byte
        clrBit(*(this->_sclkPinOut), this->_sclkPinBit);        // Clock pulse
        doNop();                                                // ...
        setBit(*(this->_sclkPinOut), this->_sclkPinBit);        // ...
    }

    // Deselects device
    setBit(*(this->_chipSelectPinOut), this->_chipSelectPinBit);

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}
