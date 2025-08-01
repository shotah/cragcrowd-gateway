## **CragCrowd: LoRa-Powered Crag Activity Monitor (No Cell Service Needed)**

### **🧗 The Problem**

Climbing areas are getting more crowded—but there's no way to know how busy a crag is until you're standing beneath it. Especially in places with **no cell reception**, climbers are flying blind.

Land managers lack reliable usage data to support trail maintenance, parking, and sustainable access. And climbers end up stacking on top of each other.

---

### **💡 The Solution**

**CragCrowd** is a lightweight, solar-powered device that passively counts how many phones are nearby—giving climbers and orgs a real-time sense of how crowded a wall is, even in offline areas.

Using open-source [Paxcounter](https://github.com/LilyGO/ESP32-Paxcounter) software on an ESP32 LoRa board, CragCrowd detects nearby mobile devices and sends anonymized counts over LoRa radio (Meshtastic-compatible) to a community network or internet gateway.

---

### **⚙️ How It Works**

* The node passively scans for WiFi/BLE “pings” from phones

* Each detected device is deduplicated by MAC address (randomized MACs are still useful\!)

Every 10–15 minutes, it broadcasts a message like:

 json  
CopyEdit  
`{ wall_id: "index_lower", devices: 13, time: "10:45" }`

*   
* Any nearby Meshtastic device (stationary or climber-carried) can relay the data

* Data is collected and visualized in a shared dashboard for climbers or orgs

---

### **🔐 Key Features**

* **No Cameras, No Mics**: 100% passive and privacy-respecting

* **No WiFi or Cell Needed**: Uses LoRa mesh radios

* **Low Power**: Solar \+ 18650 keeps it running all season

* **Anonymous \+ Open**: No personal data collected; everything open source

* **Real Trends, Not Just Guesses**: Hourly presence tracking instead of noisy entry sensors

---

### **📈 Use Cases**

* Climbers check crag activity before leaving the house

* Rangers track usage over time and justify maintenance budgets

* Access orgs monitor real impact without invasive tools

* App developers integrate live crag data into route guides

* Community members host or relay data via their own Meshtastic nodes

---

### **🔧 Prototype Hardware**

We're using the [**LilyGO T3S3 board with SX1262 (915 MHz)**](https://lilygo.cc/products/t3-s3-meshtastic?variant=45348460626101) for initial development. This board includes:

* ESP32‑S3 MCU with Wi-Fi, BLE, and LoRa

* USB‑C power and charging

* Optional OLED and microSD support

* 915 MHz SX1262 LoRa module (ideal for North America)

* Designed for compatibility with both Paxcounter and Meshtastic firmware

This board offers excellent power efficiency, supports long-range communication, and is already widely used in the community for similar mesh deployments.

---

### **💬 What's Next?**

We're testing the first batch of CragCrowd nodes at climbing areas in WA.  
 We’re looking for:

* Feedback from climbers: would you use this?

* Suggestions on where to deploy first

* Hosts for nodes, or folks with Meshtastic gear already

* Volunteers to help visualize the data or build the web UI

**Let us know what you think → christopherblodgett@gmail.com**

---

### **🔗 Learn More:**

* Paxcounter firmware: [https://github.com/LilyGO/ESP32-Paxcounter](https://github.com/LilyGO/ESP32-Paxcounter)

* LilyGO T3S3 board: [https://lilygo.cc/products/t3-s3-meshtastic?variant=45348460626101](https://lilygo.cc/products/t3-s3-meshtastic?variant=45348460626101)

* Meshtastic (LoRa mesh radios): https://meshtastic.org

* Project page: [https://github.com/shotah/cragcrowd](https://github.com/shotah/cragcrowd) *(coming soon)*

---

### **🔭 Looking Ahead**

Future versions of CragCrowd may support **multi-node triangulation**, enabling even more accurate wall-specific usage estimates—without GPS, apps, or user tracking. We're exploring lightweight, privacy-first ways to map presence across complex areas like Lower Town Wall and beyond.

---

