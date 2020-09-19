EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr User 9843 8268
encoding utf-8
Sheet 1 1
Title "Pot Watering Controller"
Date "2020-09-10"
Rev "1.2"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:GND #PWR0101
U 1 1 5F56E58B
P 4700 3000
F 0 "#PWR0101" H 4700 2750 50  0001 C CNN
F 1 "GND" H 4705 2827 50  0000 C CNN
F 2 "" H 4700 3000 50  0001 C CNN
F 3 "" H 4700 3000 50  0001 C CNN
	1    4700 3000
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0102
U 1 1 5F56EA20
P 4700 1700
F 0 "#PWR0102" H 4700 1550 50  0001 C CNN
F 1 "+3.3V" H 4715 1873 50  0000 C CNN
F 2 "" H 4700 1700 50  0001 C CNN
F 3 "" H 4700 1700 50  0001 C CNN
	1    4700 1700
	-1   0    0    1   
$EndComp
$Comp
L power:+3.3V #PWR0103
U 1 1 5F56F2A8
P 4700 850
F 0 "#PWR0103" H 4700 700 50  0001 C CNN
F 1 "+3.3V" H 4715 1023 50  0000 C CNN
F 2 "" H 4700 850 50  0001 C CNN
F 3 "" H 4700 850 50  0001 C CNN
	1    4700 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 1000 4700 900 
Wire Wire Line
	4700 900  4700 850 
Connection ~ 4700 900 
$Comp
L power:+3.3V #PWR0104
U 1 1 5F57046D
P 4700 2150
F 0 "#PWR0104" H 4700 2000 50  0001 C CNN
F 1 "+3.3V" H 4715 2323 50  0000 C CNN
F 2 "" H 4700 2150 50  0001 C CNN
F 3 "" H 4700 2150 50  0001 C CNN
	1    4700 2150
	1    0    0    -1  
$EndComp
Text GLabel 4700 2900 0    50   Input ~ 0
SDA
Text GLabel 4700 1600 0    50   Input ~ 0
SDA
Text GLabel 4700 1500 0    50   Input ~ 0
SCL
Text GLabel 4700 2800 0    50   Input ~ 0
SCL
$Comp
L power:GND #PWR0105
U 1 1 5F5714D8
P 6300 3000
F 0 "#PWR0105" H 6300 2750 50  0001 C CNN
F 1 "GND" H 6305 2827 50  0000 C CNN
F 2 "" H 6300 3000 50  0001 C CNN
F 3 "" H 6300 3000 50  0001 C CNN
	1    6300 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 5F57195A
P 6300 1700
F 0 "#PWR0106" H 6300 1450 50  0001 C CNN
F 1 "GND" H 6305 1527 50  0000 C CNN
F 2 "" H 6300 1700 50  0001 C CNN
F 3 "" H 6300 1700 50  0001 C CNN
	1    6300 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 1700 6300 1600
Connection ~ 6300 1700
$Comp
L eec:LPS33HWTR U3
U 1 1 5F57E170
P 4600 2200
F 0 "U3" H 5500 2465 50  0000 C CNN
F 1 "LPS33HWTR" H 5500 2374 50  0000 C CNN
F 2 "octopart:STMicroelectronics-CCLGA-10L-DM00294349-0-2-MFG" H 4600 2600 50  0001 L CNN
F 3 "https://www.st.com/resource/en/datasheet/lps33hw.pdf" H 4600 2700 50  0001 L CNN
F 4 "No" H 4600 2800 50  0001 L CNN "automotive"
F 5 "IC" H 4600 2900 50  0001 L CNN "category"
F 6 "Sensors" H 4600 3000 50  0001 L CNN "device class L1"
F 7 "Pressure Sensors" H 4600 3100 50  0001 L CNN "device class L2"
F 8 "unset" H 4600 3200 50  0001 L CNN "device class L3"
F 9 "CONSUMER MEMS" H 4600 3300 50  0001 L CNN "digikey description"
F 10 "497-17590-1-ND" H 4600 3400 50  0001 L CNN "digikey part number"
F 11 "https://www.st.com/content/ccc/resource/technical/document/technical_note/73/17/9c/16/dd/96/4a/01/CD00134799.pdf/files/CD00134799.pdf/jcr:content/translations/en.CD00134799.pdf" H 4600 3500 50  0001 L CNN "footprint url"
F 12 "2.9mm" H 4600 3600 50  0001 L CNN "height"
F 13 "Yes" H 4600 3700 50  0001 L CNN "lead free"
F 14 "775cc606e9901eaa" H 4600 3800 50  0001 L CNN "library id"
F 15 "STMicroelectronics" H 4600 3900 50  0001 L CNN "manufacturer"
F 16 "18.275PSI" H 4600 4000 50  0001 L CNN "max pressure"
F 17 "3.6V" H 4600 4100 50  0001 L CNN "max supply voltage"
F 18 "1.7V" H 4600 4200 50  0001 L CNN "min supply voltage"
F 19 "511-LPS33HWTR" H 4600 4300 50  0001 L CNN "mouser part number"
F 20 "3-15uA" H 4600 4400 50  0001 L CNN "operating supply current"
F 21 "CCLGA10" H 4600 4500 50  0001 L CNN "package"
F 22 "No Port" H 4600 4600 50  0001 L CNN "port style"
F 23 "3.771-18.275PSI" H 4600 4700 50  0001 L CNN "pressure"
F 24 "Yes" H 4600 4800 50  0001 L CNN "rohs"
F 25 "I2C,SPI" H 4600 4900 50  0001 L CNN "sensor output"
F 26 "+85°C" H 4600 5000 50  0001 L CNN "temperature range high"
F 27 "-40°C" H 4600 5100 50  0001 L CNN "temperature range low"
	1    4600 2200
	1    0    0    -1  
$EndComp
$Comp
L eec:LPS33HWTR U2
U 1 1 5F57FF2A
P 4600 900
F 0 "U2" H 5500 1165 50  0000 C CNN
F 1 "LPS33HWTR" H 5500 1074 50  0000 C CNN
F 2 "octopart:STMicroelectronics-CCLGA-10L-DM00294349-0-2-MFG" H 4600 1300 50  0001 L CNN
F 3 "https://www.st.com/resource/en/datasheet/lps33hw.pdf" H 4600 1400 50  0001 L CNN
F 4 "No" H 4600 1500 50  0001 L CNN "automotive"
F 5 "IC" H 4600 1600 50  0001 L CNN "category"
F 6 "Sensors" H 4600 1700 50  0001 L CNN "device class L1"
F 7 "Pressure Sensors" H 4600 1800 50  0001 L CNN "device class L2"
F 8 "unset" H 4600 1900 50  0001 L CNN "device class L3"
F 9 "CONSUMER MEMS" H 4600 2000 50  0001 L CNN "digikey description"
F 10 "497-17590-1-ND" H 4600 2100 50  0001 L CNN "digikey part number"
F 11 "https://www.st.com/content/ccc/resource/technical/document/technical_note/73/17/9c/16/dd/96/4a/01/CD00134799.pdf/files/CD00134799.pdf/jcr:content/translations/en.CD00134799.pdf" H 4600 2200 50  0001 L CNN "footprint url"
F 12 "2.9mm" H 4600 2300 50  0001 L CNN "height"
F 13 "Yes" H 4600 2400 50  0001 L CNN "lead free"
F 14 "775cc606e9901eaa" H 4600 2500 50  0001 L CNN "library id"
F 15 "STMicroelectronics" H 4600 2600 50  0001 L CNN "manufacturer"
F 16 "18.275PSI" H 4600 2700 50  0001 L CNN "max pressure"
F 17 "3.6V" H 4600 2800 50  0001 L CNN "max supply voltage"
F 18 "1.7V" H 4600 2900 50  0001 L CNN "min supply voltage"
F 19 "511-LPS33HWTR" H 4600 3000 50  0001 L CNN "mouser part number"
F 20 "3-15uA" H 4600 3100 50  0001 L CNN "operating supply current"
F 21 "CCLGA10" H 4600 3200 50  0001 L CNN "package"
F 22 "No Port" H 4600 3300 50  0001 L CNN "port style"
F 23 "3.771-18.275PSI" H 4600 3400 50  0001 L CNN "pressure"
F 24 "Yes" H 4600 3500 50  0001 L CNN "rohs"
F 25 "I2C,SPI" H 4600 3600 50  0001 L CNN "sensor output"
F 26 "+85°C" H 4600 3700 50  0001 L CNN "temperature range high"
F 27 "-40°C" H 4600 3800 50  0001 L CNN "temperature range low"
	1    4600 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 1600 6300 1500
Connection ~ 6300 1600
$Comp
L eec:MCZ33996EK U4
U 1 1 5F585581
P 4750 3700
F 0 "U4" H 5400 3965 50  0000 C CNN
F 1 "MCZ33996EK" H 5400 3874 50  0000 C CNN
F 2 "octopart:NXP-MCZ33996EK-IPC_A" H 4750 4100 50  0001 L CNN
F 3 "https://www.nxp.com/docs/en/data-sheet/MC33996.pdf" H 4750 4200 50  0001 L CNN
F 4 "+125°C" H 4750 4300 50  0001 L CNN "ambient temperature range high"
F 5 "-40°C" H 4750 4400 50  0001 L CNN "ambient temperature range low"
F 6 "No" H 4750 4500 50  0001 L CNN "automotive"
F 7 "IC" H 4750 4600 50  0001 L CNN "category"
F 8 "Integrated Circuits (ICs)" H 4750 4700 50  0001 L CNN "device class L1"
F 9 "Power Management ICs" H 4750 4800 50  0001 L CNN "device class L2"
F 10 "Hot Swap Controllers" H 4750 4900 50  0001 L CNN "device class L3"
F 11 "IC PWR SWITCH N-CH 1:16 32SOIC" H 4750 5000 50  0001 L CNN "digikey description"
F 12 "MCZ33996EK-ND" H 4750 5100 50  0001 L CNN "digikey part number"
F 13 "https://www.nxp.com/docs/en/package-information/98ARL10543D.pdf" H 4750 5200 50  0001 L CNN "footprint url"
F 14 "2.45mm" H 4750 5300 50  0001 L CNN "height"
F 15 "SOIC65P1030X250-32" H 4750 5400 50  0001 L CNN "ipc land pattern name"
F 16 "Yes" H 4750 5500 50  0001 L CNN "lead free"
F 17 "2435d55b9b6a606d" H 4750 5600 50  0001 L CNN "library id"
F 18 "NXP" H 4750 5700 50  0001 L CNN "manufacturer"
F 19 "+150°C" H 4750 5800 50  0001 L CNN "max junction temp"
F 20 "1.7W" H 4750 5900 50  0001 L CNN "max power dissipation"
F 21 "27V" H 4750 6000 50  0001 L CNN "max supply voltage"
F 22 "5V" H 4750 6100 50  0001 L CNN "min supply voltage"
F 23 "Power Switch ICs - Power Distribution 16 LOW SIDE SW" H 4750 6200 50  0001 L CNN "mouser description"
F 24 "841-MCZ33996EK" H 4750 6300 50  0001 L CNN "mouser part number"
F 25 "4mA" H 4750 6400 50  0001 L CNN "nominal supply current"
F 26 "16" H 4750 6500 50  0001 L CNN "number of channels"
F 27 "16" H 4750 6600 50  0001 L CNN "number of outputs"
F 28 "0.55Ω" H 4750 6700 50  0001 L CNN "on state resistance"
F 29 "SOICW32" H 4750 6800 50  0001 L CNN "package"
F 30 "Yes" H 4750 6900 50  0001 L CNN "rohs"
F 31 "0mm" H 4750 7000 50  0001 L CNN "standoff height"
F 32 "+150°C" H 4750 7100 50  0001 L CNN "temperature range high"
F 33 "-40°C" H 4750 7200 50  0001 L CNN "temperature range low"
F 34 "4us" H 4750 7300 50  0001 L CNN "turn off delay time"
F 35 "2us" H 4750 7400 50  0001 L CNN "turn on delay time"
	1    4750 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 2150 4700 2200
Wire Wire Line
	6300 2800 6300 2900
$Comp
L power:GND #PWR0107
U 1 1 5F589C65
P 7700 3800
F 0 "#PWR0107" H 7700 3550 50  0001 C CNN
F 1 "GND" H 7705 3627 50  0000 C CNN
F 2 "" H 7700 3800 50  0001 C CNN
F 3 "" H 7700 3800 50  0001 C CNN
	1    7700 3800
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0109
U 1 1 5F58C041
P 8250 5900
F 0 "#PWR0109" H 8250 5750 50  0001 C CNN
F 1 "+3.3V" H 8265 6073 50  0000 C CNN
F 2 "" H 8250 5900 50  0001 C CNN
F 3 "" H 8250 5900 50  0001 C CNN
	1    8250 5900
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male program_port1
U 1 1 5F58DB9D
P 8450 6100
F 0 "program_port1" H 8422 5982 50  0000 R CNN
F 1 "Conn_01x04_Male" H 8422 6073 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 8450 6100 50  0001 C CNN
F 3 "~" H 8450 6100 50  0001 C CNN
	1    8450 6100
	-1   0    0    1   
$EndComp
Text GLabel 8250 6100 0    50   Input ~ 0
TX
Text GLabel 8250 6000 0    50   Input ~ 0
RX
$Comp
L power:GND #PWR0110
U 1 1 5F598331
P 8250 6200
F 0 "#PWR0110" H 8250 5950 50  0001 C CNN
F 1 "GND" H 8255 6027 50  0000 C CNN
F 2 "" H 8250 6200 50  0001 C CNN
F 3 "" H 8250 6200 50  0001 C CNN
	1    8250 6200
	1    0    0    -1  
$EndComp
Connection ~ 5950 6100
$Comp
L power:GND #PWR0113
U 1 1 5F58A144
P 5950 6100
F 0 "#PWR0113" H 5950 5850 50  0001 C CNN
F 1 "GND" H 5955 5927 50  0000 C CNN
F 2 "" H 5950 6100 50  0001 C CNN
F 3 "" H 5950 6100 50  0001 C CNN
	1    5950 6100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5950 6000 5950 6100
Connection ~ 5950 6000
Wire Wire Line
	5950 5900 5950 6000
Connection ~ 5950 5900
Wire Wire Line
	5950 5800 5950 5900
Connection ~ 5950 5800
Wire Wire Line
	5950 5700 5950 5800
Connection ~ 5950 5700
Wire Wire Line
	5950 5600 5950 5700
Connection ~ 5950 5600
Wire Wire Line
	5950 5500 5950 5600
Connection ~ 5950 5500
Wire Wire Line
	5950 5400 5950 5500
Text GLabel 4850 4500 0    50   Input ~ 0
MISO
Text GLabel 4850 4700 0    50   Input ~ 0
MOSI
Text GLabel 4850 4300 0    50   Input ~ 0
NS_pumpes
Text GLabel 4850 4100 0    50   Input ~ 0
SCLK
Text GLabel 1650 1650 2    50   Input ~ 0
MISO
Text GLabel 1650 1750 2    50   Input ~ 0
MOSI
Text GLabel 1650 1550 2    50   Input ~ 0
SCLK
$Comp
L power:GND #PWR0114
U 1 1 5F5C4CF0
P 1650 2050
F 0 "#PWR0114" H 1650 1800 50  0001 C CNN
F 1 "GND" H 1655 1877 50  0000 C CNN
F 2 "" H 1650 2050 50  0001 C CNN
F 3 "" H 1650 2050 50  0001 C CNN
	1    1650 2050
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0115
U 1 1 5F5C5703
P 1650 1350
F 0 "#PWR0115" H 1650 1200 50  0001 C CNN
F 1 "+3.3V" H 1665 1523 50  0000 C CNN
F 2 "" H 1650 1350 50  0001 C CNN
F 3 "" H 1650 1350 50  0001 C CNN
	1    1650 1350
	1    0    0    -1  
$EndComp
Text GLabel 1650 1450 2    50   Input ~ 0
NS_sensor_00
Text GLabel 2150 1950 0    50   Input ~ 0
pumpe_00
Text GLabel 5950 3800 2    50   Input ~ 0
pumpe_01
Text GLabel 5950 3700 2    50   Input ~ 0
pumpe_00
Text GLabel 5950 3900 2    50   Input ~ 0
pumpe_02
Text GLabel 5950 4000 2    50   Input ~ 0
pumpe_03
Text GLabel 5950 4100 2    50   Input ~ 0
pumpe_04
Text GLabel 5950 4200 2    50   Input ~ 0
pumpe_05
Text GLabel 5950 4300 2    50   Input ~ 0
pumpe_06
Text GLabel 5950 4400 2    50   Input ~ 0
pumpe_07
Text GLabel 5950 4500 2    50   Input ~ 0
pumpe_08
Text GLabel 5950 4600 2    50   Input ~ 0
pumpe_09
Text GLabel 1650 2900 2    50   Input ~ 0
MISO
Text GLabel 1650 3000 2    50   Input ~ 0
MOSI
Text GLabel 1650 2800 2    50   Input ~ 0
SCLK
$Comp
L power:GND #PWR0116
U 1 1 5F5D1124
P 1650 3300
F 0 "#PWR0116" H 1650 3050 50  0001 C CNN
F 1 "GND" H 1655 3127 50  0000 C CNN
F 2 "" H 1650 3300 50  0001 C CNN
F 3 "" H 1650 3300 50  0001 C CNN
	1    1650 3300
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0117
U 1 1 5F5D112E
P 1650 2600
F 0 "#PWR0117" H 1650 2450 50  0001 C CNN
F 1 "+3.3V" H 1665 2773 50  0000 C CNN
F 2 "" H 1650 2600 50  0001 C CNN
F 3 "" H 1650 2600 50  0001 C CNN
	1    1650 2600
	1    0    0    -1  
$EndComp
Text GLabel 1650 2700 2    50   Input ~ 0
NS_sensor_02
Text GLabel 2150 3200 0    50   Input ~ 0
pumpe_02
$Comp
L Connector:8P8C_Shielded sensor_4
U 1 1 5F5D5682
P 1250 4150
F 0 "sensor_4" H 1250 4200 50  0000 C CNN
F 1 "RJ45" H 1307 4726 50  0001 C CNN
F 2 "Connector_RJ:RJ45_fciconnector_vertical" V 1250 4175 50  0001 C CNN
F 3 "~" V 1250 4175 50  0001 C CNN
	1    1250 4150
	1    0    0    1   
$EndComp
Text GLabel 1650 4150 2    50   Input ~ 0
MISO
Text GLabel 1650 4250 2    50   Input ~ 0
MOSI
Text GLabel 1650 4050 2    50   Input ~ 0
SCLK
$Comp
L power:GND #PWR0118
U 1 1 5F5D568B
P 1650 4550
F 0 "#PWR0118" H 1650 4300 50  0001 C CNN
F 1 "GND" H 1655 4377 50  0000 C CNN
F 2 "" H 1650 4550 50  0001 C CNN
F 3 "" H 1650 4550 50  0001 C CNN
	1    1650 4550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0119
U 1 1 5F5D5691
P 1650 3850
F 0 "#PWR0119" H 1650 3700 50  0001 C CNN
F 1 "+3.3V" H 1665 4023 50  0000 C CNN
F 2 "" H 1650 3850 50  0001 C CNN
F 3 "" H 1650 3850 50  0001 C CNN
	1    1650 3850
	1    0    0    -1  
$EndComp
Text GLabel 1650 3950 2    50   Input ~ 0
NS_sensor_04
Text GLabel 2150 4450 0    50   Input ~ 0
pumpe_04
Text GLabel 1650 5400 2    50   Input ~ 0
MISO
Text GLabel 1650 5500 2    50   Input ~ 0
MOSI
Text GLabel 1650 5300 2    50   Input ~ 0
SCLK
$Comp
L power:GND #PWR0120
U 1 1 5F5D75E8
P 1650 5800
F 0 "#PWR0120" H 1650 5550 50  0001 C CNN
F 1 "GND" H 1655 5627 50  0000 C CNN
F 2 "" H 1650 5800 50  0001 C CNN
F 3 "" H 1650 5800 50  0001 C CNN
	1    1650 5800
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0121
U 1 1 5F5D75EE
P 1650 5100
F 0 "#PWR0121" H 1650 4950 50  0001 C CNN
F 1 "+3.3V" H 1665 5273 50  0000 C CNN
F 2 "" H 1650 5100 50  0001 C CNN
F 3 "" H 1650 5100 50  0001 C CNN
	1    1650 5100
	1    0    0    -1  
$EndComp
Text GLabel 1650 5200 2    50   Input ~ 0
NS_sensor_06
Text GLabel 2150 5700 0    50   Input ~ 0
pumpe_06
Text GLabel 1650 6650 2    50   Input ~ 0
MISO
Text GLabel 1650 6750 2    50   Input ~ 0
MOSI
Text GLabel 1650 6550 2    50   Input ~ 0
SCLK
$Comp
L power:GND #PWR0122
U 1 1 5F5D93D4
P 1650 7050
F 0 "#PWR0122" H 1650 6800 50  0001 C CNN
F 1 "GND" H 1655 6877 50  0000 C CNN
F 2 "" H 1650 7050 50  0001 C CNN
F 3 "" H 1650 7050 50  0001 C CNN
	1    1650 7050
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0123
U 1 1 5F5D93DA
P 1650 6350
F 0 "#PWR0123" H 1650 6200 50  0001 C CNN
F 1 "+3.3V" H 1665 6523 50  0000 C CNN
F 2 "" H 1650 6350 50  0001 C CNN
F 3 "" H 1650 6350 50  0001 C CNN
	1    1650 6350
	1    0    0    -1  
$EndComp
Text GLabel 1650 6450 2    50   Input ~ 0
NS_sensor_08
Text GLabel 2150 6950 0    50   Input ~ 0
pumpe_08
Text GLabel 3550 1650 2    50   Input ~ 0
MISO
Text GLabel 3550 1750 2    50   Input ~ 0
MOSI
Text GLabel 3550 1550 2    50   Input ~ 0
SCLK
$Comp
L power:GND #PWR0124
U 1 1 5F5DB7EA
P 3550 2050
F 0 "#PWR0124" H 3550 1800 50  0001 C CNN
F 1 "GND" H 3555 1877 50  0000 C CNN
F 2 "" H 3550 2050 50  0001 C CNN
F 3 "" H 3550 2050 50  0001 C CNN
	1    3550 2050
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0125
U 1 1 5F5DB7F0
P 3550 1350
F 0 "#PWR0125" H 3550 1200 50  0001 C CNN
F 1 "+3.3V" H 3565 1523 50  0000 C CNN
F 2 "" H 3550 1350 50  0001 C CNN
F 3 "" H 3550 1350 50  0001 C CNN
	1    3550 1350
	1    0    0    -1  
$EndComp
Text GLabel 3550 1450 2    50   Input ~ 0
NS_sensor_01
Text GLabel 4050 1950 0    50   Input ~ 0
pumpe_01
Text GLabel 3550 2900 2    50   Input ~ 0
MISO
Text GLabel 3550 3000 2    50   Input ~ 0
MOSI
Text GLabel 3550 2800 2    50   Input ~ 0
SCLK
$Comp
L power:GND #PWR0126
U 1 1 5F5DD1FE
P 3550 3300
F 0 "#PWR0126" H 3550 3050 50  0001 C CNN
F 1 "GND" H 3555 3127 50  0000 C CNN
F 2 "" H 3550 3300 50  0001 C CNN
F 3 "" H 3550 3300 50  0001 C CNN
	1    3550 3300
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0127
U 1 1 5F5DD204
P 3550 2600
F 0 "#PWR0127" H 3550 2450 50  0001 C CNN
F 1 "+3.3V" H 3565 2773 50  0000 C CNN
F 2 "" H 3550 2600 50  0001 C CNN
F 3 "" H 3550 2600 50  0001 C CNN
	1    3550 2600
	1    0    0    -1  
$EndComp
Text GLabel 3550 2700 2    50   Input ~ 0
NS_sensor_03
Text GLabel 4050 3200 0    50   Input ~ 0
pumpe_03
Text GLabel 3550 4150 2    50   Input ~ 0
MISO
Text GLabel 3550 4250 2    50   Input ~ 0
MOSI
Text GLabel 3550 4050 2    50   Input ~ 0
SCLK
$Comp
L power:GND #PWR0128
U 1 1 5F5DDC31
P 3550 4550
F 0 "#PWR0128" H 3550 4300 50  0001 C CNN
F 1 "GND" H 3555 4377 50  0000 C CNN
F 2 "" H 3550 4550 50  0001 C CNN
F 3 "" H 3550 4550 50  0001 C CNN
	1    3550 4550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0129
U 1 1 5F5DDC37
P 3550 3850
F 0 "#PWR0129" H 3550 3700 50  0001 C CNN
F 1 "+3.3V" H 3565 4023 50  0000 C CNN
F 2 "" H 3550 3850 50  0001 C CNN
F 3 "" H 3550 3850 50  0001 C CNN
	1    3550 3850
	1    0    0    -1  
$EndComp
Text GLabel 3550 3950 2    50   Input ~ 0
NS_sensor_05
Text GLabel 4050 4450 0    50   Input ~ 0
pumpe_05
Text GLabel 3550 5400 2    50   Input ~ 0
MISO
Text GLabel 3550 5500 2    50   Input ~ 0
MOSI
Text GLabel 3550 5300 2    50   Input ~ 0
SCLK
$Comp
L power:GND #PWR0130
U 1 1 5F5E17CC
P 3550 5800
F 0 "#PWR0130" H 3550 5550 50  0001 C CNN
F 1 "GND" H 3555 5627 50  0000 C CNN
F 2 "" H 3550 5800 50  0001 C CNN
F 3 "" H 3550 5800 50  0001 C CNN
	1    3550 5800
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0131
U 1 1 5F5E17D2
P 3550 5100
F 0 "#PWR0131" H 3550 4950 50  0001 C CNN
F 1 "+3.3V" H 3565 5273 50  0000 C CNN
F 2 "" H 3550 5100 50  0001 C CNN
F 3 "" H 3550 5100 50  0001 C CNN
	1    3550 5100
	1    0    0    -1  
$EndComp
Text GLabel 3550 5200 2    50   Input ~ 0
NS_sensor_07
Text GLabel 4050 5700 0    50   Input ~ 0
pumpe_07
Text GLabel 3550 6650 2    50   Input ~ 0
MISO
Text GLabel 3550 6750 2    50   Input ~ 0
MOSI
Text GLabel 3550 6550 2    50   Input ~ 0
SCLK
$Comp
L power:GND #PWR0132
U 1 1 5F5E303C
P 3550 7050
F 0 "#PWR0132" H 3550 6800 50  0001 C CNN
F 1 "GND" H 3555 6877 50  0000 C CNN
F 2 "" H 3550 7050 50  0001 C CNN
F 3 "" H 3550 7050 50  0001 C CNN
	1    3550 7050
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0133
U 1 1 5F5E3042
P 3550 6350
F 0 "#PWR0133" H 3550 6200 50  0001 C CNN
F 1 "+3.3V" H 3565 6523 50  0000 C CNN
F 2 "" H 3550 6350 50  0001 C CNN
F 3 "" H 3550 6350 50  0001 C CNN
	1    3550 6350
	1    0    0    -1  
$EndComp
Text GLabel 3550 6450 2    50   Input ~ 0
NS_sensor_09
Text GLabel 4050 6950 0    50   Input ~ 0
pumpe_09
$Comp
L Connector:Conn_01x02_Female pumpe_4
U 1 1 5F5EC3E0
P 2350 4450
F 0 "pumpe_4" H 2000 4550 50  0000 L CNN
F 1 "pumpe" H 2378 4335 50  0001 L CNN
F 2 "Connector_Phoenix_MC_HighVoltage:PhoenixContact_MCV_1,5_2-G-5.08_1x02_P5.08mm_Vertical" H 2350 4450 50  0001 C CNN
F 3 "~" H 2350 4450 50  0001 C CNN
	1    2350 4450
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female pumpe_8
U 1 1 5F5F544C
P 2350 6950
F 0 "pumpe_8" H 2000 7050 50  0000 L CNN
F 1 "pumpe" H 2378 6835 50  0001 L CNN
F 2 "Connector_Phoenix_MC_HighVoltage:PhoenixContact_MCV_1,5_2-G-5.08_1x02_P5.08mm_Vertical" H 2350 6950 50  0001 C CNN
F 3 "~" H 2350 6950 50  0001 C CNN
	1    2350 6950
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female pumpe_9
U 1 1 5F5F5A97
P 4250 6950
F 0 "pumpe_9" H 3900 7050 50  0000 L CNN
F 1 "pumpe" H 4278 6835 50  0001 L CNN
F 2 "Connector_Phoenix_MC_HighVoltage:PhoenixContact_MCV_1,5_2-G-5.08_1x02_P5.08mm_Vertical" H 4250 6950 50  0001 C CNN
F 3 "~" H 4250 6950 50  0001 C CNN
	1    4250 6950
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female pumpe_7
U 1 1 5F5F6493
P 4250 5700
F 0 "pumpe_7" H 3900 5800 50  0000 L CNN
F 1 "pumpe" H 4278 5585 50  0001 L CNN
F 2 "Connector_Phoenix_MC_HighVoltage:PhoenixContact_MCV_1,5_2-G-5.08_1x02_P5.08mm_Vertical" H 4250 5700 50  0001 C CNN
F 3 "~" H 4250 5700 50  0001 C CNN
	1    4250 5700
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female pumpe_6
U 1 1 5F5F6B65
P 2350 5700
F 0 "pumpe_6" H 2000 5800 50  0000 L CNN
F 1 "pumpe" H 2378 5585 50  0001 L CNN
F 2 "Connector_Phoenix_MC_HighVoltage:PhoenixContact_MCV_1,5_2-G-5.08_1x02_P5.08mm_Vertical" H 2350 5700 50  0001 C CNN
F 3 "~" H 2350 5700 50  0001 C CNN
	1    2350 5700
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female pumpe_5
U 1 1 5F5F72D9
P 4250 4450
F 0 "pumpe_5" H 3900 4550 50  0000 L CNN
F 1 "pumpe" H 4278 4335 50  0001 L CNN
F 2 "Connector_Phoenix_MC_HighVoltage:PhoenixContact_MCV_1,5_2-G-5.08_1x02_P5.08mm_Vertical" H 4250 4450 50  0001 C CNN
F 3 "~" H 4250 4450 50  0001 C CNN
	1    4250 4450
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female pumpe_2
U 1 1 5F5F7BC7
P 2350 3200
F 0 "pumpe_2" H 2000 3300 50  0000 L CNN
F 1 "pumpe" H 2378 3085 50  0001 L CNN
F 2 "Connector_Phoenix_MC_HighVoltage:PhoenixContact_MCV_1,5_2-G-5.08_1x02_P5.08mm_Vertical" H 2350 3200 50  0001 C CNN
F 3 "~" H 2350 3200 50  0001 C CNN
	1    2350 3200
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female pumpe_3
U 1 1 5F5F82B4
P 4250 3200
F 0 "pumpe_3" H 3900 3300 50  0000 L CNN
F 1 "pumpe" H 4278 3085 50  0001 L CNN
F 2 "Connector_Phoenix_MC_HighVoltage:PhoenixContact_MCV_1,5_2-G-5.08_1x02_P5.08mm_Vertical" H 4250 3200 50  0001 C CNN
F 3 "~" H 4250 3200 50  0001 C CNN
	1    4250 3200
	1    0    0    -1  
$EndComp
$Comp
L Connector:8P8C_Shielded sensor_5
U 1 1 5F5FC6C8
P 3150 4150
F 0 "sensor_5" H 3150 4200 50  0000 C CNN
F 1 "RJ45" H 3207 4726 50  0001 C CNN
F 2 "Connector_RJ:RJ45_fciconnector_vertical" V 3150 4175 50  0001 C CNN
F 3 "~" V 3150 4175 50  0001 C CNN
	1    3150 4150
	1    0    0    1   
$EndComp
$Comp
L Connector:8P8C_Shielded sensor_7
U 1 1 5F5FD69F
P 3150 5400
F 0 "sensor_7" H 3150 5450 50  0000 C CNN
F 1 "RJ45" H 3207 5976 50  0001 C CNN
F 2 "Connector_RJ:RJ45_fciconnector_vertical" V 3150 5425 50  0001 C CNN
F 3 "~" V 3150 5425 50  0001 C CNN
	1    3150 5400
	1    0    0    1   
$EndComp
$Comp
L Connector:8P8C_Shielded sensor_6
U 1 1 5F5FEF5B
P 1250 5400
F 0 "sensor_6" H 1250 5450 50  0000 C CNN
F 1 "RJ45" H 1307 5976 50  0001 C CNN
F 2 "Connector_RJ:RJ45_fciconnector_vertical" V 1250 5425 50  0001 C CNN
F 3 "~" V 1250 5425 50  0001 C CNN
	1    1250 5400
	1    0    0    1   
$EndComp
$Comp
L Connector:8P8C_Shielded sensor_8
U 1 1 5F6001E4
P 1250 6650
F 0 "sensor_8" H 1250 6700 50  0000 C CNN
F 1 "RJ45" H 1307 7226 50  0001 C CNN
F 2 "Connector_RJ:RJ45_fciconnector_vertical" V 1250 6675 50  0001 C CNN
F 3 "~" V 1250 6675 50  0001 C CNN
	1    1250 6650
	1    0    0    1   
$EndComp
$Comp
L Connector:8P8C_Shielded sensor_9
U 1 1 5F6011BB
P 3150 6650
F 0 "sensor_9" H 3150 6700 50  0000 C CNN
F 1 "RJ45" H 3207 7226 50  0001 C CNN
F 2 "Connector_RJ:RJ45_fciconnector_vertical" V 3150 6675 50  0001 C CNN
F 3 "~" V 3150 6675 50  0001 C CNN
	1    3150 6650
	1    0    0    1   
$EndComp
$Comp
L Connector:8P8C_Shielded sensor_2
U 1 1 5F6023BA
P 1250 2900
F 0 "sensor_2" H 1250 2950 50  0000 C CNN
F 1 "RJ45" H 1307 3476 50  0001 C CNN
F 2 "Connector_RJ:RJ45_fciconnector_vertical" V 1250 2925 50  0001 C CNN
F 3 "~" V 1250 2925 50  0001 C CNN
	1    1250 2900
	1    0    0    1   
$EndComp
$Comp
L Connector:8P8C_Shielded sensor_3
U 1 1 5F603BEC
P 3150 2900
F 0 "sensor_3" H 3150 2950 50  0000 C CNN
F 1 "RJ45" H 3207 3476 50  0001 C CNN
F 2 "Connector_RJ:RJ45_fciconnector_vertical" V 3150 2925 50  0001 C CNN
F 3 "~" V 3150 2925 50  0001 C CNN
	1    3150 2900
	1    0    0    1   
$EndComp
$Comp
L Connector:8P8C_Shielded sensor_1
U 1 1 5F6050F5
P 3150 1650
F 0 "sensor_1" H 3150 1700 50  0000 C CNN
F 1 "RJ45" H 3207 2226 50  0001 C CNN
F 2 "Connector_RJ:RJ45_fciconnector_vertical" V 3150 1675 50  0001 C CNN
F 3 "~" V 3150 1675 50  0001 C CNN
	1    3150 1650
	1    0    0    1   
$EndComp
$Comp
L Connector:8P8C_Shielded sensor_0
U 1 1 5F6066FF
P 1250 1650
F 0 "sensor_0" H 1250 1700 50  0000 C CNN
F 1 "RJ45" H 1307 2226 50  0001 C CNN
F 2 "Connector_RJ:RJ45_fciconnector_vertical" V 1250 1675 50  0001 C CNN
F 3 "~" V 1250 1675 50  0001 C CNN
	1    1250 1650
	1    0    0    1   
$EndComp
$Comp
L Connector:Conn_01x02_Female pumpe_0
U 1 1 5F60828A
P 2350 1950
F 0 "pumpe_0" H 2000 2050 50  0000 L CNN
F 1 "pumpe" H 2378 1835 50  0001 L CNN
F 2 "Connector_Phoenix_MC_HighVoltage:PhoenixContact_MCV_1,5_2-G-5.08_1x02_P5.08mm_Vertical" H 2350 1950 50  0001 C CNN
F 3 "~" H 2350 1950 50  0001 C CNN
	1    2350 1950
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female pumpe_1
U 1 1 5F608CD7
P 4250 1950
F 0 "pumpe_1" H 3900 2050 50  0000 L CNN
F 1 "pumpe" H 4278 1835 50  0001 L CNN
F 2 "Connector_Phoenix_MC_HighVoltage:PhoenixContact_MCV_1,5_2-G-5.08_1x02_P5.08mm_Vertical" H 4250 1950 50  0001 C CNN
F 3 "~" H 4250 1950 50  0001 C CNN
	1    4250 1950
	1    0    0    -1  
$EndComp
Text GLabel 8300 1300 2    50   Input ~ 0
TX
Text GLabel 8300 1500 2    50   Input ~ 0
RX
$Comp
L Sensor_Motion:LIS3DH U5
U 1 1 5F60ECC1
P 7400 4900
F 0 "U5" H 7400 4211 50  0000 C CNN
F 1 "LIS3DH" H 7400 4120 50  0000 C CNN
F 2 "Package_LGA:LGA-16_3x3mm_P0.5mm_LayoutBorder3x5y" H 7500 3850 50  0001 C CNN
F 3 "https://www.st.com/resource/en/datasheet/cd00274221.pdf" H 7200 4800 50  0001 C CNN
	1    7400 4900
	1    0    0    -1  
$EndComp
Text GLabel 6900 5100 0    50   Input ~ 0
SCL
Text GLabel 6900 5000 0    50   Input ~ 0
SDA
$Comp
L power:GND #PWR0134
U 1 1 5F61B988
P 7400 5500
F 0 "#PWR0134" H 7400 5250 50  0001 C CNN
F 1 "GND" H 7405 5327 50  0000 C CNN
F 2 "" H 7400 5500 50  0001 C CNN
F 3 "" H 7400 5500 50  0001 C CNN
	1    7400 5500
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0135
U 1 1 5F61D45F
P 7400 4400
F 0 "#PWR0135" H 7400 4250 50  0001 C CNN
F 1 "+3.3V" H 7415 4573 50  0000 C CNN
F 2 "" H 7400 4400 50  0001 C CNN
F 3 "" H 7400 4400 50  0001 C CNN
	1    7400 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 4400 7400 4400
Connection ~ 7400 4400
Wire Wire Line
	7400 4400 7500 4400
Text GLabel 8300 2400 2    50   Input ~ 0
SCLK
Text GLabel 8300 2800 2    50   Input ~ 0
MOSI
Text GLabel 8300 2500 2    50   Input ~ 0
MISO
Text GLabel 8300 1700 2    50   Input ~ 0
NS_pumpes
Text GLabel 8300 3300 2    50   Input ~ 0
NS_sensor_02
Text GLabel 8300 1400 2    50   Input ~ 0
NS_sensor_03
Text GLabel 8300 2000 2    50   Input ~ 0
NS_sensor_04
Text GLabel 8300 2100 2    50   Input ~ 0
NS_sensor_05
Text GLabel 8300 3000 2    50   Input ~ 0
NS_sensor_06
Text GLabel 8300 2200 2    50   Input ~ 0
NS_sensor_07
Text GLabel 8300 3100 2    50   Input ~ 0
NS_sensor_08
Text GLabel 8300 2300 2    50   Input ~ 0
NS_sensor_09
$Comp
L Mechanical:MountingHole H4
U 1 1 5F60C976
P 8400 5400
F 0 "H4" H 8500 5446 50  0000 L CNN
F 1 "MountingHole" H 8500 5355 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2" H 8400 5400 50  0001 C CNN
F 3 "~" H 8400 5400 50  0001 C CNN
	1    8400 5400
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 5F60CCBA
P 8400 5200
F 0 "H3" H 8500 5246 50  0000 L CNN
F 1 "MountingHole" H 8500 5155 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2" H 8400 5200 50  0001 C CNN
F 3 "~" H 8400 5200 50  0001 C CNN
	1    8400 5200
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 5F60D298
P 8400 5000
F 0 "H2" H 8500 5046 50  0000 L CNN
F 1 "MountingHole" H 8500 4955 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2" H 8400 5000 50  0001 C CNN
F 3 "~" H 8400 5000 50  0001 C CNN
	1    8400 5000
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H1
U 1 1 5F60D403
P 8400 4800
F 0 "H1" H 8500 4846 50  0000 L CNN
F 1 "MountingHole" H 8500 4755 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2" H 8400 4800 50  0001 C CNN
F 3 "~" H 8400 4800 50  0001 C CNN
	1    8400 4800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0136
U 1 1 5F638331
P 8650 4300
F 0 "#PWR0136" H 8650 4050 50  0001 C CNN
F 1 "GND" H 8655 4127 50  0000 C CNN
F 2 "" H 8650 4300 50  0001 C CNN
F 3 "" H 8650 4300 50  0001 C CNN
	1    8650 4300
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0137
U 1 1 5F638EFB
P 8650 4200
F 0 "#PWR0137" H 8650 4050 50  0001 C CNN
F 1 "VCC" H 8667 4373 50  0000 C CNN
F 2 "" H 8650 4200 50  0001 C CNN
F 3 "" H 8650 4200 50  0001 C CNN
	1    8650 4200
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0138
U 1 1 5F662544
P 4050 3300
F 0 "#PWR0138" H 4050 3150 50  0001 C CNN
F 1 "VCC" V 4068 3427 50  0000 L CNN
F 2 "" H 4050 3300 50  0001 C CNN
F 3 "" H 4050 3300 50  0001 C CNN
	1    4050 3300
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC #PWR0139
U 1 1 5F66DCFD
P 4050 4550
F 0 "#PWR0139" H 4050 4400 50  0001 C CNN
F 1 "VCC" V 4068 4677 50  0000 L CNN
F 2 "" H 4050 4550 50  0001 C CNN
F 3 "" H 4050 4550 50  0001 C CNN
	1    4050 4550
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC #PWR0140
U 1 1 5F66E117
P 4050 5800
F 0 "#PWR0140" H 4050 5650 50  0001 C CNN
F 1 "VCC" V 4068 5927 50  0000 L CNN
F 2 "" H 4050 5800 50  0001 C CNN
F 3 "" H 4050 5800 50  0001 C CNN
	1    4050 5800
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC #PWR0141
U 1 1 5F66E57D
P 2150 5800
F 0 "#PWR0141" H 2150 5650 50  0001 C CNN
F 1 "VCC" V 2168 5927 50  0000 L CNN
F 2 "" H 2150 5800 50  0001 C CNN
F 3 "" H 2150 5800 50  0001 C CNN
	1    2150 5800
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC #PWR0142
U 1 1 5F66EEED
P 2150 4550
F 0 "#PWR0142" H 2150 4400 50  0001 C CNN
F 1 "VCC" V 2168 4677 50  0000 L CNN
F 2 "" H 2150 4550 50  0001 C CNN
F 3 "" H 2150 4550 50  0001 C CNN
	1    2150 4550
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC #PWR0143
U 1 1 5F66F65E
P 2150 3300
F 0 "#PWR0143" H 2150 3150 50  0001 C CNN
F 1 "VCC" V 2168 3427 50  0000 L CNN
F 2 "" H 2150 3300 50  0001 C CNN
F 3 "" H 2150 3300 50  0001 C CNN
	1    2150 3300
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC #PWR0144
U 1 1 5F66FB82
P 2150 2050
F 0 "#PWR0144" H 2150 1900 50  0001 C CNN
F 1 "VCC" V 2168 2177 50  0000 L CNN
F 2 "" H 2150 2050 50  0001 C CNN
F 3 "" H 2150 2050 50  0001 C CNN
	1    2150 2050
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC #PWR0145
U 1 1 5F670093
P 4050 2050
F 0 "#PWR0145" H 4050 1900 50  0001 C CNN
F 1 "VCC" V 4068 2177 50  0000 L CNN
F 2 "" H 4050 2050 50  0001 C CNN
F 3 "" H 4050 2050 50  0001 C CNN
	1    4050 2050
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC #PWR0146
U 1 1 5F670474
P 2150 7050
F 0 "#PWR0146" H 2150 6900 50  0001 C CNN
F 1 "VCC" V 2168 7177 50  0000 L CNN
F 2 "" H 2150 7050 50  0001 C CNN
F 3 "" H 2150 7050 50  0001 C CNN
	1    2150 7050
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC #PWR0147
U 1 1 5F670ADB
P 4050 7050
F 0 "#PWR0147" H 4050 6900 50  0001 C CNN
F 1 "VCC" V 4068 7177 50  0000 L CNN
F 2 "" H 4050 7050 50  0001 C CNN
F 3 "" H 4050 7050 50  0001 C CNN
	1    4050 7050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4700 2200 4700 2300
Connection ~ 4700 2200
Wire Wire Line
	6300 2900 6300 3000
Connection ~ 6300 2900
Connection ~ 6300 3000
$Comp
L power:GND #PWR0148
U 1 1 5F6D501E
P 1250 1150
F 0 "#PWR0148" H 1250 900 50  0001 C CNN
F 1 "GND" H 1255 977 50  0000 C CNN
F 2 "" H 1250 1150 50  0001 C CNN
F 3 "" H 1250 1150 50  0001 C CNN
	1    1250 1150
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0149
U 1 1 5F6D62C3
P 3150 1150
F 0 "#PWR0149" H 3150 900 50  0001 C CNN
F 1 "GND" H 3155 977 50  0000 C CNN
F 2 "" H 3150 1150 50  0001 C CNN
F 3 "" H 3150 1150 50  0001 C CNN
	1    3150 1150
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0150
U 1 1 5F6D6934
P 3150 2400
F 0 "#PWR0150" H 3150 2150 50  0001 C CNN
F 1 "GND" H 3155 2227 50  0000 C CNN
F 2 "" H 3150 2400 50  0001 C CNN
F 3 "" H 3150 2400 50  0001 C CNN
	1    3150 2400
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0151
U 1 1 5F6D6E73
P 1250 2400
F 0 "#PWR0151" H 1250 2150 50  0001 C CNN
F 1 "GND" H 1255 2227 50  0000 C CNN
F 2 "" H 1250 2400 50  0001 C CNN
F 3 "" H 1250 2400 50  0001 C CNN
	1    1250 2400
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0152
U 1 1 5F6D73B2
P 1250 3650
F 0 "#PWR0152" H 1250 3400 50  0001 C CNN
F 1 "GND" H 1255 3477 50  0000 C CNN
F 2 "" H 1250 3650 50  0001 C CNN
F 3 "" H 1250 3650 50  0001 C CNN
	1    1250 3650
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0153
U 1 1 5F6D79BA
P 3150 3650
F 0 "#PWR0153" H 3150 3400 50  0001 C CNN
F 1 "GND" H 3155 3477 50  0000 C CNN
F 2 "" H 3150 3650 50  0001 C CNN
F 3 "" H 3150 3650 50  0001 C CNN
	1    3150 3650
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0154
U 1 1 5F6D7DA5
P 3150 4900
F 0 "#PWR0154" H 3150 4650 50  0001 C CNN
F 1 "GND" H 3155 4727 50  0000 C CNN
F 2 "" H 3150 4900 50  0001 C CNN
F 3 "" H 3150 4900 50  0001 C CNN
	1    3150 4900
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0155
U 1 1 5F6D827E
P 1250 4900
F 0 "#PWR0155" H 1250 4650 50  0001 C CNN
F 1 "GND" H 1255 4727 50  0000 C CNN
F 2 "" H 1250 4900 50  0001 C CNN
F 3 "" H 1250 4900 50  0001 C CNN
	1    1250 4900
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0156
U 1 1 5F6D8757
P 1250 6150
F 0 "#PWR0156" H 1250 5900 50  0001 C CNN
F 1 "GND" H 1255 5977 50  0000 C CNN
F 2 "" H 1250 6150 50  0001 C CNN
F 3 "" H 1250 6150 50  0001 C CNN
	1    1250 6150
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0157
U 1 1 5F6D8BDB
P 3150 6150
F 0 "#PWR0157" H 3150 5900 50  0001 C CNN
F 1 "GND" H 3155 5977 50  0000 C CNN
F 2 "" H 3150 6150 50  0001 C CNN
F 3 "" H 3150 6150 50  0001 C CNN
	1    3150 6150
	-1   0    0    1   
$EndComp
Text GLabel 8300 2700 2    50   Input ~ 0
SDA
Text GLabel 8300 2600 2    50   Input ~ 0
SCL
$Comp
L Switch:SW_Push SW1
U 1 1 5F772D59
P 6650 1400
F 0 "SW1" V 6696 1352 50  0000 R CNN
F 1 "SW_Push" V 6605 1352 50  0000 R CNN
F 2 "Button_Switch_SMD:SW_SPST_PTS810" H 6650 1600 50  0001 C CNN
F 3 "~" H 6650 1600 50  0001 C CNN
	1    6650 1400
	0    -1   -1   0   
$EndComp
$Comp
L Switch:SW_Push SW2
U 1 1 5F7A7164
P 8500 1200
F 0 "SW2" V 8546 1152 50  0000 R CNN
F 1 "SW_Push" V 8455 1152 50  0000 R CNN
F 2 "Button_Switch_SMD:SW_SPST_PTS810" H 8500 1400 50  0001 C CNN
F 3 "~" H 8500 1400 50  0001 C CNN
	1    8500 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 1200 6950 1200
$Comp
L power:GND #PWR0111
U 1 1 5F7B2F7A
P 8700 1200
F 0 "#PWR0111" H 8700 950 50  0001 C CNN
F 1 "GND" H 8705 1027 50  0000 C CNN
F 2 "" H 8700 1200 50  0001 C CNN
F 3 "" H 8700 1200 50  0001 C CNN
	1    8700 1200
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C1
U 1 1 5F7B41C2
P 6950 1350
F 0 "C1" H 6835 1304 50  0000 R CNN
F 1 "10uF" H 6835 1395 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6988 1200 50  0001 C CNN
F 3 "~" H 6950 1350 50  0001 C CNN
	1    6950 1350
	-1   0    0    1   
$EndComp
Connection ~ 6950 1200
Wire Wire Line
	6950 1200 7100 1200
Wire Wire Line
	6650 1600 6950 1600
Wire Wire Line
	6950 1600 6950 1500
$Comp
L Device:R R1
U 1 1 5F7B97BC
P 6950 1050
F 0 "R1" H 7020 1096 50  0000 L CNN
F 1 "12K" H 7020 1005 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6880 1050 50  0001 C CNN
F 3 "~" H 6950 1050 50  0001 C CNN
	1    6950 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 800  7700 1000
Wire Wire Line
	6950 900  6950 800 
Wire Wire Line
	6950 800  7700 800 
Connection ~ 7700 800 
$Comp
L RF_Module:ESP32-WROOM-32 U1
U 1 1 5F55E2D4
P 7700 2400
F 0 "U1" H 7700 3981 50  0000 C CNN
F 1 "ESP32-WROOM-32" H 7700 3890 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 7700 900 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf" H 7400 2450 50  0001 C CNN
	1    7700 2400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0112
U 1 1 5F7BEAA5
P 6950 1600
F 0 "#PWR0112" H 6950 1350 50  0001 C CNN
F 1 "GND" H 6955 1427 50  0000 C CNN
F 2 "" H 6950 1600 50  0001 C CNN
F 3 "" H 6950 1600 50  0001 C CNN
	1    6950 1600
	1    0    0    -1  
$EndComp
Connection ~ 6950 1600
$Comp
L Device:C C2
U 1 1 5F7D7CA8
P 8700 1050
F 0 "C2" H 8585 1004 50  0000 R CNN
F 1 "100nF" H 8585 1095 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 8738 900 50  0001 C CNN
F 3 "~" H 8700 1050 50  0001 C CNN
	1    8700 1050
	-1   0    0    1   
$EndComp
Connection ~ 8700 1200
Wire Wire Line
	7700 800  8700 800 
Wire Wire Line
	8700 800  8700 900 
$Comp
L Device:C C3
U 1 1 5F84F8D6
P 4550 1000
F 0 "C3" V 4802 1000 50  0000 C CNN
F 1 "100nF" V 4711 1000 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4588 850 50  0001 C CNN
F 3 "~" H 4550 1000 50  0001 C CNN
	1    4550 1000
	0    -1   -1   0   
$EndComp
Connection ~ 4700 1000
$Comp
L power:GND #PWR0158
U 1 1 5F850B3F
P 4400 1000
F 0 "#PWR0158" H 4400 750 50  0001 C CNN
F 1 "GND" H 4405 827 50  0000 C CNN
F 2 "" H 4400 1000 50  0001 C CNN
F 3 "" H 4400 1000 50  0001 C CNN
	1    4400 1000
	0    1    1    0   
$EndComp
$Comp
L Device:C C4
U 1 1 5F851BD4
P 4700 3800
F 0 "C4" V 4952 3800 50  0000 C CNN
F 1 "100nF" V 4861 3800 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4738 3650 50  0001 C CNN
F 3 "~" H 4700 3800 50  0001 C CNN
	1    4700 3800
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C6
U 1 1 5F852D40
P 7650 4400
F 0 "C6" V 7902 4400 50  0000 C CNN
F 1 "100nF" V 7811 4400 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7688 4250 50  0001 C CNN
F 3 "~" H 7650 4400 50  0001 C CNN
	1    7650 4400
	0    -1   -1   0   
$EndComp
Connection ~ 7500 4400
$Comp
L power:GND #PWR0160
U 1 1 5F856567
P 4550 3800
F 0 "#PWR0160" H 4550 3550 50  0001 C CNN
F 1 "GND" H 4555 3627 50  0000 C CNN
F 2 "" H 4550 3800 50  0001 C CNN
F 3 "" H 4550 3800 50  0001 C CNN
	1    4550 3800
	0    1    1    0   
$EndComp
Text GLabel 6650 1200 1    50   Input ~ 0
RESET
Text GLabel 4850 5100 0    50   Input ~ 0
RESET
$Comp
L power:+3.3V #PWR0161
U 1 1 5F85E804
P 4850 3700
F 0 "#PWR0161" H 4850 3550 50  0001 C CNN
F 1 "+3.3V" H 4865 3873 50  0000 C CNN
F 2 "" H 4850 3700 50  0001 C CNN
F 3 "" H 4850 3700 50  0001 C CNN
	1    4850 3700
	1    0    0    -1  
$EndComp
Connection ~ 4850 3700
$Comp
L power:GND #PWR0162
U 1 1 5F85F499
P 7800 4400
F 0 "#PWR0162" H 7800 4150 50  0001 C CNN
F 1 "GND" H 7805 4227 50  0000 C CNN
F 2 "" H 7800 4400 50  0001 C CNN
F 3 "" H 7800 4400 50  0001 C CNN
	1    7800 4400
	0    -1   -1   0   
$EndComp
Text GLabel 5950 6200 2    50   Input ~ 0
EP
Text GLabel 8300 3200 2    50   Input ~ 0
NS_sensor_00
Text GLabel 8300 1600 2    50   Input ~ 0
NS_sensor_01
Wire Wire Line
	4850 3700 4850 3800
Connection ~ 4850 3800
Wire Wire Line
	4850 3800 4850 3900
$Comp
L Regulator_Linear:XC6206PxxxMR U7
U 1 1 5F959F7A
P 6950 5950
F 0 "U7" H 6950 6192 50  0000 C CNN
F 1 "XC6206PxxxMR" H 6950 6101 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223" H 6950 6175 50  0001 C CIN
F 3 "https://www.torexsemi.com/file/xc6206/XC6206.pdf" H 6950 5950 50  0001 C CNN
	1    6950 5950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0163
U 1 1 5F967A51
P 6950 6250
F 0 "#PWR0163" H 6950 6000 50  0001 C CNN
F 1 "GND" H 6955 6077 50  0000 C CNN
F 2 "" H 6950 6250 50  0001 C CNN
F 3 "" H 6950 6250 50  0001 C CNN
	1    6950 6250
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 Power1
U 1 1 5F637C12
P 8850 4300
F 0 "Power1" H 8768 3975 50  0000 C CNN
F 1 "Conn_01x02" H 8768 4066 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x02_P3.81mm_Drill1.2mm" H 8850 4300 50  0001 C CNN
F 3 "~" H 8850 4300 50  0001 C CNN
	1    8850 4300
	1    0    0    1   
$EndComp
$Comp
L power:VCC #PWR0164
U 1 1 5F96B61C
P 6550 5950
F 0 "#PWR0164" H 6550 5800 50  0001 C CNN
F 1 "VCC" V 6568 6077 50  0000 L CNN
F 2 "" H 6550 5950 50  0001 C CNN
F 3 "" H 6550 5950 50  0001 C CNN
	1    6550 5950
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR0165
U 1 1 5F96C4BA
P 7350 5950
F 0 "#PWR0165" H 7350 5800 50  0001 C CNN
F 1 "+3.3V" H 7365 6123 50  0000 C CNN
F 2 "" H 7350 5950 50  0001 C CNN
F 3 "" H 7350 5950 50  0001 C CNN
	1    7350 5950
	0    1    1    0   
$EndComp
$Comp
L Device:C C7
U 1 1 5F96D8CE
P 6600 6100
F 0 "C7" H 6485 6054 50  0000 R CNN
F 1 "10uF" H 6485 6145 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6638 5950 50  0001 C CNN
F 3 "~" H 6600 6100 50  0001 C CNN
	1    6600 6100
	-1   0    0    1   
$EndComp
$Comp
L Device:C C9
U 1 1 5F96FE29
P 7300 6100
F 0 "C9" H 7185 6054 50  0000 R CNN
F 1 "10uF" H 7185 6145 50  0000 R CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7338 5950 50  0001 C CNN
F 3 "~" H 7300 6100 50  0001 C CNN
	1    7300 6100
	-1   0    0    1   
$EndComp
Wire Wire Line
	6550 5950 6600 5950
Connection ~ 6600 5950
Wire Wire Line
	6600 5950 6650 5950
Wire Wire Line
	6600 6250 6950 6250
Connection ~ 6950 6250
Wire Wire Line
	6950 6250 7300 6250
$Comp
L power:+3.3V #PWR0108
U 1 1 5F58B532
P 7700 800
F 0 "#PWR0108" H 7700 650 50  0001 C CNN
F 1 "+3.3V" H 7715 973 50  0000 C CNN
F 2 "" H 7700 800 50  0001 C CNN
F 3 "" H 7700 800 50  0001 C CNN
	1    7700 800 
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 5950 7300 5950
Connection ~ 7300 5950
Wire Wire Line
	7300 5950 7350 5950
$EndSCHEMATC
