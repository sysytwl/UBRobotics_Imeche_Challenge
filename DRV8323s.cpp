#include "DRV8323S.h"

DRV8323S::DRV8323S(int8_t SCK, int8_t MISO, int8_t MOSI, int8_t SS) {
    _SCK = SCK;
    _MISO = MISO;
    _MOSI = MOSI;
    _SS = SS;

    SPI.begin();
    pinMode(_SS, OUTPUT);
}

bool DRV8323S::SetDriverControl() {
    return write_reg(Driver_Control << 11 | Reserved_bit << 10 | DriverControl_.DIS_CPUV << 9 | DriverControl_.DIS_GDF << 8 |  DriverControl_.OTW_REP << 7 | DriverControl_.PWM_MODE1 << 6 | DriverControl_.PWM_MODE0 << 5 | DriverControl_.PWM_COM << 4 | DriverControl_.PWM_DIR << 3 | DriverControl_.COAST << 2 | DriverControl_.BRAKE << 1 | DriverControl_.CLR_FLT);
}

bool DRV8323S::SetGateDriveHS() {
    return write_reg(Gate_Drive_HS << 11 | 0b01111111111);
}

bool DRV8323S::SetGateDriveLS() {
    return write_reg(Gate_Drive_LS << 11 | 0b11111111111);
}

bool DRV8323S::SetOCPControl() {
    return write_reg(OCP_Control << 11 | 0b00101011001);
}

bool DRV8323S::SetCSAControl() {
    return write_reg(CSA_Control << 11 | 0b01010000011);
}

void DRV8323S::get_status() {
    uint16_t Fault_Status_1_output = read_reg(Fault_Status_1);
    Serial.print("Fault_Status_1:   "); Serial.print(Fault_Status_1_output); Serial.println();

    uint16_t VGS_Status_2_output = read_reg(VGS_Status_2);
    Serial.print("VGS_Status_2:   "); Serial.print(VGS_Status_2_output); Serial.println();

    uint16_t Driver_Control_output = read_reg(Driver_Control);
    DriverControl_.DIS_CPUV = (Driver_Control_output >> 9) & 0x01;
    DriverControl_.DIS_GDF = (Driver_Control_output >> 8) & 0x01;
    DriverControl_.OTW_REP = (Driver_Control_output >> 7) & 0x01;
    DriverControl_.PWM_MODE1 = (Driver_Control_output >> 6) & 0x01;
    DriverControl_.PWM_MODE0 = (Driver_Control_output >> 5) & 0x01;
    DriverControl_.PWM_COM = (Driver_Control_output >> 4) & 0x01;
    DriverControl_.PWM_DIR = (Driver_Control_output >> 3) & 0x01;
    DriverControl_.COAST = (Driver_Control_output >> 2) & 0x01;
    DriverControl_.BRAKE = (Driver_Control_output >> 1) & 0x01;
    DriverControl_.CLR_FLT = Driver_Control_output & 0x01;
    Serial.print("Driver_Control:   "); Serial.print(Driver_Control_output); Serial.println();

    uint16_t Gate_Drive_HS_output = read_reg(Gate_Drive_HS);
    Serial.print("Gate_Drive_HS:   "); Serial.print(Gate_Drive_HS_output); Serial.println();

    uint16_t Gate_Drive_LS_output = read_reg(Gate_Drive_LS);
    Serial.print("Gate_Drive_LS:   "); Serial.print(Gate_Drive_LS_output); Serial.println();

    uint16_t OCP_Control_output = read_reg(OCP_Control);
    Serial.print("OCP_Control:   "); Serial.print(OCP_Control_output); Serial.println();

    uint16_t CSA_Control_output = read_reg(CSA_Control);
    Serial.print("CSA_Control:   "); Serial.print(CSA_Control_output); Serial.println();
}

bool DRV8323S::write_reg(uint16_t data) {
    delay(10);
    SPI.beginTransaction(SPISettings(_spiClk, MSBFIRST, SPI_MODE1));
    digitalWrite(SS, LOW);
    delay(1);
    uint16_t output = SPI.transfer16(0 << 15 | data);
    digitalWrite(SS, HIGH);
    SPI.endTransaction();
    return ((data & 0b0000011111111111) == output) ? true : false;
}

uint16_t DRV8323S::read_reg(uint8_t address) {
    delay(10);
    SPI.beginTransaction(SPISettings(_spiClk, MSBFIRST, SPI_MODE1));
    digitalWrite(SS, LOW);
    delay(1);
    uint16_t reg_out = SPI.transfer16(1 << 15 | address << 11);
    digitalWrite(SS, HIGH);
    SPI.endTransaction();
    return reg_out;
}
