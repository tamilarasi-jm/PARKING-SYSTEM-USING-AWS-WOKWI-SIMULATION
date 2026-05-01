# PARKING-SYSTEM-USING-AWS-WOKWI-SIMULATION
PARKING SYSTEM USING AWS &amp; WOKWI SIMULATION
📌 Smart Parking IoT System
🚀 Overview

This project simulates a real-time parking system using IoT and AWS cloud services. An ESP32 device sends parking entry and exit events via MQTT, which are processed using AWS Lambda and stored in DynamoDB.

🧠 Architecture
ESP32 → AWS IoT Core → Lambda → DynamoDB
⚙️ Technologies Used
AWS IoT Core
AWS Lambda
Amazon DynamoDB
ESP32 (Wokwi Simulation)
MQTT Protocol
Arduino (C++)
🔐 Features
Secure communication using TLS certificates
Real-time event processing
Serverless backend (no servers used)
Event-driven architecture
📊 How it Works
User presses ENTRY/EXIT button
ESP32 publishes MQTT message
AWS IoT Core receives event
Lambda processes logic
DynamoDB updates parking status
