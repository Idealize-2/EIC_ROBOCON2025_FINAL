// https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
// https://raw.githubusercontent.com/ricardoquesada/esp32-arduino-lib-builder/master/bluepad32_files/package_esp32_bluepad32_index.json
// add in additional board URL and download esp32_bluepad32 by Ricardo Quesada

#ifdef Bluetooth

void initProtocal()
{

  bd_addr_t controller_addr;

  // Parse human-readable Bluetooth address.
  sscanf_bd_addr(controller_addr_string, controller_addr);

  // Notice that this address will be added in the Non-volatile-storage (NVS).
  // If the device reboots, the address will still be stored.
  // Adding a duplicate value will do nothing.
  // You can add up to four entries in the allowlist.
  uni_bt_allowlist_add_addr(controller_addr);

  // Finally, enable the allowlist.
  // Similar to the "add_addr", its value gets stored in the NVS.
  uni_bt_allowlist_set_enabled(true);

  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
}


// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      ControllerProperties properties = ctl->getProperties();
      const uint8_t* addr = properties.btaddr;
      Serial.printf("Controller Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id,
                    properties.product_id);
      myControllers[i] = ctl;
      foundEmptySlot = true;
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println("CALLBACK: Controller connected, but could not found empty slot");
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      foundController = true;
      break;
    }
  }

  if (!foundController) {
    Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
  }
}

bool updateData( Controller_Status& C_state ) {
  bool newData = BP32.update();
  for (auto myController : myControllers) {

    //for Debug
    // Serial.print( "hasData? = ");
    // Serial.println( myController->hasData() );
    // Serial.print( "Connect? = ");
    // Serial.println( myController->isConnected() );

    if (myController && myController->isConnected() && myController->hasData()) {

      //Serial.println("has data");
      if (myController->isGamepad()) {
        // LEFT ANALOG
        C_state.x = myController->axisX();
        C_state.y = myController->axisY();

        //RIGHT ANALOG
        C_state.rx = myController->axisRX();
        C_state.ry = myController->axisRY();

        x_ctrl = mapf(myController->axisX(), -511, 512, -1, 1);
        y_ctrl = mapf(myController->axisY(), -511, 512, -1, 1);
        x_turn = mapf(myController->axisRX(), -511, 512, -1, 1);
        y_turn = mapf(myController->axisRY(), -511, 512, -1, 1);
      }

      // RIGHT BUTTON
      C_state.rt = myController->y();
      C_state.rl = myController->x();
      C_state.rr = myController->b();
      C_state.rd = myController->a();

      // // LEFT BUTTON
      C_state.lt =  myController->dpad() == 1;
      C_state.ll =  myController->dpad() == 8;
      C_state.lr =  myController->dpad() == 4;
      C_state.ld =  myController->dpad() == 2;
      
      // R3 L3
      C_state.l3 = myController->thumbL();
      C_state.r3 = myController->thumbR();


      // R2 R1 L2 L1
      C_state.l1 = myController->l1();
      C_state.l2 = myController->l2();

      C_state.r1 = myController->r1();
      C_state.r2 = myController->r2();


      // SELECT HOME
      C_state.home = myController->miscHome();
      C_state.select = myController->miscSelect();
      // bool select , home ;

      

    }
  }
  return newData;
}


#endif

