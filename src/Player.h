/* =====================================================
 *  Player Library
 *    Author: luisf18 (github)
 *    Ver.: 0.0.1
 *    last_update: 04/12/2022
 * =====================================================
 */

#ifndef PLAYER_H
#define PLAYER_H
#include <Arduino.h>

#define PLAYER_DEFAULT_COUNT 50

#define PLAYER_OFF       0
#define PLAYER_PLAYING   1
#define PLAYER_UPDATE    2
#define PLAYER_LAST_STEP 3

class Player {

private:
    
    // Player
    boolean       Playing    = false;
    boolean       Loop       = true;
    uint16_t      Repeat     = 0;
    uint16_t      Repeat_i   = 0;
    unsigned long timeout    = 0;
    uint16_t      player_i   = 0;
    uint16_t      player_len = 0;
    
    // Time
    boolean       time_us    = false;
    boolean       time_dt    = false;
    uint16_t      dt         = 0;

    // Value
    int      Value = 0;
    uint16_t key   = 0;

    // Callback functions
    int (*player_1_cb)(uint16_t                  ) = nullptr;
    int (*player_2_cb)(uint16_t,uint16_t         ) = nullptr;
    int (*player_3_cb)(uint16_t,uint16_t,uint16_t) = nullptr;
    int (*player_4_cb)(Player*                   ) = nullptr;
  
public:

    // Value
    uint16_t wav_len = 100;

    // Control player
    void  reset();
    void  reset(uint16_t i);
    void  stop();
    void  play();
    void  loop_on();
    void  loop_off();
    void  repeat(uint16_t n);

    // Return
    uint16_t len();
    int      value();
    uint16_t track();
    uint16_t track(uint16_t i);
    uint8_t  update();
    
    // Begin player
    void  set ( uint16_t len, int (*f)(uint16_t         ) );
    void  set ( uint16_t len, int (*f)(uint16_t,uint16_t) );
    void  set ( uint16_t len, int (*f)(Player*          ) );
    void  play( uint16_t len, int (*f)(uint16_t         ) );
    void  play( uint16_t len, int (*f)(uint16_t,uint16_t) );
    void  play( uint16_t len, int (*f)(Player*          ) );

    void  set_by_key(  uint16_t len, uint16_t _key, int (*f)(uint16_t,uint16_t,uint16_t) );
    void  play_by_key( uint16_t len, uint16_t _key, int (*f)(uint16_t,uint16_t,uint16_t) );

    // Time conf.
    void  time_in_ms();
    void  time_in_us();
    void  time_dt_on();
    void  time_dt_off();
    void  set_time_dt(uint16_t _dt);

    // Player
    void  play_blink( uint32_t periode );
    void  play_blink_n(    uint32_t n,       uint16_t one_pulse_ms );
    void  play_square(     uint32_t periode, uint16_t dutycicle );
    void  play_sin(        uint32_t periode, uint16_t phase_deg );
    void  play_cos(        uint32_t periode, uint16_t phase_deg );
    void  play_triangular( uint32_t periode, uint16_t phase_deg );
    void  play_sawtooth(   uint32_t periode, uint16_t phase_deg );
    void  play_wave( uint32_t periode, uint16_t phase_deg, uint16_t len, int (*f)(uint16_t) );
    void  play_wave( uint32_t periode, uint16_t phase_deg, uint16_t len, int (*f)(uint16_t,uint16_t) );
      
};

#endif