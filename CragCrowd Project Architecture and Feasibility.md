### **CragCrowd Project Architecture and Feasibility**

This document outlines the project's feasibility, required software packages, and a proposed GitHub repository structure for the CragCrowd project.

---

#### **1\. Project Feasibility**

The CragCrowd project is considered feasible due to its reliance on established open-source hardware and software. Key points supporting feasibility include:

* **Existing Components**: The project uses readily available hardware (e.g., LilyGO T3S3 with an ESP32-S3 microcontroller) and well-supported open-source software (Paxcounter, Meshtastic).  
* **Privacy-Respecting Design**: The system does not use cameras or microphones. It only collects anonymous data, which is a significant advantage for user adoption.  
* **Community Support**: The project seeks community involvement for development, which can accelerate progress and foster a sustainable ecosystem.

A primary challenge lies in the reliance on a Meshtastic community network for data relay. The effectiveness of the system is dependent on the density of Meshtastic devices acting as internet gateways in climbing areas.

---

#### **2\. Software Packages**

The project requires the following software components, which can be developed and maintained separately:

* **Sensor Node Firmware**:  
  * **Purpose**: The code that runs on the CragCrowd device itself.  
  * **Functionality**: Scans for Wi-Fi and BLE signals, counts devices, and broadcasts anonymized data over LoRa to the Meshtastic mesh network.  
  * **Technology**: Developed in **C++** using the PlatformIO framework, leveraging its performance and low-power capabilities.  
* **LoRa to Internet Gateway/Bridge**:  
  * **Purpose**: A Meshtastic-compatible device with an internet connection.  
  * **Functionality**: Acts as an "exit point" for the Meshtastic network, receiving LoRa messages from the sensor nodes and forwarding them to the internet via an MQTT broker.  
  * **Technology**: This device would be configured using the C++ Meshtastic firmware.  
* **API and Database Layer**:  
  * **Purpose**: The backend system for data management.  
  * **Functionality**: Receives data from the internet gateways, stores it in a database, and provides an API for the UI to retrieve aggregated information.  
  * **Technology**: The core API would be in **TypeScript**. Dedicated microservices in **Python** could be added for AI/ML tasks (e.g., traffic prediction) or in **Go** for high-performance, compiled operations if needed.  
* **User Interface (UI)**:  
  * **Purpose**: A user-facing dashboard.  
  * **Functionality**: Visualizes real-time and historical crag traffic data in a clear and accessible format.  
  * **Technology**: The web interface would be built using a framework like React or Vue and written in **TypeScript**.  
    ---

    #### **3\. GitHub Architecture**

A multi-repository approach is recommended to organize the project's development and support community contributions.

* **`cragcrowd-firmware`**  
  * **Purpose**: To manage the firmware and configuration for the physical sensor nodes.  
  * **Content**: Source code, build scripts, and device-specific configuration files in **C++**.  
* **`cragcrowd-gateway`**  
  * **Purpose**: A dedicated repository for the internet gateway software.  
  * **Content**: Code to configure a Meshtastic device to act as an MQTT bridge, written in **C++**.  
* **`cragcrowd-api`**  
  * **Purpose**: For the backend services.  
  * **Content**: API code in **TypeScript**, database schema definitions, and data processing logic. This repository could also contain separate microservices in **Go** for compiled resources or **Python** for AI/ML tasks, with clear interfaces for communication.  
* **`cragcrowd-web-ui`**  
  * **Purpose**: To host the front-end application.  
  * **Content**: Web interface code in **TypeScript**.


