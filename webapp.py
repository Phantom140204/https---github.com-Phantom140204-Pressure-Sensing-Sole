from flask import Flask, request, jsonify, render_template
import socket
import time
import webbrowser
import tkinter as tk
import threading

server_time = time.time()

hostname = socket.gethostname()
ip = socket.gethostbyname(hostname)
port = 8000

sensor_data = [[0, 0, 0], [0, 0, 0]]
sensor_left_data_available = 0
sensor_right_data_available = 0

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html', ip=(ip + ":" + str(port)))

@app.route('/sensor_data', methods=['POST'])
def sensordata():
    global sensor_data, sensor_left_data_available, sensor_right_data_available

    data = request.get_json()
    if (data['device_id'] == 0):
        sensor_data[0][0] = data['fsr_toe']
        sensor_data[0][1] = data['fsr_middle']
        sensor_data[0][2] = data['fsr_heel']
        sensor_left_data_available = 1

    elif (data['device_id'] == 1):
        sensor_data[1][0] = data['fsr_toe']
        sensor_data[1][1] = data['fsr_middle']
        sensor_data[1][2] = data['fsr_heel']
        sensor_right_data_available = 1
    
    return jsonify({'status': 'success'})

@app.route('/sensor_data_0', methods=['GET'])
def sensor_data_0():
    global sensor_data, sensor_left_data_available

    if (sensor_left_data_available):
        sensor_left_data_available = 0
        return jsonify({
            'timestamp': int((time.time() - server_time) * 1000),
            'fsr_toe': sensor_data[0][0],
            'fsr_middle': sensor_data[0][1],
            'fsr_heel': sensor_data[0][2],
            'is_valid': 1
        })
    else:
        return jsonify({
            'timestamp': int((time.time() - server_time) * 1000),
            'fsr_toe': sensor_data[0][0],
            'fsr_middle': sensor_data[0][1],
            'fsr_heel': sensor_data[0][2],
            'is_valid': 0
        })

@app.route('/sensor_data_1', methods=['GET'])
def sensor_data_1():
    global sensor_data, sensor_right_data_available

    if (sensor_right_data_available):
        sensor_right_data_available = 0
        return jsonify({
            'timestamp': int((time.time() - server_time) * 1000),
            'fsr_toe': sensor_data[1][0],
            'fsr_middle': sensor_data[1][1],
            'fsr_heel': sensor_data[1][2],
            'is_valid': 1
        })
    else:
        return jsonify({
            'timestamp': int((time.time() - server_time) * 1000),
            'fsr_toe': sensor_data[1][0],
            'fsr_middle': sensor_data[1][1],
            'fsr_heel': sensor_data[1][2],
            'is_valid': 0
        })

def run_flask():
    app.run(host=ip, port=port)

def run_gui():
    root = tk.Tk()
    root.title("Pressure Sensing Sole")

    # Create a label or any other GUI element
    label = tk.Label(root, text="Flask Web Server is Running")
    label.pack()

    # Start Flask server in a separate thread
    flask_thread = threading.Thread(target=run_flask)
    flask_thread.daemon = True  # Make sure the thread exits when the program exits
    flask_thread.start()

    # Open the webpage in the default browser
    webbrowser.open('http://' + ip + ':' + str(port) + '/')

    root.mainloop()


if __name__ == '__main__':
    run_gui()