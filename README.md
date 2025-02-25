# Welcome to the PeltierChiller.ESP32 project

PeltierChiller.ESP32 is a project to provide you possibility to create flexible configuration chiller on Peltier modules for your PC. With possibility to use 24/7.

Each PC overclockers finally comes to limitation of cooling system. And most of them stops on achieved result, but some of them go farther and install chiller systems based on Peltier modules or freon. Of course this systems impossible to use as common cooling system because of uncontrolled cold production. This may cause your computer to break down because of condensation. Also you don't need all time maximum of your cooling system. This project is a solution.

## Features

* Control of the power supply of the peltier modules to maintain the temperature in the selected mode:
    * Fixed temperature mode
    * ΔT between room and cold circuit temperature
    * ΔT between dew point and cold circuit temperature
* Emulation of the PC power button. This allows you to start the computer after reaching the set temperature.
* Supporting different temperature sensors (BME280, DS18B20, NTC).
* PWM devices flexible control.
* [Logging system](#logging-system).
* [Commands](#commands) support. You can control your chiller by them.

## Hardware

**Minimal hardware requirements:**

* Power supply for peltier modules.
* ESP32 Wroom-32 30pin board.

>[!WARNING]
> 
> Verify that your board contains the diode on USB 5V input. If not, replace zero resistor to diode to protect your PC USB ports.
* BME280 temperature sensor.
* DS18B20 or NTC thermistor sensors for: cold and hot loop. I recommend to use DS18B20.
* I2C Digital potentiometer. For example: MCP4561-502E/MS. Choose it based on the regulating resistor of your power supply.
* Molex 4pin connector and resistors with different values to monitor PC power supply voltage.
* Relay to switch on/off power supply for peltier modules.
* Relay or transistor for emulating PC power button.
* MicroSD module and MicroSD card.
* Cables to connect each other.
* Peltier modules.

## Software

### Preparation

* Install driver for your ESP32 board UART controller.
* Install [Esptool.py](https://docs.espressif.com/projects/esptool/en/latest/esp32/)

### Installation

* Download certain [release](https://github.com/s1kven/PeltierChiller.ESP32/releases) and unpack.
* Move files from PeltierChiller.ESP32 folder to esptool executable file folder.
* Open directory with esptool and binaries from terminal.
* Run this command (change "{Port name}" to port of your ESP32, to get port see documentation for [Windows](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/establish-serial-connection.html#check-port-on-windows) or [Linux/ MacOS](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/establish-serial-connection.html#check-port-on-linux-and-macos)):
    ` esptool -p {Port name} -b 460800 --before default_reset --after hard_reset --chip esp32  write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x1000 PeltierChiller.Esp32.ino.bootloader.bin 0x8000 PeltierChiller.Esp32.ino.partitions.bin 0x10000 PeltierChiller.Esp32.ino.esp32da.bin `
    
    Press and hold the BOOT button on ESP32 while this command running.

### Configuration

The configuration file must be stored on MicroSD card which you connect to ESP32 before. Format MicroSD to FAT32.  The name and extension should remain the same - **"Configuration.json"**. Example of it you can find [here](https://github.com/s1kven/PeltierChiller.ESP32/blob/main/Configuration.json).


>[!WARNING]
> 
> Do not change the types of numeric values. Floating-point types must remain floating-point types, and the integer must be an integer.
> All Resistance in configuration should be passed in ohm (Ω).
> All times properties measures in milliseconds.

So, let's get acquainted with the settings:

`"RequestType": 1` don't change this option, it's for internal using.

`"Data"` block contain all chiller configuration.

1. `"Pins"`

    * `"PowerButton"` pin which should be connected to power button. Remember, system save how long you hold button, it used for `"StartupPcAfterFeatTargetTemperature"`.
    * `"PowerSignal"` pin which should be connected to power button connectors on motherboard.
    * `"ChillerPsSignal"` pin which should be connected to Relay or transistor for switch on power supply of chiller pumps and fans if you want to use `"StartupPcAfterFeatTargetTemperature": true`.
    * `"ChillerSignal"` pin which should be connected to Relay to switch on/off power supply for peltier modules.
    * `"PcVoltage"` pin which should be connected to voltage resistive divider which in turn connected to PC 12 or 5V molex power supply contact. Be careful, resulted voltage on pin should be less than 3.6V. Voltage higher than 3.6V can damage ESP32.
    
2. `"ChillerType"` operating mode selector. Supported values:

  * `"ChillerType": 1` - in this mode system tries to maintain `"TargetTemperature"`.
  * `"ChillerType": 2` - in this mode system tries to maintain `"TargetTemperature"` as ΔT between room and cold circuit temperature.
  * `"ChillerType": 3` - in this mode system tries to maintain `"TargetTemperature"` as ΔT between dew point and cold circuit temperature.

3. `"TargetTemperature"` target cold circuit temperature or ΔT for `"ChillerType": 2` and `"ChillerType": 3`.
4. `"VoltmeterThreshold"` a voltage above this threshold value will cause the chiller to turn on, below this value it will turn off.
5. `"VoltmeterR1"` R1 of your voltage resistive divider.
6. `"VoltmeterR2"` R2 of your voltage resistive divider.
7. `"StartupPcAfterFeatTargetTemperature"` `false` value enable your PC right after pushing power button. `true` value enable it only after achieving `"TargetTemperature"`.
8. `"ChillerSettings"`

    * `"PotentiometerAddress"` decimal address of I2C digital potentiometer.
    * `"ComputePidDelay"` delay for updating digital potentiometer value.
  
   Next properties used to calculate digital potentiometer value by [PID Controller](https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller).
    * `"MaxPotentiometerValue"` and `"MinPotentiometerValue"` should be installed in dependencies of your digital potentiometer resolution and peltier modules power supply possibilities. For example MCP4561-502E/MS have 256 positions and it can be installed from 0 to 255. Remember that `"MinPotentiometerValue" <= current potentiometer value <= "MaxPotentiometerValue"`.
    * `"MinIntegral"` and `"MaxIntegral"` they limit PID controller if long time no possible maintain target temperature.
    * `"Kp"`, `"Ki"`, `"Kd"`, `"Dt"` and `"PidRatio"` used for PID controlling. `"PidRatio"` add up with [PID Controller](https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller) formula result and this value sended to digital potentiometer.
    
9. `"Timers"`

    * `"ButtonMinPressTime"` fixing buttons contact chattering. Holding buttons less installed time in ms will be ignored.
    * `"CommunicationDelay"` delay between sending json packages from ESP32 to outside.
    * `"TemperatureSensorsRequestDelay"` delay between requests temperature sensors.
    * `"UpdatePwmDelay"` delay between update state of PWM devices.

10. `"TemperatureSensors"` 

    You can remove any of this block if you don't use it: `"BME280"`, `"NTC"` and `"DS18B20"`.

    Each temperature sensor have `"Name"` and `"Target"` property.
    
    `"Name"` - call your sensor as you want. It's a string value. Can be used in future application on your PC.
    
    `"Target"` - describes to the system of which component the sensor is measuring the temperature. Possible values:
    
        0 - None, this temperature sensor will be ignored
        1 - Room
        2 - PC case
        3 - Cold circuit
        4 - Hot circuit
        
    If you add few sensors to one target, system take average value between them for internal use.

    * `"BME280"` this sensors support measuring temperature, atmosphere pressure and humidity.
    
        * Register all your BME280 sensors in `"Items"` array. You can register no more 2 BME280 because `"Address"` can be 0x76 (118) or 0x77 (119). Use decimal value in configuration. See [datasheet](https://www.alldatasheet.com/html-pdf/1132060/BOSCH/BME280/173/1/BME280.html) for more information.
            ```
            {
                "Address": 118,
                "Name": "Example",
                "Target": 1
            }
            ```
    
    * `"NTC"` simple and cheap sensors. But I can't recomment to use it. Because of bad quality of ESP32 ADC.
    
        * `"AdcResolution"` set measurement accuracy. Max value 12 bits.
    
        * Register all your NTC sensors in `"Items"` array.
            ```
            {
                "Address": 34,
                "Name": "Example",
                "Target": 1,
                "Resistance": 10000,
                "ResistanceNTC": 10000,
                "BCoefficient": 3950,
                "BaseTemperature": 25,
                "SupplyVoltage": 3.3
            }
            ```
            `"Address"` is a pin ESP32 which you connect it.
            
            `"Resistance"` which connected between NTC and ESP32.
            
            `"ResistanceNTC"`, `"BCoefficient"` and `"BaseTemperature"` you should now from seller.
            
            `"SupplyVoltage"` connect your NTC to ESP32 3.3V. So, don't need to change.
            
    
    * `"DS18B20"` easy-to-use sensor, high precision. Several sensors can be connected to one contact at a unique address.
    
        * `"Pin"` data wire connected to ESP32 with 4.7kOhm resistor connected to 3.3V.
        
        * `"TemperaturePrecision"` set measurement accuracy. Max value 12 bits.
    
        * Register all your DS18B20 sensors in `"Items"` array.
            ```
            {
                "Address": [ 40, 167, 198, 20, 0, 0, 0, 79 ],
                "Name": "Example",
                "Target": 1
            }
            ```
            
            `"Address"` - array of decimal. to get address of your DS18B20 upload "Single" sketch from [DallasTemperature](https://github.com/milesburton/Arduino-Temperature-Control-Library) library examples. Convert each hex value to decimal.

11. `"PWMs"` - array of pwm devices, like fans or pumps, which you control independently.
        
    ```
        {
            "Tacho": 4,
            "PWM": 2,
            "Name": "Radiators fans",
            "SetToMaxWhenChillerLoad": 60,
            "Value": [
                {
                    "Temperature": 2.0,
                    "Load": 20
                },
                {
                    "Temperature": 6.0,
                    "Load": 50
                },
                {
                    "Temperature": 12.0,
                    "Load": 80
                }
            ],
            "ControlType": 1
        }
    ```

    `"Tacho"` pin to monitor device tachometer.
    
    `"PWM"` pin to control device.

    `"Name"` name of device.

    `"SetToMaxWhenChillerLoad"` the chiller loading threshold as a percentage after which device pwm value set to maximum. Chiller loading in percentage calculates by this equation: 
    $`chiller load = (current POT value / "MaxPotentiometerValue") * 100.0`$
    
    Set it to -1 if you want to disable this feature.

    `"ControlType"` describes which item controled. Possible values:
    
         0 - None, not specified (in this case PWM will run on maximum)
         1 - Fan
         2 - Cold circuit pump
         3 - Hot circuit pump

    `"Value"` array of items. Each item can imagine like dot on chart. It can be empty and then you get different behaviour for different `"ControlType"`:
      * `"ControlType": 1` - fan will be disabled.
      * `"ControlType": 2` or `"ControlType": 3` - pumps will run on full.

    Each dot have 2 parameters:
      * `"Temperature"` - ΔT between hot circuit and room temperature.
      * `"Load"` - speed of fan/ pump in % on this ΔT.
   
    PWM value calculates by linear function formula: $`y = kx + b`$ where y = target pwm load in percentage and x = ΔT between hot circuit and room temperature.
    
    Also there are differences between "Fan" and "Cold/ Hot circuit pump" control types if `"Load"` for first value set higher than 0. For pwm load lower this value for pumps pwm value stays on it, but for fan dropes to 0.

    Example on charts how it looks like: [PWM chart for fan](https://github.com/user-attachments/assets/119de880-bc11-488a-bdc7-769167e19c9d) and [PWM chart for pump](https://github.com/user-attachments/assets/955089fa-383e-4491-87b8-95179f4a326b).

12. `"Wifi"` - configuration for Wifi.
        
    ```
        { 
            "SSID": "YourNetworkName", 
            "Password": "YourPassword", 
            "ReconnectionTimeout": 30000, 
            "NtpServer": "pool.ntp.org" 
        }
    ```
    `"SSID"` name of your WiFi network.
    
    `"Password"` password of your WiFi network.

    `"ReconnectionTimeout"` the time in milliseconds between attempts to restore the wifi connection. Recommended value from 10000 to 30000.

    `"NtpServer"` available NTP server to update MCU clock.

13. `"Log"` - configuration for logging service.
        
    ```
        { 
            "IsEnabled": true,
            "Delay": 2000,
            "CreateNewLogTime": "00:00:00",
            "LogExpiration": 30 
        }
    ```
    `"IsEnabled"` set true/ false if you want to enable/ disable logging.
    
    `"Delay"` time in milliseconds when logging service will collect data to cache and send it to SD card after delay expiration.

    `"CreateNewLogTime"` time of day when will be created new log file. It only works if the MCU clock was previously installed from the NTP server. Enter it in UTC format.

    `"LogExpiration"` count of days. Log files older this value will be deleted.

### Logging system

The logging system records all requests ("Commands") and responses to the SD card. All log files stored in "/Logs" folder. During the first cycle of trying to connect to the WiFi and NTP server, delay time from `"ReconnectionTimeout"` configuration property, all logs saved to cache and after it finished created new log file. Depending on whether it was possible to connect to NTP server system have two ways how to work with logs:
1) If the connection to NTP is successful.
After first cycle of `"ReconnectionTimeout"` from `"Wifi"` will be created new file with this name pattern `YYYYmmdd_HHMMSS.txt`. Date and time from file name correspond to time when MCU was started in UTC. Also will start new timer for creation new log file in time specified in `"CreateNewLogTime"` from `"Wifi"` configuration property.
An expired file check is performed, and all of them will be deleted. Configured by `"LogExpiration"` from `"Log"` configuration.
Each log information string have this structure: `YYYY-mm-ddTHH:MM:SS | "milliseconds after startup" -> "Log info"`.

2) If the connection to NTP failed.
After first loop of `"ReconnectionTimeout"` from `"Wifi"` will be created new file with name `Latest.txt`. If `Latest.txt` file was available before, it will be overwritten.
Each log information string have this structure: `"milliseconds after startup" -> "Log info"`.

Log example:
`2025-02-23T05:38:57 | 5766 -> {"ResponseType":1,"Success":true,"Data":{"TemperatureSensors":[{"Type":1,"Target":3,"Temperature":26.5,"Name":""},{"Type":1,"Target":1,"Temperature":28.125,"Name":""},{"Type":1,"Target":2,"Temperature":25.5625,"Name":""},{"Type":1,"Target":4,"Temperature":26.0625,"Name":""},{"Type":3,"Target":1,"Temperature":29.29299927,"Name":""},{"Type":2,"Target":1,"Temperature":27.71999931,"Name":"","Pressure":101041.4219,"Humidity":29.37695313}]},"ErrorMessage":null}
2025-02-23T05:38:57 | 5768 -> {"ResponseType":2,"Success":true,"Data":{"Pwms":[{"ControlType":1,"Name":"Radiators fans","RPM":0}]},"ErrorMessage":null}`

### Commands

Commands used to control your chiller and improve user expirience. If you want to send command to Esp32 - send JSON to the serial port of it. You can find all examples in repository "/Requests/Commands" folder.

Invalid command response example:
`{"ResponseType":101,"Success":false,"Data":null,"ErrorMessage":"InvalidInput{wrong command}"}`

Commands list:
1) "SoftReset" - reboot Esp32.
2) "UpdateConfiguration" - update "Configuration.json" file.
3) "UpdateAndApplyConfiguration" - update "Configuration.json" file and restarts main services to apply configuration.
4) "UpdateTempConfiguration" - restarts main services to apply configuration provided by command. It's applies only for current session.
5) "ResetTempConfiguration" - restarts main services and load configuration from "Configuration.json".

### Data monitor

  There are plans to create an application to communicate with ESP 32. But for now you can use any serial monitor program to see parameters of your chiller. Only set correct COM port and Baudrate to 115200.
