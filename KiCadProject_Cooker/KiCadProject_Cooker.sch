EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Electrical Schemtic"
Date ""
Rev "A"
Comp ""
Comment1 "ESP32 model version 1"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:Thermistor TH-A1
U 1 1 6078632B
P 3950 2050
F 0 "TH-A1" V 4192 2050 50  0000 C CNN
F 1 "Thermistor" V 4101 2050 50  0000 C CNN
F 2 "Connector:Banana_Jack_2Pin" H 3950 2050 50  0001 C CNN
F 3 "~" H 3950 2050 50  0001 C CNN
	1    3950 2050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3750 2950 3750 2050
$Comp
L Device:R R1
U 1 1 6078E346
P 4300 2700
F 0 "R1" V 4507 2700 50  0000 C CNN
F 1 "1k" V 4416 2700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 4230 2700 50  0001 C CNN
F 3 "~" H 4300 2700 50  0001 C CNN
	1    4300 2700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4150 2050 4150 2350
$Comp
L Device:Thermistor TH-P1
U 1 1 60795372
P 4800 2050
F 0 "TH-P1" V 5042 2050 50  0000 C CNN
F 1 "Thermistor" V 4951 2050 50  0000 C CNN
F 2 "Connector:Banana_Jack_2Pin" H 4800 2050 50  0001 C CNN
F 3 "~" H 4800 2050 50  0001 C CNN
	1    4800 2050
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R2
U 1 1 6079537A
P 5150 2700
F 0 "R2" V 5357 2700 50  0000 C CNN
F 1 "25k" V 5266 2700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5080 2700 50  0001 C CNN
F 3 "~" H 5150 2700 50  0001 C CNN
	1    5150 2700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5000 2050 5000 2350
$Comp
L Device:C C3
U 1 1 6079633B
P 6000 2350
F 0 "C3" H 6115 2396 50  0000 L CNN
F 1 "0.1uF" H 6115 2305 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 6038 2200 50  0001 C CNN
F 3 "~" H 6000 2350 50  0001 C CNN
	1    6000 2350
	0    -1   -1   0   
$EndComp
$Comp
L Device:Thermistor TH-P2
U 1 1 60796341
P 5650 2050
F 0 "TH-P2" V 5892 2050 50  0000 C CNN
F 1 "Thermistor" V 5801 2050 50  0000 C CNN
F 2 "Connector:Banana_Jack_2Pin" H 5650 2050 50  0001 C CNN
F 3 "~" H 5650 2050 50  0001 C CNN
	1    5650 2050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5450 2950 5450 2050
$Comp
L Device:R R3
U 1 1 60796349
P 6000 2700
F 0 "R3" V 6207 2700 50  0000 C CNN
F 1 "25k" V 6116 2700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5930 2700 50  0001 C CNN
F 3 "~" H 6000 2700 50  0001 C CNN
	1    6000 2700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5850 2050 5850 2350
Connection ~ 5850 2700
Connection ~ 5000 2700
$Comp
L Device:C C2
U 1 1 6079F8A7
P 5150 2350
F 0 "C2" H 5265 2396 50  0000 L CNN
F 1 "0.1uF" H 5265 2305 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 5188 2200 50  0001 C CNN
F 3 "~" H 5150 2350 50  0001 C CNN
	1    5150 2350
	0    -1   -1   0   
$EndComp
Connection ~ 4150 2700
Wire Wire Line
	3750 2950 4600 2950
Wire Wire Line
	4600 2050 4600 2950
Connection ~ 4600 2950
Wire Wire Line
	4600 2950 5450 2950
Wire Wire Line
	3150 2950 3750 2950
Connection ~ 3750 2950
Wire Wire Line
	4450 2700 4500 2700
Wire Wire Line
	4500 2700 4500 4250
Wire Wire Line
	4500 4250 3150 4250
Connection ~ 4500 4250
Wire Wire Line
	5300 2700 5350 2700
Wire Wire Line
	5350 2700 5350 4250
Connection ~ 5350 4250
Wire Wire Line
	5350 4250 4500 4250
Wire Wire Line
	6150 2700 6200 2700
Wire Wire Line
	6200 2700 6200 4250
Wire Wire Line
	5350 4250 6200 4250
Wire Wire Line
	4150 3350 3150 3350
Wire Wire Line
	5000 3450 3150 3450
Wire Wire Line
	5850 3550 3150 3550
$Comp
L esp32:ESP32-WROOM U1
U 1 1 607988C6
P 2200 3550
F 0 "U1" H 2175 4937 60  0000 C CNN
F 1 "ESP32-WROOM" H 2175 4831 60  0000 C CNN
F 2 "esp32:ESP32-WROOM" H 2550 4900 60  0001 C CNN
F 3 "" H 1750 4000 60  0001 C CNN
	1    2200 3550
	-1   0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 60817F0F
P 4300 2350
F 0 "C1" H 4415 2396 50  0000 L CNN
F 1 "0.1uF" H 4415 2305 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 4338 2200 50  0001 C CNN
F 3 "~" H 4300 2350 50  0001 C CNN
	1    4300 2350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4150 2700 4150 3350
Wire Wire Line
	5000 2700 5000 3450
Wire Wire Line
	5850 2700 5850 3550
Connection ~ 4150 2350
Wire Wire Line
	4150 2350 4150 2700
Connection ~ 5000 2350
Wire Wire Line
	5000 2350 5000 2700
Connection ~ 5850 2350
Wire Wire Line
	5850 2350 5850 2700
Wire Wire Line
	4450 2350 4500 2350
Wire Wire Line
	4500 2350 4500 2700
Connection ~ 4500 2700
Wire Wire Line
	5300 2350 5350 2350
Wire Wire Line
	5350 2350 5350 2700
Connection ~ 5350 2700
Wire Wire Line
	6150 2350 6200 2350
Wire Wire Line
	6200 2350 6200 2700
Connection ~ 6200 2700
$EndSCHEMATC
