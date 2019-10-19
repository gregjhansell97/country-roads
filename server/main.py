#
# Adapted from Google Cloud Platform tutorial on Python App
# Engine Hello, world! program
# Source: https://github.com/GoogleCloudPlatform/python-docs-samples/blob/master/appengine/standard_python37/hello_world/main.py
#


from flask import Flask, render_template, Response
import mysql.connector
import json

# If `entrypoint` is not defined in app.yaml, App Engine will look for an app
# called `app` in `main.py`.
app = Flask(__name__,static_url_path='')

db = mysql.connector.connect(
    unix_socket="/cloudsql/country-roads-256405:us-east1:country-roads",
    user="root",
    passwd="denver",
    database="country_roads_db")

print(db)

@app.route('/')
def hello():
    """Return a friendly HTTP greeting."""
    return 'Hello World!'

#@app.route('/cars/now')
#def serve_static_cars_now():
#    """Serve the static canned response for /cars/now"""
#    return app.send_static_file('sample_get_cars_now.json')

@app.route('/test')
def test_sql():
    mycursor = db.cursor()

    mycursor.execute("SHOW TABLES")

    retval = ""
    for x in mycursor:
        retval+=str(x)
    return retval

@app.route('/cars/all')
def get_all_cars():
    pass

# TODO: CHANGE THIS ENDPOINT PATH
@app.route('/cars/new')
def get_cars_now():
    query = "select * from measurements where (select count(*) from measurements as m where m.carId=measurements.carId and m.measurementTime<=measurements.measurementTime)<=1;"

    mycursor = db.cursor()
    mycursor.execute(query)

    results = []
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

    response = Response(json.dumps(results), status=200, mimetype='application/json')
    return response

if __name__ == '__main__':
    # This is used when running locally only. When deploying to Google App
    # Engine, a webserver process such as Gunicorn will serve the app. This
    # can be configured by adding an `entrypoint` to app.yaml.
    print(db)
    app.run(host='127.0.0.1', port=8080, debug=True)
