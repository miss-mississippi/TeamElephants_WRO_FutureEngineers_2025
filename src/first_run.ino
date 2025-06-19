import time
import RPi.GPIO as GPIO

TRIG_R, ECHO_R = 16, 25  
TRIG_L, ECHO_L = 19, 26   

DRV_IN1, DRV_IN2, DRV_PWM = 5, 6, 13   
STR_IN1, STR_IN2, STR_PWM = 17, 27, 12 

GPIO.setmode(GPIO.BCM)
for p in (TRIG_R, TRIG_L): GPIO.setup(p, GPIO.OUT)
for p in (ECHO_R, ECHO_L): GPIO.setup(p, GPIO.IN)
GPIO.setup(DRV_IN1, GPIO.OUT)
GPIO.setup(DRV_IN2, GPIO.OUT)
GPIO.setup(DRV_PWM, GPIO.OUT)
drv_pwm = GPIO.PWM(DRV_PWM, 200)
drv_pwm.start(0)
GPIO.setup(STR_IN1, GPIO.OUT)
GPIO.setup(STR_IN2, GPIO.OUT)
GPIO.setup(STR_PWM, GPIO.OUT)
str_pwm = GPIO.PWM(STR_PWM, 200)
str_pwm.start(0)

def measure(trig, echo):
    GPIO.output(trig, True)
    time.sleep(1e-5)
    GPIO.output(trig, False)
    t0 = time.time()
    while GPIO.input(echo) == 0:
        t0 = time.time()
    t1 = t0
    while GPIO.input(echo) == 1:
        t1 = time.time()
    return (t1 - t0) * 17150 

def drive(speed):
    if speed < 0:
        GPIO.output(DRV_IN1, False)
        GPIO.output(DRV_IN2, True)
    else:
        GPIO.output(DRV_IN1, True)
        GPIO.output(DRV_IN2, False)
    drv_pwm.ChangeDutyCycle(min(abs(speed), 100))

def steer(speed):
    if speed < 0:
        GPIO.output(STR_IN1, False)
        GPIO.output(STR_IN2, True)
    else:
        GPIO.output(STR_IN1, True)
        GPIO.output(STR_IN2, False)
    str_pwm.ChangeDutyCycle(min(abs(speed), 100))

try:
    while True:
        dr = measure(TRIG_R, ECHO_R)
        dl = measure(TRIG_L, ECHO_L)

        if dr == 0 or dr > 200:
            steer(50)
            drive(-50)
            continue
        if dl == 0 or dl > 200:
            steer(-50)
            drive(-50)
            continue

        err = dl - dr            
        corr = err * 0.5         # kp=0.5

 
        steer(corr)
        drive(-50)

        time.sleep(0.05)

except KeyboardInterrupt:
    pass

finally:
    drive(0); steer(0)
    drv_pwm.stop(); str_pwm.stop()
    GPIO.cleanup()
