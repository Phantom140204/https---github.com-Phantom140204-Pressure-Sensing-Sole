from flask import Flask, request, jsonify, render_template
from flask_socketio import SocketIO, emit
from engineio.async_drivers import threading
import socket
import time
import webbrowser
import tkinter as tk
import threading
import csv

server_time = time.time()

hostname = socket.gethostname()
ip = socket.gethostbyname(hostname)
port = 8000
recording = False
csv_file = 'sensor_data.csv'

app = Flask(__name__)
socketio = SocketIO(app, async_mode='threading')  # using threading async mode

with open(csv_file, mode ='w', newline='') as file:
    writer = csv.DictWriter(file, fieldnames=['timestamp', 'device_id', 'fsr_toe', 'fsr_middle', 'fsr_heel'])
    writer.writeheader()

@app.route('/')
def index():
    return render_template('index.html', ip=(ip + ":" + str(port)), record_status=recording)

@app.route('/sensor_data', methods=['POST'])
def sensor_data():
    data = request.get_json()
    data['timestamp'] = int((time.time() - server_time) * 1000)
    socketio.emit('update', data)

    if recording:
        with open(csv_file, mode='a', newline='') as file:
            writer = csv.DictWriter(file, fieldnames=['timestamp', 'device_id', 'fsr_toe', 'fsr_middle', 'fsr_heel'])
            writer.writerow(data)

    return jsonify({'status': 'success'})

@socketio.on('recording_status')
def handle_recording_toggle(data):
    global recording
    recording = data['recording']
    socketio.emit('recording_ack',{'recording': recording})


def run_flask():
    socketio.run(app, host=ip, port=port, allow_unsafe_werkzeug=True)  # Disable reloader when running in thread

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
