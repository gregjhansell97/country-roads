import requests
import json
import random
import datetime
import time
import signal

url = "https://country-roads-256405.appspot.com/data"

while 1:
    try:
        randomJson = [{}]
        carIndex = random.choice([0,1,2,3])
        randomJson[0]['carId'] = [100, 200, 300, 400][carIndex]
        randomJson[0]['color'] = ["green", "blue", "orange", "yellow"][carIndex]
        randomJson[0]['heading'] = random.randrange(0,255)
        randomJson[0]['gasLevel'] = random.randrange(0,100)/100.0
        randomJson[0]['speed'] = random.randrange(0,80)
        randomJson[0]['time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        randomJson[0]['relayStationId'] = random.choice([1234567890,345678901,567890123,67890])

        payload = json.dumps(randomJson)
        #print(json.dumps(randomJson, sort_keys=True, indent=4))
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
        time.sleep(2)
    except KeyboardInterrupt:
        break