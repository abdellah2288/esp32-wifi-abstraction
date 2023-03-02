
#include "ep_wifi.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_event.h"
#include <string.h>
// check the header file first

static const char *TAG = "scan";

static void initialize_nvs(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret ==ESP_ERR_NVS_NO_FREE_PAGES  || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );
}

static void wifi_event_handler(void* arg, esp_event_base_t *event, int32_t event_id, void* event_data)
{
    switch(event_id)
    {
        case WIFI_EVENT_STA_CONNECTED:
            printf("Connected \n");
            break;  
        case WIFI_EVENT_STA_DISCONNECTED:
            printf("Disconnected \n");
            break;
        default:
            printf("%ld \n",event_id);
            break;
    }
}

void init_wifi_event_loop(void)
{
    
    ESP_ERROR_CHECK(esp_event_loop_create_default()); // creates an event catch/handle loop
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
}
 // checks for the authentifcation mode, not really useful since you can pretty much find out on your own using 
 // a phone or a computer.
static void print_auth_mode(int authmode)
{
    switch (authmode) {
    case WIFI_AUTH_OPEN:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_OPEN");
        break;
    case WIFI_AUTH_OWE:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_OWE");
        break;
    case WIFI_AUTH_WEP:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WEP");
        break;
    case WIFI_AUTH_WPA_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA_PSK");
        break;
    case WIFI_AUTH_WPA2_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_PSK");
        break;
    case WIFI_AUTH_WPA_WPA2_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA_WPA2_PSK");
        break;
    case WIFI_AUTH_WPA2_ENTERPRISE:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_ENTERPRISE");
        break;
    case WIFI_AUTH_WPA3_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA3_PSK");
        break;
    case WIFI_AUTH_WPA2_WPA3_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_WPA3_PSK");
        break;
    default:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_UNKNOWN");
        break;
    }
}

void init_wifi(char mode , access_point_t ap )
{
    initialize_nvs();
    ESP_ERROR_CHECK(esp_netif_init()); //initializes  the network stack
    init_wifi_event_loop();
    wifi_init_config_t cfg_default = WIFI_INIT_CONFIG_DEFAULT(); // default config to use when in station mode (Nobody cares since you can't have le funny SSID)
    wifi_config_t cfg_ap = {
        .ap = {
        .ssid = WIFI_AP_SSID,
        .password =WIFI_AP_PASSWORD,
        .channel = WIFI_AP_CHANNEL,
        .ssid_hidden = WIFI_AP_HIDDEN,
        .max_connection = WIFI_AP_MAX_CONNECTIONS,
        .beacon_interval = WIFI_AP_BEACON,   
        },
    };
    wifi_config_t cfg_sta = {
            .sta = {
                .ssid ="",
                .password = "",
            },
            };
    memcpy((cfg_sta.sta).ssid, ap.ssid,32);
    memcpy((cfg_sta.sta).password, ap.password,64);
    ESP_ERROR_CHECK(esp_wifi_init(&cfg_default));
    switch(mode)
    {
        
        case 's':
        esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
        assert(sta_netif);


        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA)); // set wifi to "sta" mode ie: a wifi station (eg: a laptop is a wifi station) 
        ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &cfg_sta));
        ESP_ERROR_CHECK(esp_wifi_start()); // start the wifi according to the current configuration 
        ESP_ERROR_CHECK(esp_wifi_connect());
        break;
        case 'a':
        
        esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();
        assert(ap_netif);
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP)); 
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP,&cfg_ap));
        ESP_ERROR_CHECK(esp_wifi_start()); // start the wifi according to the current configuration 
        break;      
    }

}
/* Requires the esp to be in station mode*/
wifi_ap_record_register_t wifi_scan(void)
{
	
    wifi_ap_record_register_t register_;
    uint16_t number = DEFAULT_SCAN_LIST_SIZE;
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE]; // a list containing informations about the  available networks (access points)
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));

    esp_wifi_scan_start(NULL, true); //starts scanning for networks, takes two arguments the config (set to NULL for defaults) and block ie: whether or not the scan will block the caller until it's done 
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
    register_.ap_array = ap_info;
    register_.ap_array_size = DEFAULT_SCAN_LIST_SIZE;
    return register_; 
}
