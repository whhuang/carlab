import cam

framerate = 1000 # frames / second
while True:
    # Step 1: Extract Car Vectors from Image
    car_vectors = cam.get_car_vectors()

    # Step 2: Calculate Packet Fields
    velocities = (1, 1, 1) # Desired speed of each car
    colors = ((255,0,0),(0,255,0),(0,0,255)) # RGB colors of each car's LED

    # Step 3: Transmit Packet
    cam.tx_pkt(car_vectors, velocities, colors)

    
    if cam.cv2.waitKey(1000/framerate) & 0xFF == ord('q'):
        break;
