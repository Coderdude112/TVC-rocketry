// Handles logging the data
void logging() {
  flightTime = dTime - launchTime;
  if (dTime - lastLog >= loggingInterval) {
    myFile.println();
    myFile.print(dTime);
    myFile.print(", ");
    myFile.print(flightTime);
    myFile.print(", ");
    myFile.print(trueAlt);
    myFile.print(", ");
    myFile.print(accelX);
    myFile.print(", ");
    myFile.print(accelY);
    myFile.print(", ");
    myFile.print(accelZ);
    myFile.print(", ");
    myFile.print(gyroX);
    myFile.print(", ");
    myFile.print(gyroY);
    myFile.print(", ");
    myFile.print(gyroZ);
    myFile.print(", ");
    myFile.print(yaw);
    myFile.print(", ");
    myFile.print(pitch);
    myFile.print(", ");
    myFile.print(roll);
    myFile.print(", ");
    myFile.print(xOut);
    myFile.print(", ");
    myFile.print(yOut);
    myFile.print(", ");
    myFile.print(state);
    myFile.print(", ");
    myFile.print(pyroState);
    myFile.print(", ");
    myFile.flush(); // TODO: Figure out if .flush() is async
    lastLog = dTime;
  }
}

// Set the headers for the CSV file
void loggingLabel() {
  myFile.println();
  myFile.print("dTime");
  myFile.print(", ");
  myFile.print("flightTime");
  myFile.print(", ");
  myFile.print("trueAlt");
  myFile.print(", ");
  myFile.print("accelX");
  myFile.print(", ");
  myFile.print("accelY");
  myFile.print(", ");
  myFile.print("accelZ");
  myFile.print(", ");
  myFile.print("gyroX");
  myFile.print(", ");
  myFile.print("gyroY");
  myFile.print(", ");
  myFile.print("gyroZ");
  myFile.print(", ");
  myFile.print("yaw");
  myFile.print(", ");
  myFile.print("pitch");
  myFile.print(", ");
  myFile.print("roll");
  myFile.print(", ");
  myFile.print("xOut");
  myFile.print(", ");
  myFile.print("yOut");
  myFile.print(", ");
  myFile.print("state");
  myFile.print(", ");
  myFile.print("pyroState");
  myFile.print(", ");
  myFile.flush();
}
