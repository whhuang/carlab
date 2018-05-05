import cam

framerate = 1000 # frames / second
while True:
    # Step 1: Extract Car Vectors from Image
    car_vectors = cam.get_car_vectors()

    # Step 2: Calculate Packet Fields
    colors = ((255,0,0),(0,255,0),(0,0,255)) # RGB colors of each car's LED
    error_vectors = car_vectors
    for i in range(0, len(error_vectors)):
        error_vectors[i].append(0) # r = 0

    # Step 3: Transmit Packet
    cam.tx_pkt(error_vectors, colors)

    
    if cam.cv2.waitKey(1000/framerate) == 113: # Press 'q' to end
        break;
