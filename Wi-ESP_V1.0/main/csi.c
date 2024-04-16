#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include <string.h>
#include <stdio.h>
#include "structures.h"



#define MAC_ADD_LEN 20

/*Set the SSID and Password via "make menuconfig"*/
#define DEFAULT_SSID CONFIG_WIFI_SSID
#define DEFAULT_PWD CONFIG_WIFI_PASSWORD

#if CONFIG_WIFI_ALL_CHANNEL_SCAN
#define DEFAULT_SCAN_METHOD WIFI_ALL_CHANNEL_SCAN
#elif CONFIG_WIFI_FAST_SCAN
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#else
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#endif /*CONFIG_SCAN_METHOD*/

#if CONFIG_WIFI_CONNECT_AP_BY_SIGNAL
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#elif CONFIG_WIFI_CONNECT_AP_BY_SECURITY
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SECURITY
#else
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#endif /*CONFIG_SORT_METHOD*/

#if CONFIG_FAST_SCAN_THRESHOLD
#define DEFAULT_RSSI CONFIG_FAST_SCAN_MINIMUM_SIGNAL
#if CONFIG_EXAMPLE_OPEN
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#elif CONFIG_EXAMPLE_WEP
#define DEFAULT_AUTHMODE WIFI_AUTH_WEP
#elif CONFIG_EXAMPLE_WPA
#define DEFAULT_AUTHMODE WIFI_AUTH_WPA_PSK
#elif CONFIG_EXAMPLE_WPA2
#define DEFAULT_AUTHMODE WIFI_AUTH_WPA2_PSK
#else
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#endif
#else
#define DEFAULT_RSSI -127
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#endif /*CONFIG_FAST_SCAN_THRESHOLD*/

static const char *TAG = "scan";
static bool Print_Info = 1;


static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id) {
        case SYSTEM_EVENT_STA_START:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
            ESP_ERROR_CHECK(esp_wifi_connect());
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
            ESP_LOGI(TAG, "Got IP: %s\n",
                     ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
            ESP_ERROR_CHECK(esp_wifi_connect());
            break;
        default:
            break;
    }
    return ESP_OK;
}

/*
 * This function ONLY receive the CSI preamble of frames (if there is any)
 */

void receive_csi_cb(void *ctx, wifi_csi_info_t *Rx_Data) {
							
	// Goal : Get Channel State Information Packets and fill fields accordingly In : Contexte (null), CSI packet  Out : Null, Fill fields of corresponding AP
	wifi_csi_info_t Rx = Rx_Data[0];
	
	char MacAddTx[MAC_ADD_LEN] = {0}; // Sender

	char APMAC[MAC_ADD_LEN] = {'3','C',':','4','6',':','A','A',':','3','B',':','D','F',':','1','A'}; // APMACAddress
	char ClientMAC[MAC_ADD_LEN] = {'A','6',':','C','A',':','1','2',':','6','4',':','7','A',':','D','8'}; // ClientMACAddress
	char targetMacChr[MAC_ADD_LEN] = {'3','B',':','7','1',':','C','F',':','4','3',':','E','F',':','1','9'}; // APMACAddress
	
	sprintf(MacAddTx, "%02X:%02X:%02X:%02X:%02X:%02X", Rx.mac[0], Rx.mac[1], Rx.mac[2], Rx.mac[3], Rx.mac[4], Rx.mac[5]);
	
	if (Rx.rx_ctrl.sig_mode==1 
		/*&& 
		MacAddTx[0]==targetMacChr[0] && 
		MacAddTx[1]==targetMacChr[1] && 
		MacAddTx[2]==targetMacChr[2] && 
		MacAddTx[3]==targetMacChr[3] && 
		MacAddTx[4]==targetMacChr[4] && 
		MacAddTx[5]==targetMacChr[5] &&
		MacAddTx[6]==targetMacChr[6] &&
		MacAddTx[7]==targetMacChr[7] &&
		MacAddTx[8]==targetMacChr[8] &&
		MacAddTx[9]==targetMacChr[9] &&
		MacAddTx[10]==targetMacChr[10] &&
		MacAddTx[11]==targetMacChr[11] &&
		MacAddTx[12]==targetMacChr[12] &&
		MacAddTx[13]==targetMacChr[13] &&
		MacAddTx[14]==targetMacChr[14] &&
		MacAddTx[15]==targetMacChr[15] &&
		MacAddTx[16]==targetMacChr[16]*/
			)
			
	{
		

		//printf("Rx CSI from adress %s\n", MacAddTx); 
		//printf("Rece   CSI from adress %s\n", MacAddTx); 
		bool dsplay=0;
		if(dsplay)
		{
			printf("Target CSI from adress %s\n", targetMacChr); 
			printf("Rece   CSI from adress %s\n", MacAddTx); 
			//printf("Following packet :\n");
			printf("Rate	%d", Rx.rx_ctrl.rate);
			printf("RSSI	%d\n", Rx.rx_ctrl.rssi);
			printf("sig_mode %d -> ",Rx.rx_ctrl.sig_mode);
			if (Rx.rx_ctrl.sig_mode==0)printf("non HT(11bg)\n");
			if (Rx.rx_ctrl.sig_mode==1)printf("HT(11n)\n");
			if (Rx.rx_ctrl.sig_mode==2)printf("UNKNOWN!!!");
			if (Rx.rx_ctrl.sig_mode==3)printf("VHT(11ac)\n");

			printf("HT20 (0) or HT40 (1): %d\n",Rx.rx_ctrl.cwb);
			printf("Space time block present: %d\n", Rx.rx_ctrl.stbc);
			printf("Secondary channel : 0: none; 1: above; 2: below: %d\n", Rx.rx_ctrl.secondary_channel);
			printf("Length %d\n", Rx.len);
			printf("Last word is invalid: %d\n", Rx.rx_ctrl.rx_state);
			printf("First four bytes Invalid or not: %d\n", Rx.first_word_invalid);
			printf("<CSI><address>%s</address><len>%d</len>", MacAddTx, Rx_Data->len);
		}

		int8_t* my_ptr=Rx_Data->buf;
		bool ApFrame=false;
		bool CLFrame=false;

		///////////////////////////////////////////////////////////////////////////////////////////////
		if (MacAddTx[0]== APMAC[0] && 		MacAddTx[1]== APMAC[1] && 		MacAddTx[2]== APMAC[2] && 		MacAddTx[3]== APMAC[3] && 		MacAddTx[4]== APMAC[4] && 		MacAddTx[5]== APMAC[5] &&		MacAddTx[6]== APMAC[6] &&
			MacAddTx[7]== APMAC[7] &&		MacAddTx[8]== APMAC[8] &&		MacAddTx[9]== APMAC[9] &&		MacAddTx[10]==APMAC[10] &&		MacAddTx[11]==APMAC[11] &&		MacAddTx[12]==APMAC[12] &&		MacAddTx[13]==APMAC[13] &&
			MacAddTx[14]==APMAC[14] &&		MacAddTx[15]==APMAC[15] &&		MacAddTx[16]==APMAC[16])
		{
			printf("0:");
			ApFrame=true;
			CLFrame=false;			
		}
		else if (MacAddTx[0]== ClientMAC[0] && 		MacAddTx[1]== ClientMAC[1] && 		MacAddTx[2]== ClientMAC[2] &&		MacAddTx[3]== ClientMAC[3] && 		MacAddTx[4]== ClientMAC[4] && 		MacAddTx[5]== ClientMAC[5] &&
			MacAddTx[6]== ClientMAC[6] &&		MacAddTx[7]== ClientMAC[7] &&		MacAddTx[8]== ClientMAC[8] &&		MacAddTx[9]== ClientMAC[9] &&		MacAddTx[10]==ClientMAC[10] &&		MacAddTx[11]==ClientMAC[11] &&
			MacAddTx[12]==ClientMAC[12] &&		MacAddTx[13]==ClientMAC[13] &&		MacAddTx[14]==ClientMAC[14] &&		MacAddTx[15]==ClientMAC[15] &&		MacAddTx[16]==ClientMAC[16])
		{
			printf("1:");
			ApFrame=false;
			CLFrame=true;				
		}
		else
		{
			printf("2:");
			ApFrame=false;
			CLFrame=false;			
		}
		///////////////////////////////////////////////////////////////////////////////////////////////


		printf("%s", MacAddTx); 
		printf(":");
		//if(CLFrame)
		{
			for(int a=0; a<Rx_Data->len; a++)
			{
			//printf("%d:",my_ptr[a]);
			}
			printf("%u:", Rx.rx_ctrl.timestamp);
			printf("\n");
		}
		vTaskDelay(1);

	} 
	
	else 
	{
		//printf("Invalid: CSI");
	}
	
	
}

/*
 * This function receive all frames, would they contain CSI preamble or not.
 * It gets the content of the frame, not the preamble.
 */

void promi_cb(void *buff, wifi_promiscuous_pkt_type_t type) 
{
	if (Print_Info)
	{
		Print_Info=0;
		///////////////////////////////////////////////////////////////////////////////////////////////
		const wifi_promiscuous_pkt_t *PayloadPacket = (wifi_promiscuous_pkt_t *)buff;
		const wifi_ieee80211_packet_t *InfoPacket = (wifi_ieee80211_packet_t *)PayloadPacket->payload;
		const wifi_ieee80211_mac_hdr_t *Header = &InfoPacket->hdr;
		uint8_t* data_ptr = InfoPacket;
		char MacAddTx[MAC_ADD_LEN] = {0}; 
		char MacAddRx[MAC_ADD_LEN] = {0};
		///////////////////////////////////////////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////////////////////////////////////////
		sprintf(MacAddRx, "%02X:%02X:%02X:%02X:%02X:%02X", Header->addr1[0], Header->addr1[1], Header->addr1[2], Header->addr1[3], Header->addr1[4], Header->addr1[5]);
		sprintf(MacAddTx, "%02X:%02X:%02X:%02X:%02X:%02X", Header->addr2[0], Header->addr2[1], Header->addr2[2], Header->addr2[3], Header->addr2[4], Header->addr2[5]);
		///////////////////////////////////////////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////////////////////////////////////////
		//Display Valid Frames only
		//if (PayloadPacket->rx_ctrl.sig_mode>0)
		//{
			//printf("Rx Data packet from %s to %s\n", MacAddTx, MacAddRx);
			//for (int i=0;i<PayloadPacket->rx_ctrl.sig_len;i++)printf("%02x ", data_ptr[i]);
			//printf("\n\n");
		//}
		///////////////////////////////////////////////////////////////////////////////////////////////
	}
	Print_Info = 1;
}

void app_main()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
	{
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );
    sleep(1);
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));



	///////////////////////////////////////////////////////////////////////////////////////////////
	wifi_init_config_t Configuration = WIFI_INIT_CONFIG_DEFAULT();
	Configuration.csi_enable = 1;	
	
	ESP_ERROR_CHECK(esp_wifi_init(&Configuration));
	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
	///////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////
	wifi_promiscuous_filter_t Promicus_Filter;
	wifi_promiscuous_filter_t Promicus_Filter_Crl;
	
	uint32_t filter_promi_field=WIFI_PROMIS_FILTER_MASK_ALL;	
	uint32_t filter_promi_ctrl_field = (0xFF800000); 
	uint32_t filter_event = WIFI_EVENT_MASK_ALL;
   
	Promicus_Filter.filter_mask = filter_promi_field;
	Promicus_Filter_Crl.filter_mask = filter_promi_ctrl_field;
	
	esp_wifi_set_promiscuous_filter(&Promicus_Filter);
	esp_wifi_set_event_mask(filter_event);
	esp_wifi_set_promiscuous_ctrl_filter(&Promicus_Filter_Crl);
	
	ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
	esp_wifi_set_promiscuous_rx_cb(promi_cb);
	
	ESP_ERROR_CHECK(esp_wifi_set_csi(1));
	///////////////////////////////////////////////////////////////////////////////////////////////




	///////////////////////////////////////////////////////////////////////////////////////////////
	//CSI Configuration
	//ESP32 offer some settings which can be configured Before Capturing
	wifi_csi_config_t CSI_Config; // CSI = Channel State Information
	CSI_Config.lltf_en = 1;//1
	CSI_Config.htltf_en = 1;//1
	CSI_Config.stbc_htltf2_en = 1;//1
	CSI_Config.ltf_merge_en = 1;//1
	CSI_Config.channel_filter_en = 0;//1
	CSI_Config.manu_scale = 0; // Automatic scalling
	//CSI_Config.shift=15; // 0->15
	ESP_ERROR_CHECK(esp_wifi_set_csi_config(&CSI_Config));
	ESP_ERROR_CHECK(esp_wifi_set_csi_rx_cb(&receive_csi_cb, NULL));
	///////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////
	// AP Configrations are required to acquire CSI Packets from the RF Cahnnel
	wifi_config_t AP_Configuration;
	ESP_ERROR_CHECK(esp_wifi_get_config(WIFI_IF_AP, &AP_Configuration));
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &AP_Configuration)); 
	ESP_ERROR_CHECK(esp_wifi_set_protocol(WIFI_IF_AP, WIFI_PROTOCOL_11B| WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N));
	///////////////////////////////////////////////////////////////////////////////////////////////



	esp_log_level_set("TEST", ESP_LOG_VERBOSE);
	sleep(1);
	

	int WiFi_Channel;
	WiFi_Channel = 1;
	//There are 14 channel on WiFi RF Interface. Select that which is configured on the AP
	//int bandwith = 0;
	//
	int Ch_HighThrogh;
	// There are three possible in case of Channel This is usuallu used in case of Channel width is 40 MHz
	//WIFI_SECOND_CHAN_NONE;//WIFI_SECOND_CHAN_ABOVE or WIFI_SECOND_CHAN_BELOW
	Ch_HighThrogh = WIFI_SECOND_CHAN_NONE;
	
	ret = esp_wifi_set_channel(WiFi_Channel, Ch_HighThrogh);
	ESP_ERROR_CHECK(ret);

	printf("Configuration Complete...\n");
	
}














