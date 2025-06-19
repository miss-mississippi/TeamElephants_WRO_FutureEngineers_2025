import time, cv2, numpy as np, RPi.GPIO as GPIO
from smbus2 import SMBus
from mpu6050 import mpu6050

def main_obstacle():
    gyro = Gyro()
    wall = UltrasonicWallFollower(TRIG_L,ECHO_L,TRIG_R,ECHO_R, SimplePID(2.0,0.0,0.5))
    cam  = cv2.VideoCapture(CAM_INDEX)
    lap = 0

    try:
        while lap < 3:
            corr,, = wall.correction()
            ret, frame = cam.read()
            if ret:
                hsv = cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)
                mask_r = cv2.inRange(hsv,(0,100,100),(10,255,255))
                mask_g = cv2.inRange(hsv,(50,100,100),(70,255,255))
                if mask_r.mean()>5:
                    MotorController.drive(50, turn=-30)
                    continue
                if mask_g.mean()>5:
                    MotorController.drive(50, turn=30)
                    continue
            MotorController.drive(60, turn=corr)
            if abs(gyro.angle_z()) > (lap+1)*360:
                lap += 1

        MotorController.stop()
        while True:
            ret, frame = cam.read()
            hsv = cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)
            mask_m = cv2.inRange(hsv,(140,100,100),(160,255,255))
            cnts,_ = cv2.findContours(mask_m,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
            if len(cnts)>=2:
                break
            MotorController.drive(40)

        MotorController.stop()
        time.sleep(0.5)
        MotorController.drive(-50, turn=30)
        time.sleep(1.0)
        MotorController.drive(-50, turn=-30)
        time.sleep(1.0)
        MotorController.stop()
        print("Obstacle Challenge completed!")

    except KeyboardInterrupt:
        MotorController.stop()

if _name=="main_":
    main_obstacle()
