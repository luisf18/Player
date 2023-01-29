#include "Player.h"

Player p1;
Player p2;

void setup() {
  
  Serial.begin(115200);
  
  pinMode( 10, OUTPUT );
  pinMode( 11, OUTPUT );
  pinMode( 12, OUTPUT );
  pinMode( 13, OUTPUT );

  p1.play_triangular( 1000, 0 );
  //p1.play_blink( 1000 );
  p2.play_blink( 1000 );

}

void loop() {

  if( p1.update() >= PLAYER_UPDATE ){
    int an = p1.value();
    Serial.println(an);
    analogWrite( 11, map(an,0,1000,0,255) );
  }

  if( p2.update() >= PLAYER_UPDATE ){
    digitalWrite( 13, p2.value() );
  }

}
