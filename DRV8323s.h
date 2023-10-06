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


extern uint16_t drv_regs_error;

static const std::string Fault_Status_1_str[11] = {"vds_lc", "vds_hc", "vds_lb", "vds_hb", "vds_la", "vds_ha", "otsd", "uvlo", "gdf", "vds_ocp", "fault"};
static const std::string VGS_Status_2_str[11] = {"vgs_lc", "vgs_hc", "vgs_lb", "vgs_hb", "vgs_la", "vgs_ha", "cpuv", "otw", "sc_oc", "sb_oc", "sa_oc"};
static const std::string Driver_Control_str[11] = {"CLR_FLT", "BRAKE", "COAST", "1PWM_DIR", "1PWM_COM", "PWM_MODE0", "PWM_MODE1", "OTW_REP", "DIS_GDF", "DIS_CPUV", "Reserved"};

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

    void enable() {
        
    }

/*
R/W ADDRESS         DATA
B15 B14 B13 B12 B11 B10 B9 B8 B7 B6 B5 B4 B3 B2 B1 B0       For a write command (W0 = 0)
W0  A3  A2  A1  A0  D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
*/

    void get_status(){
        uint16_t Fault_Status_1_output = read_reg(Fault_Status_1);
        Serial.print("Fault_Status_1:   "); Serial.print(Fault_Status_1_output); Serial.println();

        uint16_t VGS_Status_2_output = read_reg(Fault_Status_1);
        Serial.print("VGS_Status_2:   "); Serial.print(VGS_Status_2_output); Serial.println();

        uint16_t Driver_Control_output = read_reg(Driver_Control);
        Serial.print("Driver_Control:   "); Serial.print(Driver_Control_output); Serial.println();
    }

    void write_reg(uint16_t data) {
        SPI.beginTransaction(SPISettings(_spiClk, MSBFIRST, SPI_MODE0));
        digitalWrite(SS, LOW);
        SPI.write16(data);
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