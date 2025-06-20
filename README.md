# TeamElephants_WRO_FutureEngineers_2025
Repository of Team Elephants for WRO Future Engineers 2025 National Stage

# Team Members:
- Altynbekuly Yernar
- Orazbay Beknur

# Repository Overview:
- models - contains 3D Printable and CNC files designed by us
- schemes - contains the electrical systems schematics
- src - contains the main programs and control software and
- t-photos - contains one serious and one funny photo
- v-photos - contains the photos of the robot from all required directions

# Program Arrangement and Algorithm Planning – EV3-Based Robot with Raspberry Pi Vision
Our robot is based on the LEGO Mindstorms EV3 platform and uses a Raspberry Pi as a co-processor for vision tasks. A USB webcam is connected to the Raspberry Pi to detect obstacles and corner markers, while the EV3 brick handles motor control and sensor feedback. Orientation tracking is done using the MPU6050 gyroscope/accelerometer connected to the EV3 or Raspberry Pi. The system runs fully autonomously to detect run direction, complete laps with wall and obstacle avoidance, and stop after three full laps.

# Startup Sequence
When powered on, the EV3 initializes the drive and steering motors, while the Raspberry Pi boots up and starts the vision processing script (OpenCV-based). The MPU6050 sensor is initialized to track the yaw rotation. The steering motor is centered, and the robot waits for a user to press the EV3 button to begin the run.

# Part 1 – Detecting Run Direction
The robot moves forward slowly, using ultrasonic sensors on both sides to determine track orientation. If the left sensor sees an open space >90 cm, the robot assigns the direction as clockwise (L). If it’s on the right, it assigns counter-clockwise (R). This direction determines which wall it will follow.

# Part 2 – Lap Navigation and Obstacle Avoidance
Wall Following (Round 1):
A PID control loop maintains a constant distance from the inner wall, based on ultrasonic readings. During turns, the inner wheel slows down for smoother cornering. When the robot exits a turn (based on MPU6050 angle change), it applies a temporary speed boost for 40–50 ms to accelerate on straight paths.

Simplified Control (Round 2):
In the second round, PID is reduced to P-only (proportional) control to maintain a central path. The speed is lower, and boosts are disabled.

Obstacle Detection via Raspberry Pi:
The webcam on the Raspberry Pi runs image recognition to detect red and green obstacles, as well as orange/blue corner lines. Once the Raspberry Pi detects an obstacle, it sends object color and position data (via serial or Bluetooth) to the EV3. Based on this:

The EV3 adjusts the steering to slightly right of red obstacles and slightly left of green obstacles.

If the obstacle is closer than 30 cm (estimated from bounding box size), the EV3 proportionally turns the steering motor based on object offset and distance.

Wall Collision Protection
If an object is detected within 8 cm by either ultrasonic sensor, the robot makes a quick steering correction to avoid a crash.

# Part 3 – Lap Counting and Finish Detection
The MPU6050 gyroscope tracks yaw angle over time. Each lap causes the robot to rotate ~360 degrees, and after 3 full laps (~1080° ±20°), the EV3 listens for a blue or orange corner marker detected by the Raspberry Pi camera.

Once the finish marker is seen, the EV3 performs a hard stop by briefly reversing the drive motors, ending the run precisely in the designated stop zone.
