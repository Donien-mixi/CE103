import socket
import threading
import json
from flask import Flask, render_template
from flask_socketio import SocketIO

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*", async_mode='threading')

TCP_IP = "0.0.0.0"
TCP_PORT = 5001

def start_tcp_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    
    try:
        server.bind((TCP_IP, TCP_PORT))
        server.listen(5)
        print(f"--- [DEBUG SERVER] Ready on TCP port {TCP_PORT} ---")
    except Exception as e:
        print(f"--- Port Error: {e} ---")
        return

    while True:
        client, addr = server.accept()
        socketio.emit('update_web_console', {'msg': f"--- Robot Connected: {addr} ---\n", 'type': 'system'})
        
        try:
            while True:
                data = client.recv(1024)
                if not data: break
                
                messages = data.decode('utf-8', errors='ignore').strip().split('\n')
                for msg in messages:
                    if not msg: continue
                    
                    try:
                        telemetry_data = json.loads(msg)
                        if telemetry_data.get("type") == "telemetry":
                            socketio.emit('update_telemetry', telemetry_data)
                    except json.JSONDecodeError:
                        socketio.emit('update_web_console', {'msg': msg + '\n', 'type': 'robot'})
                        
        except Exception as e:
            print(f"TCP Error: {e}")
        finally:
            client.close()
            socketio.emit('update_web_console', {'msg': "--- Robot Disconnected ---\n", 'type': 'system'})

@app.route('/')
def index():
    return render_template('index.html')

if __name__ == '__main__':
    threading.Thread(target=start_tcp_server, daemon=True).start()
    socketio.run(app, host='0.0.0.0', port=5000, debug=False)