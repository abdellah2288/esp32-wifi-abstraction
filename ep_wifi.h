#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include <string.h>
#include <stdbool.h>

/*
* inclusiong guard, it's standard pratctice to avoid header files from being included more than once
* will save you a lot of headaches
*/
#ifndef MAIN_WIFI_H_

#define MAIN_WIFI_H_
/* macros for custom access point config */
#define WIFI_AP_SSID "Smart_house"
#define WIFI_AP_PASSWORD "theyearofthelinuxdesktop"
#define WIFI_AP_CHANNEL 1 // Just use channel 1 no need to worry about interference or legal bullshit
#define WIFI_AP_HIDDEN 0 // Don't hide the network, let the feds know you're there
#define WIFI_AP_MAX_CONNECTIONS  12 
#define WIFI_AP_BEACON 100 // Determines how often the beacon (access point) will advertise itself (share relevant info like the SSID AUTH_MODE ... etc)
#define WIFI_AP_IP "192.168.4.1" // Just use the default, no need to get fancy
#define WIFI_AP_GATEWAY "192.168.4.1" // ever visited 192.168.1.1 ? Yeah, that's what a gateway is.
#define WIFI_AP_BW WIFI_BW_HT20 // shamelessly copied. But feel free to do your own research

#endif

typedef struct {
    wifi_ap_record_t * ap_array;
    uint8_t ap_array_size;
} wifi_ap_record_register_t;

/*
* access point structure with 64 char limit on the password and 32 char limit on the ssid as defined by IEEE
*/
typedef struct {
    char ssid[32];
    char password[64];
} access_point_t;

#define DEFAULT_SCAN_LIST_SIZE 20
/**
* Initializes the wifi and ip event loop
*/
void init_wifi_event_loop(void);
/**
* event handler for wifi events, check esp_wifi_types.h for event types. For our purposes this handler only handles connect/disconnect events in station mode
* @param arg data passed to the handler upon call (irrelevant)
* @param event event base in our case it's WIFI_EVENT
* @param event_id event specefic id
* @param event_data
*/
static void wifi_event_handler(void* arg, esp_event_base_t *event, int32_t event_id, void* event_data);
/*
* Initializes non volatile storage
*/
static void initialize_nvs(void);
/**
*Identifies the authentification mode of an access point
* @param authmode, authentification mode from predefined macros in esp_wifi.h
*/
static void print_auth_mode(int authmode);
/**
* Initialize wireless in either station or access point mode [s/a], should only be ran once
* @param mode 's' for station and 'a' for access point
* @param ap access point info for station mode
*/
void init_wifi(char mode, access_point_t ap);
/*
* Scans for access points and returns a wifi_ap_record_register_t pointer requires the wifi to be initialized in station mode
*/
wifi_ap_record_register_t wifi_scan(void);
/*
* Scans for access points and returns a wifi_ap_record_register_t pointer requires the wifi to be initialized in station mode
*/

