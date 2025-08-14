#ifndef NETWORK_H
#define NETWORK_H

extern uint64_t last_post_time, last_get_time;
extern const uint64_t post_time, get_time;

void WifiInit();
void postData(uint64_t now);
void postState(String device, uint8_t device_state);
void getState(uint64_t now);

#endif