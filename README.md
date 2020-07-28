# Wi-ESP

Steps to acquire CSI are as follows:

Step-1: Setup the wireless network to acquire CSI.

Step-2: Make Client/Server architecture to generate traffic.

Step-3: Acquire CSI to configure ESP32 on the same channle on which Wireless Network is established.

Device-Free Wi-Fi Sensing (DFWS) is becoming a popular research due to the ubiquitous availability of Wi-Fi signals and comparatively accurate sensing with less infrastructure. The primary advantage of DFWS is exploiting the Wi-Fi signals around us to detect a person, fall, gesture location detection, and various other applications passively. In comparison with state-of-the-art techniques using sensors, cameras, or wearables to monitor an environment or person, DFWS uses Wi-Fi signal patterns to sense the state. The main advantage of being device-free is that it does not require any specifc devices like sensors or cameras. DFWS can be an ideal solution for many applications like the Internet of Things (IoT), Human Activity Recognition (HAR), monitoring elderly people, and many more since it is less obtrusive and offers pervasive sensing. The earliest work on "Sensorless Sensing" is a human sensing application using Wireless Sensor Networks (WSN) and a Received Signal Strength Indicator (RSSI) based detection. These preliminary works on localization and tracking the presence of humans accelerated the deployment of various applications based on DFWS. Unlike the use of RSSI, Channel State Information (CSI) in 802:11n gave more fine-grained dive into the Physical (PHY) layer and leverage useful information at every frame level.

FAQ:

Q: DO I need to use UDP_Client and UDP_Server of ESP32 to acquire CSI ?
Answer:No, this is not essential to used UDP_Client and UDP_Server of the ESP32, This can be built on any device or any routher. The main purpose is just to generte the traffic to acquire CSI this can be done by any device and router.


Q: Do you also have access to the AGC for amplitude compensation. 
Answer: No, ESP32 does not provide any control on the ASIC of the Wi-Fi module, so we can not change the AGC.

Q:do I need to recover  the phase after the collection or is it already done in the CSI Tool ?
ANswer:Yes, the collection of Phase is already included in the CSI Tool. The CSI information includes phase and amplitude of all the subcarriers. 
