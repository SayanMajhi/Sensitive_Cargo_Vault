# Architecture

The system consists of:

- Arduino (data acquisition + FSM)
- Python (middleware)
- Firebase RTDB (cloud)

Flow:
Sensors → Arduino → Serial → Python → Firebase
