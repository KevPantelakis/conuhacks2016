import requests
import serial

r = requests.get('http://104.236.69.2/output.txt')

print(r.text)

s = serial.Serial('/dev/ttyAMA0', 9600, timeout=1)

try:
    s.open()

except SystemError:
    print('Error : Already open')

try:
    while True:
        r = requests.get('http://104.236.69.2/output.txt')
        s.write(r)
except KeyboardInterrupt:
    s.close()
