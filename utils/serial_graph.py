import serial
import serial.tools.list_ports
from matplotlib.animation import FuncAnimation
import sys
from pynput import keyboard
import matplotlib.pyplot as plt


# Configuration
BAUD_RATE = 9600
VALUE_PREFIX = "#V: "

# Find available serial ports
ports = list(serial.tools.list_ports.comports())
if not ports:
    print("No serial ports found.")
    sys.exit(1)
elif len(ports) == 1:
    SERIAL_PORT = ports[0].device
    print(f"Found one serial port: {SERIAL_PORT}")
else:
    print("Available serial ports:")
    for i, port in enumerate(ports):
        print(f"  [{i}] {port.device} - {port.description}")
    while True:
        try:
            idx = int(input("Select port number: "))
            if 0 <= idx < len(ports):
                SERIAL_PORT = ports[idx].device 
                break
            else:
                print("Invalid selection. Try again.")
        except ValueError:
            print("Please enter a valid number.")

# Data storage
data = []
paused = False
running = True

# Initialize serial connection
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
except Exception as e:
    print(f"Error opening serial port: {e}")
    sys.exit(1)

# Set up the plot
fig, ax = plt.subplots(figsize=(12, 5))  # Wider window
line, = ax.plot([], [])
ax.set_xlabel('Sample')
ax.set_ylabel('Value')
ax.set_title('Serial Data Plot (Space: Pause/Resume, Esc: Exit)')
ax.set_ylim(0, 1024)  # Absolute scale

def on_press(key):
    global paused, running
    try:
        if key == keyboard.Key.space:
            paused = not paused
            status = "PAUSED" if paused else "RUNNING"
            print(f"\n{status}")
        elif key == keyboard.Key.esc:
            running = False
            plt.close(fig)
            ser.close()
            sys.exit(0)
    except Exception as e:
        pass

# Start keyboard listener
listener = keyboard.Listener(on_press=on_press)
listener.start()

def update(frame):
    global data
    if not paused and ser.in_waiting:
        try:
            line_data = ser.readline().decode('utf-8').strip()
            if line_data.startswith(VALUE_PREFIX):
                value_str = line_data[len(VALUE_PREFIX):]
                value = float(value_str)
                data.append(value)
                # Keep only last 100 points for better visualization
                if len(data) > 100:
                    data = data[-100:]
        except ValueError:
            pass  # Ignore invalid values
        except Exception as e:
            print(f"Error reading serial data: {e}")
    if data:
        line.set_data(range(len(data)), data)
        ax.set_xlim(max(0, len(data)-100), len(data))
        fig.canvas.draw_idle()
    return line,

# Start animation
ani = FuncAnimation(fig, update, interval=100, blit=False, cache_frame_data=False)

print(f"Connected to {SERIAL_PORT} at {BAUD_RATE} baud")
print("Press SPACE to pause/resume, ESC to exit")

plt.show()

# Cleanup
ser.close()
listener.stop()
