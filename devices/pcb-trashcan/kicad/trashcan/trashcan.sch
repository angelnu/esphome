EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RF_Module:ESP32-WROOM-32 U?
U 1 1 5F61431D
P 3750 3600
F 0 "U?" H 3750 5181 50  0000 C CNN
F 1 "ESP32-WROOM-32" H 3750 5090 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 3750 2100 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf" H 3450 3650 50  0001 C CNN
	1    3750 3600
	1    0    0    -1  
$EndComp
$Comp
L Driver_Motor:TMC2130-LA U?
U 1 1 5F6154FE
P 6050 3550
F 0 "U?" H 6050 2261 50  0000 C CNN
F 1 "TMC2130-LA" H 6050 2170 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-36-1EP_5x6mm_P0.5mm_EP3.6x4.1mm" H 6050 1950 50  0001 C CNN
F 3 "http://www.trinamic.com/fileadmin/assets/Products/ICs_Documents/TMC2130_datasheet.pdf" H 6050 1850 50  0001 C CNN
	1    6050 3550
	1    0    0    -1  
$EndComp
$Comp
L Sensor_Motion:LIS3DH U?
U 1 1 5F61B0DE
P 8000 4000
F 0 "U?" H 8000 3311 50  0000 C CNN
F 1 "LIS3DH" H 8000 3220 50  0000 C CNN
F 2 "Package_LGA:LGA-16_3x3mm_P0.5mm_LayoutBorder3x5y" H 8100 2950 50  0001 C CNN
F 3 "https://www.st.com/resource/en/datasheet/cd00274221.pdf" H 7800 3900 50  0001 C CNN
	1    8000 4000
	1    0    0    -1  
$EndComp
$Comp
L Motor:Stepper_Motor_unipolar_6pin M?
U 1 1 5F61BC71
P 7600 2850
F 0 "M?" H 7788 2974 50  0000 L CNN
F 1 "Stepper_Motor_unipolar_6pin" H 7788 2883 50  0000 L CNN
F 2 "" H 7610 2840 50  0001 C CNN
F 3 "http://www.infineon.com/dgdl/Application-Note-TLE8110EE_driving_UniPolarStepperMotor_V1.1.pdf?fileId=db3a30431be39b97011be5d0aa0a00b0" H 7610 2840 50  0001 C CNN
	1    7600 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 3350 6850 3350
Wire Wire Line
	6850 3350 6850 2250
Wire Wire Line
	6850 2250 7700 2250
Wire Wire Line
	7700 2250 7700 2550
Wire Wire Line
	7600 2550 7600 2300
Wire Wire Line
	7600 2300 6900 2300
Wire Wire Line
	6900 2300 6900 3550
Wire Wire Line
	6900 3550 6750 3550
Wire Wire Line
	6750 3450 6950 3450
Wire Wire Line
	6950 3450 6950 2350
Wire Wire Line
	6950 2350 7500 2350
Wire Wire Line
	7500 2350 7500 2550
Wire Wire Line
	6750 4350 7000 4350
Wire Wire Line
	7000 4350 7000 2850
Wire Wire Line
	7000 2850 7300 2850
Wire Wire Line
	6750 4450 7050 4450
Wire Wire Line
	7050 4450 7050 2750
Wire Wire Line
	7050 2750 7300 2750
Wire Wire Line
	6750 4550 7100 4550
Wire Wire Line
	7100 4550 7100 2950
Wire Wire Line
	7100 2950 7300 2950
$EndSCHEMATC
