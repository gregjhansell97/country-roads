#
# Adapted from Google Cloud Platform tutorial on Python App
# Engine Hello, world! program
# Source: https://github.com/GoogleCloudPlatform/python-docs-samples/blob/master/appengine/standard_python37/hello_world/main.py
#


from flask import Flask, render_template, Response, request
import mysql.connector
import json

# If `entrypoint` is not defined in app.yaml, App Engine will look for an app
# called `app` in `main.py`.
app = Flask(__name__)

db = mysql.connector.connect(
    unix_socket="/cloudsql/country-roads-256405:us-east1:country-roads",
    user="root",
    passwd="denver",
    database="country_roads_db")


@app.route('/data',methods=['POST'])
def recv_post_data():
    body = request.get_json()

    query = "INSERT INTO measurements (carId,color,heading,gasLevel,speed,measurementTime,relayStationId) VALUES "

    try:
        for m in body:
            measurement = "(" + str(m["carId"]) + ",'" + m["color"] + "'," + str(m.get("heading","NULL")) + "," + str(m.get("gasLevel","NULL")) + "," + str(m.get("speed","NULL")) + ",'" + str(m["time"]) + "'," + str(m.get("relayStationId","NULL")) + "),"
            query += measurement
            
        query = query[:-1]+';' # remove trailing comma and end with a semicolon

        mycursor = db.cursor(buffered=True)
        mycursor.execute(query)

        db.commit()
        mycursor.close()
                
        response = Response("Great job Alex, you POSTed some data!", status=200, mimetype='application/json')
        response.headers.add('Access-Control-Allow-Origin', '*')
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = '0'
        response.headers["Pragma"] = "no-cache"
        return response

#    except OperationalError as e:
#        mycursor.close()
#        db.close
#        db = mysql.connector.connect(
#            unix_socket="/cloudsql/country-roads-256405:us-east1:country-roads",
#            user="root",
#            passwd="denver",
#            database="country_roads_db")
    except Exception as e:
        print(str(e))
        response = Response("Bad request", status=400)    
        response.headers.add('Access-Control-Allow-Origin', '*')
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = '0'
        response.headers["Pragma"] = "no-cache"
        return response
    
@app.route('/')
def hello():
    """Return a friendly HTTP greeting."""
    return 'Hello World!'

@app.route('/cars/current')
def get_cars_now():
    query = "select * from measurements where (select count(*) from measurements as m where m.carId=measurements.carId and m.measurementTime>=measurements.measurementTime)<=1;"

    mycursor = db.cursor(buffered=True)
    mycursor.execute(query)

    results = []
    try:
        for (mId,cId,color,heading,gasLevel,speed,time,rsid) in mycursor:
            results.append({
                "measurementId": mId,
	        "carId": cId,
	        "color": color,
                "heading": heading,
	        "gasLevel": gasLevel,
	        "speed": speed,
	        "time": str(time),
	        "relayStationId": rsid })
        mycursor.close()
    except OperationalError as e:
        pass
#        mycursor.close()
#        db.close
#        db = mysql.connector.connect(
#            unix_socket="/cloudsql/country-roads-256405:us-east1:country-roads",
#            user="root",
#            passwd="denver",
#            database="country_roads_db")
    
    print("Response: "+json.dumps(results))
    response = Response(json.dumps(results), status=200, mimetype='application/json')
    response.headers.add('Access-Control-Allow-Origin', '*')
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = '0'
    response.headers["Pragma"] = "no-cache"
    return response

if __name__ == '__main__':
    # This is used when running locally only. When deploying to Google App
    # Engine, a webserver process such as Gunicorn will serve the app. This
    # can be configured by adding an `entrypoint` to app.yaml.
    print(db)
    app.run(host='127.0.0.1', port=8080, debug=True)
