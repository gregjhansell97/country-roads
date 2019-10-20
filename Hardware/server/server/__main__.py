# -*- coding: utf-8 -*-
import requests
import json
import datetime
import time
import signal
import serial
from serial.tools import list_ports 
import struct

class CarMsg:
    """
    """
    def __init__(self, raw_msg):
        format_ = "=BBBBBB"
        loc, car_id, color, heading, gas_level, speed = struct.unpack(
                format_, 
                raw_msg)
        self.loc = loc 
        self.car_id = car_id
        self.color = ["green", "blue", "orange", "yellow"][color]
        self.heading = heading
        self.gas_level = gas_level
        self.speed = speed
    def size():
        return struct.calcsize("=BBBBBB")

    def to_json(self):
        return {
            "relayStationId": self.loc,
            "carId": self.car_id,
            "color": self.color,
            "heading": self.heading,
            "gasLevel": self.gas_level,
            "speed": self.speed,
            "time": datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        }

    def __str__(self):
        return str(self.loc) + ": " + str(self.car_id)

def connect_to_arduino(baud_rate=9600):
    ports = list(list_ports.comports())

    node = None
    for p in ports:
        if "Arduino"in p.description:
            device = p.device
            node = serial.Serial(device, baud_rate)
            break
    if node is None and ports:
        device = ports[0].device
        node = serial.Serial(device, baud_rate)
    return node

def listen_for_data():
    url = "https://country-roads-256405.appspot.com/data"
    print("listening for data")
    print(CarMsg.size())
    arduino = connect_to_arduino(9600)
    if arduino is None:
        print("failed to connect")
        return
    else:
        print("connected")
    # need to have message size!
    try:
        while True: 
            raw_msg = arduino.read(CarMsg.size())
            msg = CarMsg(raw_msg)
            print(str(msg))
            # this is where you publish
            payload = json.dumps([msg.to_json()])
            print(payload)

            headers = {
                'Content-Type': "application/json",
                'User-Agent': "PostmanRuntime/7.18.0",
                'Accept': "*/*",
                'Cache-Control': "no-cache",
                'Postman-Token': "6f5d80ee-bbe8-46c1-bada-2d8347696976,a9a888d7-89a3-4eb5-98fb-9308e8ed472f",
                'Host': "country-roads-256405.appspot.com",
                'Accept-Encoding': "gzip, deflate",
                'Content-Length': "162",
                'Connection': "keep-alive",
                'cache-control': "no-cache"
            }
            response = requests.request("POST", url, data=payload, headers=headers)
            print(response.text)

    except KeyboardInterrupt:
        return

if __name__ == "__main__":
    listen_for_data()

