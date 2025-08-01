# CragCrowd Gateway - ESP32-S3 LoRa to Internet Bridge

The gateway firmware for CragCrowd's LoRa mesh network, bridging sensor data to the internet.

## 🏗️ Hardware

**Target Board**: LilyGO T3S3 with ESP32-S3 and SX1262 LoRa module

### Pin Configuration
- **LoRa SPI**: SCK=5, MISO=3, MOSI=6, SS=7, RST=8, DIO0=33
- **WiFi**: Built-in ESP32-S3 WiFi
- **Power**: USB-C or external 5V supply

## ⚙️ Functionality

The gateway node:
1. **Receives LoRa packets** from sensor nodes
2. **Connects to WiFi** for internet access
3. **Forwards sensor data** to CragCrowd API via HTTP
4. **Adds gateway metadata** (RSSI, SNR, gateway ID)
5. **Handles connection recovery** automatically

### Data Flow
```
Sensor Node → LoRa → Gateway → WiFi → Internet → API
```

## 🚀 Quick Start

### Prerequisites
- [PlatformIO Core](https://platformio.org/install/cli) or [PlatformIO IDE](https://platformio.org/platformio-ide)
- LilyGO T3S3 board
- WiFi network with internet access
- CragCrowd API running (local or remote)

### Setup
```bash
# Setup development environment
make setup

# Configure WiFi credentials (edit src/main.cpp)
# Set WIFI_SSID and WIFI_PASSWORD

# Build firmware
make build

# Upload to device
make upload

# Monitor serial output
make monitor
```

## 🔧 Configuration

### WiFi Settings
Edit `src/main.cpp`:
```cpp
const char* WIFI_SSID = "Your_WiFi_Network";
const char* WIFI_PASSWORD = "Your_WiFi_Password";
const char* API_ENDPOINT = "http://your-api-server:3000/api/sensor-data";
```

### LoRa Settings
```cpp
// Frequency (must match sensor nodes)
LoRa.begin(915E6);  // 915 MHz for North America

// Optional: Adjust for better range/speed
LoRa.setSpreadingFactor(7);
LoRa.setSignalBandwidth(125E3);
LoRa.setCodingRate4(5);
```

## 📡 LoRa Reception

### Protocol
- **Frequency**: 915 MHz (North America)
- **Reception**: Continuous listening mode
- **Callback**: Interrupt-driven packet processing
- **Error handling**: RSSI/SNR validation

### Data Enhancement
The gateway adds metadata to received packets:
```json
{
  "wall_id": "lower_town_wall",
  "device_count": 8,
  "timestamp": 1704067200000,
  "gateway_id": "AA:BB:CC:DD:EE:FF",
  "rssi": -85,
  "snr": 7.5,
  "received_at": 1704067205000
}
```

## 🌐 Internet Connectivity

### WiFi Management
- **Auto-reconnect**: Handles WiFi disconnections
- **Connection retry**: 30-second retry interval
- **Status monitoring**: LED indicators for connection status

### API Communication
- **HTTP POST**: JSON data to API endpoint
- **Error handling**: Retry logic for failed requests
- **Health monitoring**: Regular API health checks

## 🛠️ Development

### File Structure
```
src/
├── main.cpp           # Main application logic
├── config.h           # Configuration constants
├── wifi_manager.h     # WiFi connection handling
├── lora_handler.h     # LoRa reception wrapper
└── api_client.h       # HTTP API communication
```

### Key Functions
```cpp
void setupLoRa();           // Initialize LoRa receiver
void setupWiFi();           // Connect to WiFi network
void onLoRaReceive();       // Handle incoming LoRa packets
void forwardToAPI();        # Send data to API
void checkWiFiConnection(); // Monitor/restore WiFi
```

## 🔌 Deployment Options

### Permanent Installation
- **Power**: USB adapter or PoE (with converter)
- **Internet**: WiFi or Ethernet (with adapter)
- **Location**: Central position for LoRa coverage
- **Enclosure**: Weatherproof if outdoor deployment

### Mobile/Temporary
- **Power bank**: For temporary installations
- **Mobile hotspot**: Internet via cellular
- **Portable enclosure**: Easy relocation

## 📊 Monitoring

### Serial Output
```bash
make monitor

# Expected output:
# CragCrowd Gateway Starting...
# LoRa Initializing OK!
# WiFi connected!
# IP address: 192.168.1.100
# Gateway setup complete. Listening for sensor data...
# Received LoRa packet: {"wall_id":"test_wall","device_count":5,"timestamp":1704067200}
# HTTP Response: 201 - {"success":true,"id":"...","message":"Sensor data received successfully"}
```

### Status Indicators
- **WiFi LED**: Connection status
- **LoRa LED**: Packet reception
- **API LED**: Successful data forwarding

## 🧪 Testing

### Range Testing
```bash
# Deploy with sensor node
# Monitor reception distance
# Check RSSI/SNR values
# Test in various terrain
```

### API Testing
```bash
# Test API connectivity
curl -X POST http://your-api:3000/api/sensor-data \
  -H "Content-Type: application/json" \
  -d '{"wall_id":"test","device_count":5,"timestamp":1704067200}'
```

## 🔋 Power Consumption

### Typical Usage
- **Active reception**: ~150mA
- **WiFi transmission**: ~200mA peak
- **Idle**: ~50mA
- **Deep sleep**: ~10mA (if implemented)

### Power Optimization
```cpp
// Reduce WiFi power
WiFi.setTxPower(WIFI_POWER_8_5dBm);

// LoRa power management
LoRa.setPreambleLength(6);  // Shorter preamble
LoRa.setTxPower(14);        // Reduce if not needed
```

## 🌐 Network Architecture

### Single Gateway
```
[Sensor] --LoRa--> [Gateway] --WiFi--> [Internet] --> [API]
```

### Multiple Gateways
```
[Sensor] --LoRa--> [Gateway A] --WiFi--> [Internet] --> [API]
[Sensor] --LoRa--> [Gateway B] --4G----> [Internet] --> [API]
[Sensor] --LoRa--> [Gateway C] --Ethernet--> [Internet] --> [API]
```

## 🐛 Troubleshooting

### WiFi Issues
```cpp
// Check WiFi status
if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, reconnecting...");
    setupWiFi();
}
```

### LoRa Issues
```cpp
// Check LoRa initialization
if (!LoRa.begin(915E6)) {
    Serial.println("LoRa init failed!");
    // Check SPI connections
}
```

### API Issues
```bash
# Test API directly
curl http://your-api:3000/api/health

# Check network connectivity
ping your-api-server
```

## 🔐 Security

### Network Security
- **WPA2/WPA3**: Use secure WiFi networks
- **VPN**: Consider VPN for remote deployments
- **Firewall**: Restrict outbound connections if needed

### Data Security
- **HTTPS**: Use HTTPS for API communication
- **API keys**: Implement API authentication
- **Rate limiting**: Prevent data flooding

## 📈 Scaling

### Coverage Area
- **Single gateway**: 2-5km radius
- **Multiple gateways**: Overlapping coverage
- **Mesh networking**: Future Meshtastic integration

### Data Volume
- **Bandwidth**: 1-10 packets/minute typical
- **Storage**: Minimal gateway storage needed
- **Processing**: Real-time forwarding

## 🤝 Contributing

### Development Setup
1. Clone repository
2. Configure WiFi credentials
3. Set API endpoint
4. Test with local API
5. Deploy and monitor

### Testing Checklist
- [ ] LoRa reception working
- [ ] WiFi connection stable
- [ ] API communication successful
- [ ] Error recovery functional
- [ ] Range testing completed

## 📄 License

MIT License - see [LICENSE](../LICENSE) file for details.

## 🔗 Related Projects

- **Sensor**: [cragcrowd-firmware](../cragcrowd-firmware/)
- **API**: [cragcrowd-api](../cragcrowd-api/)
- **Web UI**: [cragcrowd-web-ui](../cragcrowd-web-ui/)

## 📧 Support

- **WiFi connectivity**: Check router settings and signal strength
- **LoRa reception**: Verify frequency and antenna connections
- **API issues**: Check network connectivity and API status