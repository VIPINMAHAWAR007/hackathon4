import face_recognition
import cv2
import RPi.GPIO as GPIO
from time import sleep
import serial
import os

PORT = '/dev/ttyACM0'
BAUD = 9600
ser = serial.Serial(PORT, BAUD,timeout = 10)

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(14, GPIO.IN)
GPIO.setup(15, GPIO.OUT) #External - Near Cam
GPIO.setup(17, GPIO.OUT) #Internal
GPIO.output(15,0)
GPIO.output(17,0)

def Speak(word):
    file = "/tmp/pico.wav"
    cmd = "pico2wave -l=en-US -w=" + file + " \"" + word + "\""
    os.system(cmd)
    os.system("aplay "+ file)
    sleep(1)
    os.remove(file)

def Open():
    GPIO.output(15,1)
    cmd = '1'
    ser.write(cmd.encode())
    Speak("Opening the door")
    sleep(3)
    GPIO.output(15,0)

def Close():
    cmd = '0'
    Speak("Closing the door")
    ser.write(cmd.encode())

def Name():
    video_capture = cv2.VideoCapture(0)

    Vipin_image = face_recognition.load_image_file("Vipin.jpg")
    Vipin_face_encoding = face_recognition.face_encodings(Vipin_image)[0]

    # Create arrays of known face encodings and their names
    known_face_encodings = [
        Vipin_face_encoding
    ]
    known_face_names = [
        "Mr. Vipin"
    ]

    # Initialize some variables
    face_locations = []
    face_encodings = []
    face_names = []
    process_this_frame = True

    while True:
        ret, frame = video_capture.read()
        name = ""
        small_frame = cv2.resize(frame, (0, 0), fx=0.25, fy=0.25)

        rgb_small_frame = small_frame[:, :, ::-1]

        if process_this_frame:
            face_locations = face_recognition.face_locations(rgb_small_frame)
            face_encodings = face_recognition.face_encodings(rgb_small_frame, face_locations)

            face_names = []
            for face_encoding in face_encodings:
                matches = face_recognition.compare_faces(known_face_encodings, face_encoding)
                name = "Unknown"

                if True in matches:
                    first_match_index = matches.index(True)
                    name = known_face_names[first_match_index]

                face_names.append(name)

        process_this_frame = not process_this_frame

        for (top, right, bottom, left), name in zip(face_locations, face_names):
            top *= 4
            right *= 4
            bottom *= 4
            left *= 4

            cv2.rectangle(frame, (left, top), (right, bottom), (0, 0, 255), 2)
            
            cv2.rectangle(frame, (left, bottom - 35), (right, bottom), (0, 0, 255), cv2.FILLED)
            font = cv2.FONT_HERSHEY_DUPLEX
            cv2.putText(frame, name, (left + 6, bottom - 6), font, 1.0, (255, 255, 255), 1)

            face = frame[left+50:top+50, right+50:bottom-50]
            cv2.imwrite('Image.jpg',frame)
        cv2.imshow('Video', frame)
        print(name)
        if name:
            break

    video_capture.release()
    cv2.destroyAllWindows()
    return name

Vipin_image = face_recognition.load_image_file("Vipin.jpg")
Vipin_face_encoding = face_recognition.face_encodings(Vipin_image)[0]

# Create arrays of known face encodings and their names
known_face_encodings = [
    Vipin_face_encoding
]
known_face_names = [
    "Mr. Vipin"
]

try:
    sw = 1
    while True:
        print("waiting for input")
        sw = GPIO.input(14)
        if sw == 0:
            print("Input got")
            GPIO.output(17,1)
            sleep(0.5)
            GPIO.output(17,0)
            name = Name()
            if name == "Unknown":
                Speak("Sir")
                sleep(0.2)
                Speak("an unknown person is on the door")
                sleep(0.2)
                Speak("Sending you the photo")
                sleep(0.2)
                p = input("Do you want to open the gate?(Y/N) :")
                if p == 'Y':
                    Open()
                    sleep(4)
                    Close()
                elif p == 'N':
                    Close()
                else:
                    print("Wrong Key Entered!!!")
            else :
                Speak("Sir")
                sleep(0.2)
                Speak(name + " is on the door")
                sleep(0.2)
                Speak("Permission granted")
                sleep(0.1)
                Open()
                Speak("Welcome "+name)
                sleep(4)
                Close()
                sw = 1
        else:
            GPIO.output(17,0)
            sw = 1
    
finally:
    GPIO.cleanup()
            
