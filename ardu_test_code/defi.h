#ifndef DEFI_H
#define DEFI_H

const int MPU_ADDR = 0x68;

struct DataValue
{
    unsigned long timestamp = 0;
    float accel_x = 0;
    float accel_y = 0;
    float accel_z = 0;
    float gyro_x = 0;
    float gyro_y = 0;
    float gyro_z = 0;
    float temperature = 0;
};

//void writeRegister(int address, int value);

void writeRegister(int address, int value){
    Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave
    // Queue changes in register - first address then value
    Wire.write(address);
    Wire.write(value);
    Wire.endTransmission(true);
}

void Gyrosetup(){
    Wire.begin();
    writeRegister(0x6B, B10000000); // PWR_MGMT_1 register (p.40) - device reset
    writeRegister(0x6B, B00000010); // PWR_MGMT_1 register (p.40) - wake up; X-axis gyroscope as clock
    writeRegister(0x1A, B00000110); // Low Pass Filter
}


#endif
