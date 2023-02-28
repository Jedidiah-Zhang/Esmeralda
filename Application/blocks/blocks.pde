import processing.core.*;

PShape box3d;                // PShape object for drawing the 3D box
float boxSize = 100.0;       // Box size
float rotateX = 0.0;         // Rotation angle around X axis
float rotateY = 0.0;         // Rotation angle around Y axis
float scaleFactor = 1.0;     // Scale factor for the box
float previousX, previousY;  // Previous mouse position
boolean isDragging = false;  // Flag for tracking if the box is being dragged
float dragOffsetX, dragOffsetY; // Offset between mouse and box position when dragging
float boxPosX = 0.0;         // X position of the box
float boxPosY = 0.0;         // Y position of the box

void setup() {
    size(600, 600, P3D);    // Create a 3D canvas with size of 600x600 pixels
    box3d = createShape(BOX, boxSize, boxSize, boxSize);   // Create a box shape object
    box3d.setStroke(false); // Do not draw the wireframe of the box
}

void draw() {
    background(0);
    lights();                  // Enable lighting
    translate(width/2 + boxPosX, height/2 + boxPosY, -500); 
    // Move the origin to the center of the canvas and translate towards the screen by 500 pixels, plus the box position
    rotateX(rotateX);          // Rotate the box around X axis
    rotateY(rotateY);          // Rotate the box around Y axis
    scale(scaleFactor);        // Scale the box
    shape(box3d);              // Draw the box
}

void mousePressed() {
    previousX = mouseX;
    previousY = mouseY;

    // Check if right mouse button is pressed
    if (mouseButton == RIGHT) {
        // Calculate offset between mouse position and box position
        dragOffsetX = mouseX - boxPosX;
        dragOffsetY = mouseY - boxPosY;
        isDragging = true;
    }
}

void mouseDragged() {
    float dx = mouseX - previousX;
    float dy = mouseY - previousY;
    
    // If the box is being dragged with the right mouse button
    if (isDragging) {
        // Update the box position based on the mouse position and offset
        boxPosX = mouseX - dragOffsetX;
        boxPosY = mouseY - dragOffsetY;
    } else {
        rotateY += dx * 0.01;
        rotateX -= dy * 0.01;
    }
    
    previousX = mouseX;
    previousY = mouseY;
}

void mouseReleased() {
    // Set the dragging flag to false when the mouse button is released
    isDragging = false;
}

void mouseWheel(MouseEvent event) {
    float delta = event.getCount();
    scaleFactor -= delta * 0.1;
    scaleFactor = max(scaleFactor, 0.1);
    scaleFactor = min(scaleFactor, 5.0);
}