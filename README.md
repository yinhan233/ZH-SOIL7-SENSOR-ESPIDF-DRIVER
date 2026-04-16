# 🌱 ESP32 RS485 土壤多参数传感器 (ZH-SOIL7) 驱动  
基于 ESP-IDF 框架开发的 RS485 Modbus RTU 传感器读取程序。针对 ZH-SOIL7系列（7合1）土壤传感器，可稳定读取土壤的温度、湿度（水分）、电导率(EC)、氮(N)、磷(P)、钾(K)，并将其封装为标准 JSON 格式输出。  
## ✨ 特性  
ESP-IDF原生驱动：使用uart实现通信，支持ESP32全系芯片。  
智能地址重置：开机自动发送广播指令，强制将传感器 Modbus 地址统一配置为 0x01，无惧出厂默认地址混乱。  
高可靠性：内置完整的 Modbus CRC16 校验算法，过滤错包乱码。  
适配自动流控：专门针对“自动收发流向控制”的 RS485 模块优化，解决了极速响应下 RX 缓存被清空导致的 Timeout 丢包问题。  
JSON 格式化：数据直接转为 JSON 字符串，完美对接 MQTT 或后端 Web 业务接口。  
## 🛠️ 硬件需求  
主控：ESP32开发板  
通信模块：TTL 转 RS485 模块（带自动流向控制，无需手动控制 DE/RE 引脚）  
传感器：ZH-SOIL7 土壤多参数传感器  
## ⚡ 接线说明  
ESP32 引脚	RS485 模块	传感器引脚	备注  
GPIO_NUM_17	TXD (或 DI)	-	UART2 发送端  
GPIO_NUM_16	RXD (或 RO)	-	UART2 接收端  
3.3V / 5V	VCC	-	视 RS485 模块工作电压而定  
GND	GND	黑色线 (GND)	共地  
-	A	黄色线 (A+)	485 差分正  
-	B	蓝色线 (B-)	485 差分负  
-	-	棕色线 (VCC)	建议接 12V 独立电源，保证探针供电稳定  
## 🚀运行日志示例  
编译烧录后，串口监视器将输出类似如下信息：  
```<TEXT>  
I (1134) DEBUG_APP: Sent Force-Address-01 Command to register 0x0080  
I (2134) DEBUG_APP: 正在调用工具函数读取数据...  
I (2134) DEBUG_APP: Executing soil data retrieval...  
I (2340) DEBUG_APP: Data successfully retrieved.
I (2340) DEBUG_APP: 工具调用成功！
[JSON 输出]: {"temperature_celsius": 22.9, "humidity_percent": 18.8, "ec_us_cm": 794, "nitrogen_mg_kg": 56, "phosphorus_mg_kg": 77, "potassium_mg_kg": 192}
I (2340) DEBUG_APP: 等待 5 秒后进行下一次读取...
```

上面这些都是ai生成的markdown  
在使用的过程中，最好看一下代码，自己辨识一下有没有要改的地方，因为写这个的人是个菜鸡QWQ  
