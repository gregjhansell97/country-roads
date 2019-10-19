#
# Adapted from Google Cloud Platform tutorial on Python App
# Engine Hello, world! program
# Source: https://github.com/GoogleCloudPlatform/python-docs-samples/blob/master/appengine/standard_python37/hello_world/main.py
#


from flask import Flask, render_template
import mysql.connector

# If `entrypoint` is not defined in app.yaml, App Engine will look for an app
# called `app` in `main.py`.
app = Flask(__name__,static_url_path='')

db = mysql.connector.connect(
    host="country-roads-256405:us-east1:country-roads",
    user="root",
    passwd="denver",
    database="country_roads_db")

@app.route('/')
def hello():
    """Return a friendly HTTP greeting."""
    return 'Hello World!'

@app.route('/cars/now')
def serve_static_cars_now():
    """Serve the static canned response for /cars/now"""
    return app.send_static_file('sample_get_cars_now.json')

@app.route('/test')
def test_sql():
    mycursor = mydb.cursor()

    mycursor.execute("SHOW TABLES")

    retval = ""
    for x in mycursor:
        retval+=str(x)
    return retval


if __name__ == '__main__':
    # This is used when running locally only. When deploying to Google App
    # Engine, a webserver process such as Gunicorn will serve the app. This
    # can be configured by adding an `entrypoint` to app.yaml.
    print(db)
    app.run(host='127.0.0.1', port=8080, debug=True)
