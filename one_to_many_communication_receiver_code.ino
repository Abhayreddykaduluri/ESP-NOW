#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH THE MAC Addresses of your receiver ESP32s
uint8_t receiverAddress1[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t receiverAddress2[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t receiverAddress3[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Define a custom message structure
typedef struct struct_message {
  char message[32];
  int messageId;
} struct_message;

// Create a structured object
struct_message myData;

// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: "); Serial.println(macStr);
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery success" : "Delivery fail");
}

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  // Register peers
  esp_now_peer_info_t peerInfo;
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  memcpy(peerInfo.peer_addr, receiverAddress1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer 1");
    return;
  }
  
  memcpy(peerInfo.peer_addr, receiverAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer 2");
    return;
  }
  
  memcpy(peerInfo.peer_addr, receiverAddress3, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer 3");
    return;
  }

  Serial.println("Sender ready. Type a message and press enter to send.");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    if (input.length() > 0) {
      strncpy(myData.message, input.c_str(), sizeof(myData.message));
      myData.message[sizeof(myData.message) - 1] = '\0';
      myData.messageId = millis();

      // Send message to all registered peers
      esp_err_t result = esp_now_send(NULL, (uint8_t *) &myData, sizeof(myData));
      
      if (result == ESP_OK) {
        Serial.println("Message sent to all receivers");
      } else {
        Serial.println("Error sending message");
      }
    }
  }
}
