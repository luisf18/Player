/* =====================================================
 *  Player Library
 *    Author: luisf18 (github)
 *    Ver.: 0.0.1
 *    last_update: 04/12/2022
 * =====================================================
 */

#include "Player.h"
  

// Player functions ============================================================
int  Player_blink( uint16_t i ) { return ( i == 0 ); }
int  Player_blink_n( uint16_t i, uint16_t MAX ) { return ( (MAX-5) > i ? (i+1)%2 : 0 ); }
int  Player_square( uint16_t i, uint16_t MAX, uint16_t key ) { return ( key > i ? 1000 : 0 ); }
// void  Player_rise(  uint16_t i ) { return i; }
// void  Player_fall(  uint16_t i, uint16_t MAX ) { return MAX - i; }
int  Player_cos(   uint16_t i, uint16_t MAX ) { return 1000*cos( i*(2*PI/(float)MAX) ); }
int  Player_sin(   uint16_t i, uint16_t MAX ) { return 1000*sin( i*(2*PI/(float)MAX) ); }
int  Player_sawtooth(    uint16_t i, uint16_t MAX ) { return ((long)1000*i)/MAX; }
int  Player_triangular(  uint16_t i, uint16_t MAX ) { return ( (long) 2000*( i > (MAX/2) ? MAX - i : i ) ) / MAX; }


// Control player =============================================
void Player::reset(){ reset(0);}
void Player::reset(uint16_t i){
  player_i = i;
  timeout = 0;
  player_1_cb = nullptr;
  player_2_cb = nullptr;
  player_3_cb = nullptr;
  player_4_cb = nullptr;
  Repeat_i = Repeat;
}

void Player::stop(){ Playing = false; }
void Player::play(){ Playing = true;  update(); }

// Start player =================================================
void Player::set ( uint16_t len, int (*f)(uint16_t                  ) ){ reset(); player_1_cb = f; player_len = len; }
void Player::set ( uint16_t len, int (*f)(uint16_t,uint16_t         ) ){ reset(); player_2_cb = f; player_len = len; }
void Player::set ( uint16_t len, int (*f)(Player*                   ) ){ reset(); player_4_cb = f; player_len = len; }
void Player::play( uint16_t len, int (*f)(uint16_t                  ) ){ set(len,f); play(); }
void Player::play( uint16_t len, int (*f)(uint16_t,uint16_t         ) ){ set(len,f); play(); }
void Player::play( uint16_t len, int (*f)(Player*                   ) ){ set(len,f); play(); }
void Player::set_by_key(  uint16_t len, uint16_t _key, int (*f)(uint16_t,uint16_t,uint16_t) ){ reset(); player_3_cb = f; player_len = len; key = _key; }
void Player::play_by_key( uint16_t len, uint16_t _key, int (*f)(uint16_t,uint16_t,uint16_t) ){ set_by_key(len,_key,f); play(); }


void Player::loop_on() { Loop = true; }
void Player::loop_off(){ repeat(0); }
void Player::repeat(uint16_t n){
  Repeat = n;
  Repeat_i = n;
  Loop = false;
}

uint16_t Player::len() { return player_len; }
int      Player::value() { return Value; }
uint16_t Player::track(){ return player_i; }
uint16_t Player::track(uint16_t i){ player_i = i; return track(); }
uint8_t Player::update(){
  if( Playing ){
    unsigned long t = (time_us?micros():millis());
    if( t >= timeout ){
      if( player_i >= player_len ){ // Detecta que terminou a sequência
        player_i = 0;
        if( !Loop ){
          if(Repeat_i == 0){
            Playing = false;
            return PLAYER_OFF;
          }else{
            Repeat_i --;
          }
        }
      }

      // Call function
      uint16_t _dt = 0;
           if(player_1_cb != nullptr) Value = player_1_cb(player_i);
      else if(player_2_cb != nullptr) Value = player_2_cb(player_i,player_len);
      else if(player_3_cb != nullptr) Value = player_3_cb(player_i,player_len,key);
      else if(player_4_cb != nullptr) Value = player_4_cb(this);

      // Updadte timeout
      timeout = t + (time_dt?dt:Value);

      player_i++;
      return PLAYER_UPDATE + (player_i == player_len);
    }
  }
  return Playing;
}

// dt ==========================================================================
void  Player::time_in_ms() { time_us = false; }
void  Player::time_in_us() { time_us = true;  }
void  Player::time_dt_on() { time_dt = true;  }
void  Player::time_dt_off(){ time_dt = false; }
void  Player::set_time_dt(uint16_t _dt){ dt = _dt; time_dt_on(); }

// Default Player functions ====================================================
void  Player::play_blink( uint16_t periode ){ set_time_dt( periode/2 ); play( 2, Player_blink ); }
void  Player::play_blink_n( uint16_t n, uint16_t one_pulse_ms ){ set_time_dt( one_pulse_ms/2 ); play( 2*(n+5), Player_blink_n ); }
void  Player::play_square( uint16_t periode, uint16_t dutycicle ){ set_time_dt( periode/wav_len ); play_by_key( wav_len, dutycicle, Player_square ); }
void  Player::play_sin(        uint16_t periode, uint16_t phase_deg ){ play_wave( periode, (wav_len*phase_deg)/360.0, wav_len, Player_sin        ); }
void  Player::play_cos(        uint16_t periode, uint16_t phase_deg ){ play_wave( periode, (wav_len*phase_deg)/360.0, wav_len, Player_cos        ); }
void  Player::play_triangular( uint16_t periode, uint16_t phase_i   ){ play_wave( periode,                   phase_i, wav_len, Player_triangular ); }
void  Player::play_sawtooth(   uint16_t periode, uint16_t phase_i   ){ play_wave( periode,                   phase_i, wav_len, Player_sawtooth   ); }

void  Player::play_wave( uint16_t periode, uint16_t phase_i, uint16_t len, int (*f)(uint16_t) ){
  set_time_dt(periode/len);
  reset( phase_i );
  player_len = len;
  player_1_cb = f;
  play();
}

void  Player::play_wave( uint16_t periode, uint16_t phase_i, uint16_t len, int (*f)(uint16_t,uint16_t) ){
  set_time_dt(periode/len);
  reset( phase_i );
  player_2_cb = f;
  player_len = len;
  play();
}
