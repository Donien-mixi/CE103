import socket
import threading
import tkinter as tk
from tkinter import scrolledtext

# --- Cấu hình ---
IP = "0.0.0.0"
PORT = 5000

def start_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    try:
        server.bind((IP, PORT))
        server.listen(5)
        log_message(f"--- TCP Server listening on port {PORT} ---\n", "system")
    except Exception as e:
        log_message(f"--- Port Error: {e} ---\n", "system")
        return

    while True:
        client, addr = server.accept()
        log_message(f"--- Connected by {addr} ---\n", "system")
        
        try:
            while True:
                # Nhận bao nhiêu ghi ra bấy nhiêu ngay lập tức
                data = client.recv(1024)
                if not data: break
                
                # Decode ignore để tránh crash nếu nhận dữ liệu rác/binary
                msg = data.decode('utf-8', errors='ignore')
                log_message(msg, "robot")
        except Exception as e:
            log_message(f"\n--- Error: {e} ---\n", "system")
        finally:
            client.close()
            log_message("\n--- Client disconnected ---\n", "system")

def log_message(message, tag):
    # Đưa việc cập nhật giao diện vào luồng chính của Tkinter để tránh lỗi thread
    root.after(0, lambda: _update_ui(message, tag))

def _update_ui(message, tag):
    console.configure(state='normal')
    console.insert(tk.END, message, tag + "_style")
    console.configure(state='disabled')
    console.see(tk.END)

# --- Khởi tạo GUI ---
root = tk.Tk()
root.title("Hercules Raw Console")
root.geometry("800x500")

# Header y hệt Hercules
top_bar = tk.Frame(root, bg="#d4d0c8", height=30)
top_bar.pack(side=tk.TOP, fill=tk.X)
tk.Label(top_bar, text=" Received Data ", bg="#d4d0c8", fg="black", font=("Arial", 9)).pack(side=tk.LEFT)

# Màn hình chính
console = scrolledtext.ScrolledText(root, bg="white", font=("Courier New", 10), state='disabled')
console.pack(padx=2, pady=2, fill=tk.BOTH, expand=True)

# Định nghĩa màu chuẩn Hercules
console.tag_config("robot_style", foreground="black")  # Dữ liệu thô màu đen
console.tag_config("system_style", foreground="#FF00FF") # Trạng thái màu hồng cánh sen

# Chạy Server
thread = threading.Thread(target=start_server, daemon=True)
thread.start()

root.mainloop()