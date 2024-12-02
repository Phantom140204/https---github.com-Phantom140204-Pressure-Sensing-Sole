from flask import Flask, request, jsonify, render_template
from flask_socketio import SocketIO, emit
from engineio.async_drivers import threading
import socket
import time
import webbrowser
import tkinter as tk
import threading

server_time = time.time()

hostname = socket.gethostname()
ip = socket.gethostbyname(hostname)
port = 8000

app = Flask(__name__)
socketio = SocketIO(app, async_mode='threading')  # using threading async mode

@app.route('/')
def index():
    return render_template('index.html', ip=(ip + ":" + str(port)))

@app.route('/sensor_data', methods=['POST'])
def sensor_data():
    data = request.get_json()
    data['timestamp'] = int((time.time() - server_time) * 1000)
    socketio.emit('update', data)
    return jsonify({'status': 'success'})

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
