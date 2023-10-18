#include "Player.h"

Zint netSeed;  // Must be synced for coop

Players players;
Zp<Player> player;

void Player::Dereferences() {
}

void Players::ParseReferences() {
}


void Player::Save() {
 players.Save();
}
