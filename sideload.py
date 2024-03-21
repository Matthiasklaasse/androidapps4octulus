import os

apps = os.listdir('apps')

devices = os.popen(r'.\platform-tools\adb devices -l').read()
new_devices = []
unauthorized_devices = []
target = None

for device in devices.split('\n')[1:]:
    if 'device' in device:
        new_devices.append(device.split('model:')[1].split(' device:')[0])

    if 'unauthorized' in device:
        unauthorized_devices.append(device.split('model:')[1].split(' device:')[0])

devices = new_devices
del new_devices

if len(devices) == 0:
    if(len(unauthorized_devices) == 0):
        print('No devices found')
    else:
        print(f'No devices found however we found unauthorized device(s): \n')
        for device in unauthorized_devices:
            print(device + '\n')
    print('Make sure you set up developer mode correcty and trusted this pc')
    exit()

if len(devices) == 1:
    print('Found: '+devices[0])
    if input('Do you wanna continue with this device?: y/n: \n') != 'y':
        exit()
else:
    print('Found multiple devices, only plug in the target device, andriod phone with usb debugging on also detceted so turn off usb debugging or disconnect your phone')

for app in apps:
    print('installing app ' + app)
    os.popen(r'.\platform-tools\adb install "apps\\' + app + '"')
#./adb shell pm list packages -f
