# IoT Arduino Bridge (LoRa Receiver)

## Documentation

This is a brief explanation of the major functionality of the ParkSense LoRa receiver.  
This code handles all the data structure sent using LoRa and works as a data collector directly connected with the bridge.

## General Information

This documentation, along with our project, is based on an Arduino Uno using a specific LoRa module (E220-900T22D). More information can be found in its [datasheet](https://www.cdebyte.com/pdf-down.aspx?id=1136).

## Data Structure

This module expects to receive LoRa messages formatted in the following way:
- **Header** (in `byte` format)
- **Parking ID** (in `uint8_t` format)
- **Footer** (in `byte` format)

The header byte identifies the beginning of a message and is used to detect errors on the transmission line. The footer has a similar purpose. Finally, the parking ID is the unique identifier of a parking slot.

## Loop

The basic behavior of this program is straightforward: it continuously checks (according to the `deltaReading` variable) if there is available data sent via LoRa. If there is, it reads it, treating the data as a [packet](#data-structure). Once the data is received, the Arduino Bridge sends it to the actual bridge through the serial channel.

## Float Handling

In the code, you will also find a section that shows how to read and send float data using the serial channel. This feature can be useful if you want to make the initialization phase adaptive, improving scalability and system flexibility.

## Configuration

This final piece of code, kindly provided by Renzo Mischianti in [this blog post](https://mischianti.org/it/ebyte-lora-e220-llcc68-per-arduino-esp32-o-esp8266-configurazione-3/), is used to check and, with its counterpart, set the current configuration of the LoRa E220 module.
