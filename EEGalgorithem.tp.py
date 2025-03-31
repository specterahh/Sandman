import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import butter, lfilter, welch
import mne

# 示例EEG数据加载（使用MNE库加载示例数据）
# 请根据实际情况替换为自己的EEG数据加载方式
# 这里使用MNE库提供的示例数据
raw = mne.io.read_raw_fif(mne.datasets.sample.data_path() + '/MEG/sample/sample_audvis_raw.fif')
raw.pick_types(eeg=True)  # 仅选择EEG通道
data, times = raw[:, :10000]  # 提取前10000个时间点的数据

# 信号预处理
def butter_bandpass(lowcut, highcut, fs, order=5):
    nyq = 0.5 * fs
    low = lowcut / nyq
    high = highcut / nyq
    b, a = butter(order, [low, high], btype='band')
    return b, a

def butter_bandpass_filter(data, lowcut, highcut, fs, order=5):
    b, a = butter_bandpass(lowcut, highcut, fs, order=order)
    y = lfilter(b, a, data)
    return y

# 假设采样频率为250Hz
fs = 250.0
lowcut = 0.5
highcut = 30.0

# 对每个通道进行带通滤波
filtered_data = np.array([butter_bandpass_filter(channel, lowcut, highcut, fs, order=2) for channel in data])

# 特征提取：计算功率谱密度
frequencies, psd = welch(filtered_data, fs=fs, nperseg=1024)

# 分析与睡眠相关的频段
delta_band = (frequencies >= 1) & (frequencies <= 4)
theta_band = (frequencies >= 4) & (frequencies <= 8)
alpha_band = (frequencies >= 8) & (frequencies <= 13)

# 计算每个频段的功率
delta_power = np.mean(psd[:, delta_band], axis=1)
theta_power = np.mean(psd[:, theta_band], axis=1)
alpha_power = np.mean(psd[:, alpha_band], axis=1)

# 确定最适合促进睡眠的频率
# 假设最适合促进睡眠的频率是θ波和δ波中功率最高的频率
sleep_promoting_frequency = np.argmax(np.concatenate((delta_power, theta_power))) + 1  # 从1Hz开始

# 输出结果
print(f"最适合促进睡眠的频率：{sleep_promoting_frequency} Hz")

# 可视化功率谱密度
plt.figure(figsize=(12, 6))
plt.plot(frequencies, np.mean(psd, axis=0), label='Average PSD')
plt.fill_between(frequencies[delta_band], np.mean(psd, axis=0)[delta_band], alpha=0.3, label='Delta Band (1-4 Hz)')
plt.fill_between(frequencies[theta_band], np.mean(psd, axis=0)[theta_band], alpha=0.3, label='Theta Band (4-8 Hz)')
plt.fill_between(frequencies[alpha_band], np.mean(psd, axis=0)[alpha_band], alpha=0.3, label='Alpha Band (8-13 Hz)')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Power/Frequency (dB/Hz)')
plt.title('Power Spectral Density')
plt.legend()
plt.show()