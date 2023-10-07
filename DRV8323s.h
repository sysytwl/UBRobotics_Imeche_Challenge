#include <SPI.h>
#include <Arduino.h>



#define Fault_Status_1 0x0
#define VGS_Status_2 0x1
#define Driver_Control 0x2
#define Gate_Drive_HS 0x3
#define Gate_Drive_LS 0x4
#define OCP_Control 0x5
#define CSA_Control 0x6
#define Reserved 0x7
#define Reserved_bit 0b0

/*
R/W ADDRESS         DATA
B15 B14 B13 B12 B11 B10 B9 B8 B7 B6 B5 B4 B3 B2 B1 B0       For a write command (W0 = 0), For a read command (W0 = 1)
W0  A3  A2  A1  A0  D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
*/



class DRV8323S {
 public:
    DRV8323S(int8_t SCK_, int8_t MISO_, int8_t MOSI_, int8_t SS_) {
        _SCK = SCK_;
        _MISO = MISO_;
        _MOSI = MOSI_;
        _SS = SS_;

        SPI.begin(_SCK, _MISO, _MOSI, _SS); // init
        pinMode(_SS, OUTPUT);
    }

    struct FaultStatus {
        uint1_t vds_lc : 1;
        uint1_t vds_hc : 1;
        uint1_t vds_lb : 1;
        uint1_t vds_hb : 1;
        uint1_t vds_la : 1;
        uint1_t vds_ha : 1;
        uint1_t otsd : 1;
        uint1_t uvlo : 1;
        uint1_t gdf : 1;
        uint1_t vds_ocp : 1;
        uint1_t fault : 1;
    } FaultStatus_;

    struct VGSStatus {
        uint1_t vgs_lc : 1;
        uint1_t vgs_hc : 1;
        uint1_t vgs_lb : 1;
        uint1_t vgs_hb : 1;
        uint1_t vgs_la : 1;
        uint1_t vgs_ha : 1;
        uint1_t cpuv : 1;
        uint1_t otw : 1;
        uint1_t sc_oc : 1;
        uint1_t sb_oc : 1;
        uint1_t sa_oc : 1;
    } VGSStatus_;

    struct DriverControl {
        uint1_t CLR_FLT : 1;
        uint1_t BRAKE : 1;
        uint1_t COAST : 1;
        uint1_t PWM_MODE0 : 1;
        uint1_t PWM_MODE1 : 1;
        uint1_t OTW_REP : 1;
        uint1_t DIS_GDF : 1;
        uint1_t DIS_CPUV : 1;
    } DriverControl_;


    void disable() {
        uint32_t status = get_drv_status();
        logger.log_printf("drv8323 disabled, status1: %03x status2: %03x", status & 0xFFFF, status >> 16);
        std::string s = "drv8323 status bits";
        for(int i=0; i<11; i++) {
            if ((status >> i) & 0x1)
                s += " " + drv8323_status1_bits[i];
        }
        for(int i=0; i<11; i++) {
            if ((status >> (16+i)) & 0x1)
                s += " " + drv8323_status2_bits[i];
        }
        logger.log(s);
        GPIOC->BSRR = GPIO_BSRR_BR13; // drv disable
        DriverBase::disable();
    }

    void enable() { // TODO: add read function and compare the data, return true or false 
        write_reg(Driver_Control << 11 | Reserved_bit << 10 | DriverControl_.DIS_CPUV << 9 | DriverControl_.DIS_GDF << 8 |  DriverControl_.OTW_REP << 7 | DriverControl_.PWM_MODE1 << 6 | DriverControl_.PWM_MODE0 << 5 | DriverControl_.1PWM_COM << 4 | DriverControl_.1PWM_DIR << 3 | DriverControl_.COAST << 2 | DriverControl_.BRAKE << 1 | DriverControl_.CLR_FLT);
    }

    void get_status(){ // TODO: Check the order, MSB first
        uint16_t Fault_Status_1_output = read_reg(Fault_Status_1);
        Serial.print("Fault_Status_1:   "); Serial.print(Fault_Status_1_output); Serial.println();

        uint16_t VGS_Status_2_output = read_reg(Fault_Status_1);
        Serial.print("VGS_Status_2:   "); Serial.print(VGS_Status_2_output); Serial.println();

        uint16_t Driver_Control_output = read_reg(Driver_Control);
        driverControl.Reserved = (Driver_Control_output >> 10) & 0x01;
        driverControl.DIS_CPUV = (Driver_Control_output >> 9) & 0x01;
        driverControl.DIS_GDF = (Driver_Control_output >> 8) & 0x01;
        driverControl.OTW_REP = (Driver_Control_output >> 7) & 0x01;
        driverControl.PWM_MODE1 = (Driver_Control_output >> 6) & 0x01;
        driverControl.PWM_MODE0 = (Driver_Control_output >> 5) & 0x01;
        driverControl.PWM_COM_1 = (Driver_Control_output >> 4) & 0x01;
        driverControl.PWM_DIR_1 = (Driver_Control_output >> 3) & 0x01;
        driverControl.COAST = (Driver_Control_output >> 2) & 0x01;
        driverControl.BRAKE = (Driver_Control_output >> 1) & 0x01;
        driverControl.CLR_FLT = Driver_Control_output & 0x01;
        Serial.print("Driver_Control:   "); Serial.print(Driver_Control_output); Serial.println();
    }

    void write_reg(uint16_t data) { // 15bits of data 
        SPI.beginTransaction(SPISettings(_spiClk, MSBFIRST, SPI_MODE0));
        digitalWrite(SS, LOW);
        SPI.write16(0 << 15 | data);
        digitalWrite(SS, HIGH);
        SPI.endTransaction();
    }

    uint16_t read_reg(uint8_t address) {
        SPI.beginTransaction(SPISettings(_spiClk, MSBFIRST, SPI_MODE0));
        digitalWrite(SS, LOW);
        uint16_t reg_out = SPI.transfer16(1 << 15 | address << 11);
        digitalWrite(SS, HIGH);
        SPI.endTransaction();
        return reg_out;
    }

private:
    int8_t _SCK, _MISO, _MOSI, _SS;
    static const int _spiClk = 1000000; // 1 MHz
};