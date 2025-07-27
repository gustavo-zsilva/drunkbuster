
// =============================================================================
// Include guard (START)
// =============================================================================

#ifndef __FUNSAPE_LIB_PCD8544_HPP
#define __FUNSAPE_LIB_PCD8544_HPP               2407

// =============================================================================
// Dependencies
// =============================================================================

//     /////////////////     GLOBAL DEFINITIONS FILE    /////////////////     //
#include "funsape/funsapeLibGlobalDefines.hpp"
#if !defined(__FUNSAPE_LIB_GLOBAL_DEFINES_HPP)
#    error "Global definitions file is corrupted!"
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __FUNSAPE_LIB_PCD8544_HPP
#    error "Version mismatch between file header and global definitions file!"
#endif

//     //////////////////     LIBRARY DEPENDENCIES     //////////////////     //

// NONE

//     ///////////////////     STANDARD C LIBRARY     ///////////////////     //

// NONE

//     ////////////////////    AVR LIBRARY FILES     ////////////////////     //
#include <avr/pgmspace.h>

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constant definitions
// =============================================================================

#define LCD_CONTRAST                    0x46    // Range: 0-127 (0x00-0x7F)

// =============================================================================
// New data types
// =============================================================================

// NONE

// =============================================================================
// Interrupt callback functions
// =============================================================================

// NONE

// =============================================================================
// Public functions declarations
// =============================================================================

// NONE

extern const uint8_t SmallFont[] PROGMEM;
extern const uint8_t TinyFont[] PROGMEM;

// =============================================================================
// Classes
// =============================================================================

//!
//! \brief          Pcd8544 class
//! \details        Pcd8544 class.
//!
class Pcd8544
{
    // -------------------------------------------------------------------------
    // New data types ----------------------------------------------------------
public:

    enum class Color : bool_t {
        BLACK       = false,
        WHITE       = true
    };

private:

    struct current_font {
        uint8_t *font;
        uint8_t x_size;
        uint8_t y_size;
        uint8_t offset;
        uint8_t numchars;
        uint8_t inverted;
    };
    enum class Mode : bool_t {
        COMMAND     = false,
        DATA        = true
    };


protected:

    // -------------------------------------------------------------------------
    // Constructors ------------------------------------------------------------
public:

    //!
    //! \brief      Pcd8544 class constructor
    //! \details    Creates a Pcd8544 object.
    //!
    Pcd8544(
            void
    );

    //!
    //! \brief      Pcd8544 class destructor
    //! \details    Destroys a Pcd8544 object.
    //!
    ~Pcd8544(
            void
    );

private:

    // NONE

protected:

    // NONE

    // -------------------------------------------------------------------------
    // Methods - Inherited methods ---------------------------------------------
public:

    // NONE

private:

    // NONE

protected:

    // NONE

    // -------------------------------------------------------------------------
    // Methods - Class own methods ---------------------------------------------
public:

    //     ////////////////////    CONFIGURATION     ////////////////////     //

    bool_t init(
            cuint8_t contrastValue_p    = LCD_CONTRAST
    );
    bool_t setPort(
            ioRegAddress_t controlPort_p,
            ioPinIndex_t chipSelectPinIndex_p,
            ioPinIndex_t clockPinIndex_p,
            ioPinIndex_t dataPinIndex_p,
            ioPinIndex_t dataCommandPinIndex_p,
            ioPinIndex_t resetPinIndex_p
    );

    //     /////////////////     CONTROL AND STATUS     /////////////////     //

    bool_t clearScreen(
            void
    );
    bool_t fillScreen(
            void
    );
    Error getLastError(
            void
    );
    bool_t renderScreen(
            void
    );
    bool_t setContrast(
            cuint8_t contrastValue_p
    );
    bool_t setInvertedMode(
            cbool_t isTurnedOn_p        = true
    );
    bool_t setSleepMode(
            cbool_t isTurnedOn_p        = true
    );

    //     ///////////////////////    DRAWING     ///////////////////////     //

    bool_t drawPixel(
            cuint8_t xPos_p,
            cuint8_t yPos_p,
            const Color color_p = Color::BLACK
    );
    bool_t drawLine(
            uint8_t xPos1_p,
            uint8_t yPos1_p,
            uint8_t xPos2_p,
            uint8_t yPos2_p,
            const Color color_p = Color::BLACK
    );
    bool_t drawRectangle(
            cuint8_t xPos1_p,
            cuint8_t yPos1_p,
            cuint8_t xPos2_p,
            cuint8_t yPos2_p,
            const Color color_p = Color::BLACK
    );
    bool_t drawRectangleFill(
            cuint8_t xPos1_p,
            cuint8_t yPos1_p,
            cuint8_t xPos2_p,
            cuint8_t yPos2_p,
            const Color color_p = Color::BLACK
    );
    bool_t drawRoundedRectangle(
            uint8_t xPos1_p,
            uint8_t yPos1_p,
            uint8_t xPos2_p,
            uint8_t yPos2_p,
            uint8_t radius_p,
            const Color color_p = Color::BLACK
    );
    bool_t drawRoundedRectangleFill(
            uint8_t xPos1_p,
            uint8_t yPos1_p,
            uint8_t xPos2_p,
            uint8_t yPos2_p,
            uint8_t radius_p,
            const Color color_p = Color::BLACK
    );

    //     ////////////////////////     TEXT     ////////////////////////     //

    bool_t setFont(
            uint8_t *font_p
    );

    bool_t invertText(bool mode);
    bool_t print(char *st, int x, int y);

private:

    bool_t _drawLineDiagonal(
            cuint8_t xPos1_p,
            cuint8_t yPos1_p,
            cuint8_t xPos2_p,
            cuint8_t yPos2_p,
            const Color color_p = Color::BLACK
    );
    bool_t _drawLineHorizontal(
            cuint8_t xPos_p,
            cuint8_t yPos_p,
            cuint8_t length_p,
            const Color color_p = Color::BLACK
    );
    bool_t _drawLineVertical(
            cuint8_t xPos_p,
            cuint8_t yPos_p,
            cuint8_t length_p,
            const Color color_p = Color::BLACK
    );
    bool_t _write(
            cuint8_t data_p,
            const Mode mode_p
    );
    bool_t _printChar(
            uint8_t c,
            uint8_t x,
            uint8_t y
    );

    // -------------------------------------------------------------------------
    // Properties --------------------------------------------------------------
public:

    // NONE

private:

    //     //////////////////     DEVICE BUS PORTS     //////////////////     //

    ioRegAddress_t  _sclkPinOut;
    ioRegAddress_t  _sclkPinDir;
    ioPinIndex_t    _sclkPinBit;
    ioRegAddress_t  _mosiPinOut;
    ioRegAddress_t  _mosiPinDir;
    ioPinIndex_t    _mosiPinBit;
    ioRegAddress_t  _dataControlPinOut;
    ioRegAddress_t  _dataControlPinDir;
    ioPinIndex_t    _dataControlPinBit;
    ioRegAddress_t  _resetPinOut;
    ioRegAddress_t  _resetPinDir;
    ioPinIndex_t    _resetPinBit;
    ioRegAddress_t  _chipSelectPinOut;
    ioRegAddress_t  _chipSelectPinDir;
    ioPinIndex_t    _chipSelectPinBit;

    //     /////////////////     CONTROL AND STATUS     /////////////////     //

    uint8_t         _contrastValue              : 7;
    bool_t          _isInitialized              : 1;
    bool_t          _isPortsSet                 : 1;
    bool_t          _isSleepModeOn              : 1;
    Error           _lastError;

    //     ///////////////     HARDWARE CONFIGURATION     ///////////////     //

    current_font    _cfont;
    uint8_t         _dataBuffer[504];
};

#endif
