import socket
import threading
import json
import time
import random

HOST = '127.0.0.1' 
PORT = 5001

def simulate_stm32_telemetry(client_socket):
    while True:
        try:
            # Mô phỏng dữ liệu mảng theo đúng vị trí cấu hình
            telemetry = {
                "type": "telemetry",
                "line": [
                    random.choices([0, 1], weights=[0.9, 0.1])[0] for _ in range(5)
                ], # Sinh mảng 5 phần tử cho dò vạch
                "e3f": [
                    random.choices([0, 1], weights=[0.85, 0.15])[0] for _ in range(6)
                ], # Sinh mảng 6 phần tử cho hồng ngoại
                "motor_l": random.randint(-255, 255), # Có thể lùi nên có số âm
                "motor_r": random.randint(-255, 255)
            }
            message = json.dumps(telemetry) + "\n"
            client_socket.sendall(message.encode('utf-8'))
            time.sleep(0.5) 
        except Exception:
            break 

def run_client():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        client_socket.connect((HOST, PORT))
        print("Đã kết nối! Đang gửi dữ liệu mảng cảm biến...\nNhập log text (gõ 'exit' để thoát):")
        threading.Thread(target=simulate_stm32_telemetry, args=(client_socket,), daemon=True).start()
        
        while True:
            text = input()
            if text.lower() == 'exit': break
            if text.strip(): client_socket.sendall((text + "\n").encode('utf-8'))
    except Exception as e:
        print(f"Lỗi: {e}")
    finally:
        client_socket.close()

if __name__ == "__main__":
    run_client()