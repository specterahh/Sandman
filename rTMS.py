import serial
import time

# 串口通信参数
port = '/dev/ttyUSB0'  # 串口设备路径，Windows系统可能是COM3
baudrate = 9600  # 波特率

# 初始化串口
ser = serial.Serial(port, baudrate, timeout=1)

# 接收频率信号
def receive_frequency():
    if ser.in_waiting > 0:
        frequency = ser.readline().decode().strip()
        return float(frequency)
    return None

# 配置TMS设备
def configure_tms(frequency):
    # 这里假设有一个TMS设备的控制接口
    print(f"配置TMS设备，刺激频率：{frequency} Hz")
    # 实际应用中，这里会调用TMS设备的API或发送控制命令

# 主程序
def main():
    while True:
        frequency = receive_frequency()
        if frequency is not None:
            configure_tms(frequency)
            break
        time.sleep(1)

if __name__ == "__main__":
    main()